//=============================================================================
// MainGame by Bastian Damman & Sebastiaan Sprengers
//=============================================================================
#include "MainGame.h"
#include "ContentManager.h"
#include "AudioEngine.h"
#include "xact3.h"

MainGame::MainGame()	:	m_dTtime(0),
							m_pLevel(0),
							m_pCamera(0),
                            m_pLightController(0),
							m_pAudioEngine(0),
							m_pTestSound(0),
							m_bResourcesLoaded(false),
							m_bDebug(false),
							m_pEditorGUI(0),
							m_Angle(0)
{

}

MainGame::~MainGame()
{
	delete m_pCamera;
    delete m_pLightController;
	delete m_pAudioEngine;
	delete m_pTestSound;
	delete m_pEditorGUI;

	SafeDelete(m_pLevel);
}

void MainGame::Initialize(GameConfig& refGameConfig)
{
	// TEST - gameconfig word nog file!
	refGameConfig.SetTitle(_T("Happy Engine"));
	refGameConfig.SetWindowWidth(1440);
	refGameConfig.SetWindowHeight(800);
	refGameConfig.SetBlox2DAntiAliasing(true);
}

void MainGame::LoadResources(ID3D10Device* pDXDevice)
{
    Content->Init(pDXDevice);

	// CAMERA
	m_pCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
							static_cast<int>(BLOX_2D->GetWindowSize().height)	);
    m_pCamera->LookAt(Vector3(-225, 115, -205), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pCamera->SetLens(BLOX_2D->GetWindowSize().width/BLOX_2D->GetWindowSize().height,PiOver4,10.0f,10000.0f);

    // LIGHTCONTROLLER
    m_pLightController = new LightController();

    PointLight pl;
        //Omni 1
        pl = PointLight();
        pl.position = Vector3(0.0f,100.0f,0.0f);
        pl.color = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
        pl.multiplier = 1.5f;
		pl.AttenuationStart = 0;
		pl.AttenuationEnd = 1000;
        m_pLightController->AddLight(pl);

	// LEVEL
	m_pLevel = new Level(pDXDevice);
	m_pLevel->Initialize();
	m_bResourcesLoaded = true;

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
}

void MainGame::UpdateScene(const float dTime)
{
	// dtime
	m_dTtime = dTime;

	if (m_bResourcesLoaded)
	{
		m_pCamera->Tick(dTime);
		m_pLevel->Tick(dTime);

		m_pAudioEngine->DoWork();
		m_pTestSound->Tick();

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
			m_pLevel->SetLightMode(LIGHT_MODE_LIT);
		else
			m_pLevel->SetLightMode(LIGHT_MODE_UNLIT);
	}
}

void MainGame::DrawScene()
{
	if (m_bResourcesLoaded)
	{
		RenderContext renderContext(m_pCamera, m_pLightController);
		m_pLevel->Draw(&renderContext);

		m_pEditorGUI->Tick(&renderContext);
		m_pEditorGUI->Draw();

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->SetFont(_T("Arial"),true,false,12);

		CONTROLS->ResetMouse();
	}
	else
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
}

void MainGame::OnResize(ID3D10RenderTargetView* pRTView)
{
	if (m_bResourcesLoaded)
		m_pLevel->OnResize(pRTView);
}
void MainGame::Release()
{
	if (m_bResourcesLoaded)
		m_pLevel->Release();
}