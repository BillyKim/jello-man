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
							m_pEdgeDetectionEffect(0),
							m_pRenderContext(0)
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
	delete m_pLevel;
	delete m_pRenderContext;

    delete Content;

	m_pPhysXEngine = 0;
}

void MainGame::Initialize(GameConfig& refGameConfig)
{
	// TEST - gameconfig word nog file!
	refGameConfig.SetTitle(_T("Happy Engine"));
	refGameConfig.SetWindowWidth(1800);
	refGameConfig.SetWindowHeight(1000);
	refGameConfig.SetBlox2DAntiAliasing(true);
	refGameConfig.SetKeyboardLayout(GameConfig::KEYBOARD_LAYOUT_AZERTY);
	refGameConfig.UsePhysX(true);
}

void MainGame::LoadResources(ID3D10Device* pDXDevice, PhysX* pPhysXEngine)
{
    Content->Init(pDXDevice);

	// CAMERA
	m_pEditorCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
									static_cast<int>(BLOX_2D->GetWindowSize().height),
									true	);
    m_pEditorCamera->LookAt(Vector3(-500, 300, -500), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pEditorCamera->SetLens(BLOX_2D->GetWindowSize().width/BLOX_2D->GetWindowSize().height,PiOver4,0.1f,10000.0f);

	m_pTrackingCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
									static_cast<int>(BLOX_2D->GetWindowSize().height),
									false	);
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
	m_pRenderContext = new RenderContext(m_pEditorCamera, m_pLightController);

    SpotLight sl;
        //Omni 1
        sl = SpotLight();
        sl.position = Vector3(0.0f,600.0f,0.0f);
        sl.color = Color(0.8f, 0.8f, 0.5f, 1.0f);
        sl.multiplier = 1.0f;
		sl.AttenuationStart = 0;
		sl.AttenuationEnd = 2000;
		sl.direction = Vector3(0, -1, 0);
		sl.power = 0.5f;
		sl.shadowsEnabled = false;
		sl.lightEnabled = true;
        m_pLightController->AddLight(sl);

	SpotLight sl2;
        //Omni 1
        sl2 = SpotLight();
        sl2.position = Vector3(800.0f,600.0f,0.0f);
        sl2.color = Color(0.9f, 0.9f, 0.8f, 1.0f);
        sl2.multiplier = 1.0f;
		sl2.AttenuationStart = 0;
		sl2.AttenuationEnd = 2000;
		sl2.direction = Vector3(0, -1, 0);
		sl2.power = 0.5f;
		sl2.shadowsEnabled = false;
		sl2.lightEnabled = true;
        m_pLightController->AddLight(sl2);

	SpotLight sl3;
        //Omni 1
        sl3 = SpotLight();
        sl3.position = Vector3(-800.0f,600.0f,0.0f);
        sl3.color = Color(0.9f, 0.9f, 0.8f, 1.0f);
        sl3.multiplier = 1.0f;
		sl3.AttenuationStart = 0;
		sl3.AttenuationEnd = 2000;
		sl3.direction = Vector3(0, -1, 0);
		sl3.power = 0.5f;
		sl3.shadowsEnabled = false;
		sl3.lightEnabled = true;
        m_pLightController->AddLight(sl3);

	PointLight pl;
	pl = PointLight();
	pl.position = Vector3(0.0f,100.0f,-400.0f);
	pl.lightEnabled = true;
	pl.color = Color(0.7f,0.7f,0.8f, 1.0f);
	pl.AttenuationEnd = 2000;
	pl.multiplier = 1.0f;
	m_pLightController->AddLight(pl);

		
	// PHYSX
	m_pPhysXEngine = pPhysXEngine;

	// LEVEL
	m_pLevel = new Level(pDXDevice);
	m_pLevel->Initialize(m_pPhysXEngine, m_pTrackingCamera);

	// AUDIO
	tstring projectLocation = tstring(_T("./Audio/Win/JelloMan"));
	m_pAudioEngine = new AudioEngine(projectLocation);
	m_pAudioEngine->Initialize();

	m_pTestSound = new Sound(m_pAudioEngine,_T("BackgroundMusic"),_T("BackGroundMusicVolume"));
	m_pTestSound->PreLoad();
	m_pTestSound->SetLoopCount(1);
	m_pTestSound->SetVolume(90);

	// GUI
	m_pEditorGUI = new EditorGUI(m_pPhysXEngine);
	m_pEditorGUI->Initialize();

	m_bResourcesLoaded = true;
}

void MainGame::UpdateScene(const float dTime)
{
	// dtime
	m_dTtime = dTime;

	if (m_pEditorGUI->GetMode() != EditorGUI::MODE_PLAY)
	{
		m_pLevel->TickCharacter(false);
		m_pEditorCamera->Tick(dTime);
	}
	else
		m_pLevel->TickCharacter(true);

	if (m_pEditorGUI->NewModelLoaded())
	{
		LevelObject* newLvlObj = m_pEditorGUI->GetNewLevelObject();
		newLvlObj->Init(m_pPhysXEngine);

		Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();
		vLook.Normalize();

		newLvlObj->Translate(m_pRenderContext->GetCamera()->GetPosition() + vLook * 100);

		m_pLevel->AddLevelObject(newLvlObj);
	}

	m_pAudioEngine->DoWork();
	m_pTestSound->Tick();

	if (m_pEditorGUI->GetMode() != EditorGUI::MODE_EDITOR)
	{	
		m_pPhysXEngine->FetchResults();
		m_pLevel->Tick(dTime);
		m_pPhysXEngine->Simulate(dTime);
	}

	if (m_pEditorGUI->GetShowGridButton()->IsActive())
		m_pLevel->ShowGrid(true);
	else
		m_pLevel->ShowGrid(false);

	/*if (CONTROLS->IsKeyPressed(VK_SPACE))
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
	
	if (CONTROLS->IsKeyPressed(VK_ADD))
	{
		//m_pTestSound->SetVolume(m_pTestSound->GetVolume() + 1);

		
	}
	else if (CONTROLS->IsKeyDown(VK_SUBTRACT))
	{
		m_pTestSound->SetVolume(m_pTestSound->GetVolume() - 1);
	}*/

	if (m_pEditorGUI->GetLightButton()->IsActive())
		m_pDeferredRenderer->SetLightMode(LIGHT_MODE_LIT);
	else
		m_pDeferredRenderer->SetLightMode(LIGHT_MODE_UNLIT);

	m_pEditorGUI->Tick(m_pRenderContext, m_pLevel->GetLevelObjects());
}

void MainGame::DrawScene()
{
	if (m_pEditorGUI->GetMode() == EditorGUI::MODE_PLAY)
		m_pRenderContext->SetCamera(m_pTrackingCamera);
	else
		m_pRenderContext->SetCamera(m_pEditorCamera);
		
	// --------------------------------------
	//			   RENDER SCENE
	// --------------------------------------

	// POST PROCESS
	//m_pPostProcessor->Begin();

	// START DEFERRED
	m_pDeferredRenderer->Begin();

	// DRAW
	m_pLevel->DrawDeferred(m_pRenderContext);

	// END DEFERRED
	m_pDeferredRenderer->End(m_pRenderContext);

	// START FORWARD
	m_pForwardRenderer->Begin(m_pDeferredRenderer);

	// DRAW
	m_pLevel->DrawForward(m_pRenderContext);

	// END FORWARD
	m_pForwardRenderer->End();

	// POST PROCESS
	//m_pPostProcessor->End();
		
	// --------------------------------------

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