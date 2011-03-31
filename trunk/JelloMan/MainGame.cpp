//=============================================================================
// MainGame by Bastian Damman & Sebastiaan Sprengers
//=============================================================================
#include "MainGame.h"
#include "ContentManager.h"
#include "AudioEngine.h"
#include "xact3.h"

MainGame::MainGame()	:	m_dTtime(0),
							m_pLevel(0),
							m_pEditorCamera(0),
                            m_pLightController(0),
							m_pAudioEngine(0),
							m_pTestSound(0),
							m_bResourcesLoaded(false),
							m_bDebug(false),
							m_pEditorGUI(0),
							m_Angle(0),
							m_pTrackingCamera(0),
							m_pPhysXEngine(0),
							m_pDeferredRenderer(0),
							m_pForwardRenderer(0),
							m_pPostProcessor(0),
							m_pEdgeDetectionEffect(0)
{

}

MainGame::~MainGame()
{
	delete m_pEditorCamera;
    delete m_pLightController;
	delete m_pAudioEngine;
	delete m_pTestSound;
	delete m_pEditorGUI;
	delete m_pTrackingCamera;
	delete m_pDeferredRenderer;
	delete m_pForwardRenderer;
	delete m_pPostProcessor;
	delete m_pEdgeDetectionEffect;
	delete m_pLevel;

	m_pPhysXEngine = 0;
}

void MainGame::Initialize(GameConfig& refGameConfig)
{
	// TEST - gameconfig word nog file!
	refGameConfig.SetTitle(_T("Happy Engine"));
	refGameConfig.SetWindowWidth(1440);
	refGameConfig.SetWindowHeight(800);
	refGameConfig.SetBlox2DAntiAliasing(true);
	refGameConfig.SetKeyboardLayout(GameConfig::KEYBOARD_LAYOUT_AZERTY);
	refGameConfig.UsePhysX(true);
}

void MainGame::LoadResources(ID3D10Device* pDXDevice, PhysX* pPhysXEngine)
{
    Content->Init(pDXDevice);

	// CAMERA
	m_pEditorCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
									static_cast<int>(BLOX_2D->GetWindowSize().height)	);
    m_pEditorCamera->LookAt(Vector3(-225, 115, -205), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pEditorCamera->SetLens(BLOX_2D->GetWindowSize().width/BLOX_2D->GetWindowSize().height,PiOver4,10.0f,10000.0f);

	m_pTrackingCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
									static_cast<int>(BLOX_2D->GetWindowSize().height)	);
	m_pTrackingCamera->LookAt(Vector3(-225, 115, -205), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pTrackingCamera->SetLens(BLOX_2D->GetWindowSize().width/BLOX_2D->GetWindowSize().height,PiOver4,10.0f,10000.0f);

	// RENDERERS
	m_pDeferredRenderer = new DeferredRenderer(pDXDevice);
	m_pForwardRenderer = new ForwardRenderer(pDXDevice);
	m_pPostProcessor = new PostProcessor(	pDXDevice,
											static_cast<int>(BLOX_2D->GetWindowSize().width),
											static_cast<int>(BLOX_2D->GetWindowSize().height));
	m_pEdgeDetectionEffect = Content->LoadEffect<EdgeDetectionPostEffect>(_T("postEdgeDetection.fx"));
    m_pEdgeDetectionEffect->SetTechnique(0);

	m_pDeferredRenderer->Init(	static_cast<int>(BLOX_2D->GetWindowSize().width),
								static_cast<int>(BLOX_2D->GetWindowSize().height));
	m_pDeferredRenderer->SetClearColor(Vector4(0.1f, 0.1f, 0.9f, 1.0f));
	m_pPostProcessor->SetEffect(m_pEdgeDetectionEffect);

    // LIGHTCONTROLLER
    m_pLightController = new LightController();

    SpotLight sl;
        //Omni 1
        sl = SpotLight();
        sl.position = Vector3(0.0f,400.0f,0.0f);
        sl.color = Color(0.8f, 0.8f, 0.5f, 1.0f);
        sl.multiplier = 1.5f;
		sl.AttenuationStart = 0;
		sl.AttenuationEnd = 2000;
		sl.direction = Vector3(0, -1, 0);
		sl.power = 0.5f;
		sl.shadowsEnabled = false;
		sl.lightEnabled = true;
        m_pLightController->AddLight(sl);

	// LEVEL
	m_pLevel = new Level(pDXDevice);
	m_pLevel->Initialize();

	// AUDIO
	tstring projectLocation = tstring(_T("./Audio/Win/JelloMan"));
	m_pAudioEngine = new AudioEngine(projectLocation);
	m_pAudioEngine->Initialize();

	m_pTestSound = new Sound(m_pAudioEngine,_T("BackgroundMusic"),_T("BackGroundMusicVolume"));
	m_pTestSound->PreLoad();
	m_pTestSound->SetLoopCount(1);
	m_pTestSound->SetVolume(90);

	// GUI
	m_pEditorGUI = new EditorGUI();
	m_pEditorGUI->Initialize();

	// PHYSX
	m_pPhysXEngine = pPhysXEngine;

	m_bResourcesLoaded = true;
}

void MainGame::UpdateScene(const float dTime)
{
	// dtime
	m_dTtime = dTime;

	if (m_pEditorGUI->GetMode() != EditorGUI::MODE_PLAY)
		m_pEditorCamera->Tick(dTime);
	else
		m_pLevel->Tick(dTime);

	m_pAudioEngine->DoWork();
	m_pTestSound->Tick();

	if (m_pEditorGUI->GetMode() == EditorGUI::MODE_PLAY)
		m_pPhysXEngine->Simulate(dTime);

	if (CONTROLS->IsKeyPressed(VK_SPACE))
	{
		if (!m_pTestSound->IsPlaying())
		{
			m_pTestSound->Play();
		}
		else
		{
			m_pTestSound->Pause();
		}
	}
	
	if (CONTROLS->IsKeyDown(VK_ADD))
	{
		m_pTestSound->SetVolume(m_pTestSound->GetVolume() + 1);
	}
	else if (CONTROLS->IsKeyDown(VK_SUBTRACT))
	{
		m_pTestSound->SetVolume(m_pTestSound->GetVolume() - 1);
	}

	if (m_pEditorGUI->GetLightButton()->IsActive())
		m_pDeferredRenderer->SetLightMode(LIGHT_MODE_LIT);
	else
		m_pDeferredRenderer->SetLightMode(LIGHT_MODE_UNLIT);
}

void MainGame::DrawScene()
{
	RenderContext renderContext(m_pEditorCamera, m_pLightController);
		
	// --------------------------------------
	//			   RENDER SCENE
	// --------------------------------------

	// POST PROCESS
	m_pPostProcessor->Begin();

	// START DEFERRED
	m_pDeferredRenderer->Begin();

	// DRAW
	m_pLevel->DrawDeferred(&renderContext);

	// END DEFERRED
	m_pDeferredRenderer->End(&renderContext);

	// START FORWARD
	m_pForwardRenderer->Begin(m_pDeferredRenderer);

	// DRAW
	m_pLevel->DrawForward(&renderContext);

	// END FORWARD
	m_pForwardRenderer->End();

	// POST PROCESS
	m_pPostProcessor->End();
		
	// --------------------------------------

	m_pEditorGUI->Tick(&renderContext);
	m_pEditorGUI->Draw();

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->SetFont(_T("Arial"),true,false,12);

	CONTROLS->ResetMouse();		
}

void MainGame::OnResize(ID3D10RenderTargetView* pRTView)
{
	if (m_bResourcesLoaded)
	{
		m_pDeferredRenderer->OnResized(	static_cast<int>(BLOX_2D->GetWindowSize().width),
										static_cast<int>(BLOX_2D->GetWindowSize().height));
	}
}
void MainGame::Release()
{
	if (m_bResourcesLoaded)
	{
		m_pDeferredRenderer->OnResize();
	}
}


void MainGame::LoadScreen()
{
	BLOX_2D->SetColor(ColorF(ColorF::CornflowerBlue));
	BLOX_2D->FillBackGround();

	BLOX_2D->SetColor(ColorF(ColorF::LightGray));
	BLOX_2D->DrawGrid(3,RectF(0,0,BLOX_2D->GetWindowSize().width,
									BLOX_2D->GetWindowSize().height));

	BLOX_2D->SetFont(_T("Arial"),true,false,30);
	BLOX_2D->SetColor(0,0,0);
	BLOX_2D->DrawString(_T("Loading Resources..."),RectF(10,0,BLOX_2D->GetWindowSize().width,
																BLOX_2D->GetWindowSize().height-10),
																Blox2D::HORIZONTAL_ALIGN_LEFT,
																Blox2D::VERTICAL_ALIGN_BOTTOM);

	D2D1_MATRIX_3X2_F rot;
	D2D1MakeRotateMatrix(90,Point2F(BLOX_2D->GetWindowSize().width/2,
									BLOX_2D->GetWindowSize().height/2),&rot);
	BLOX_2D->SetTransform(rot);

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->SetFont(_T("Arial"),true,false,200);
	BLOX_2D->DrawStringCentered(_T(":D"),-20);

	BLOX_2D->ResetTransform();
}