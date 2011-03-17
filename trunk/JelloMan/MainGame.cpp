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
							m_bmpCamera(0)
{

}

MainGame::~MainGame()
{
	delete m_pCamera;
    delete m_pLightController;
	delete m_pAudioEngine;
	delete m_pTestSound;
	delete m_bmpCamera;

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

	m_bmpCamera = new Bitmap(_T("Content/Images/cam.png"));

    PointLight pl;
        //Omni 1
        pl = PointLight();
        pl.position = Vector3(109.888f,50.0f,-80.406f);
        pl.color = Vector4(240/255.f, 203/255.f, 104/255.f, 1);
        pl.multiplier = 1.0f;
		pl.AttenuationStart = 0;
		pl.AttenuationEnd = 1000;
        m_pLightController->AddLight(pl);

  //      //Omni 2
  //      pl = PointLight();
  //      pl.position = Vector3(-104.53f,0.0f,317.178f);
  //      pl.color = Vector4(255/255.f, 54/255.f, 54/255.f, 1);
  //      pl.multiplier = 1.0f;
		//pl.AttenuationStart = 0;
		//pl.AttenuationEnd = 500;
  //      m_pLightController->AddLight(pl);

  //      //Omni 3
  //      pl = PointLight();
  //      pl.position = Vector3(300.f, 200.95f, -300.24f);
  //      pl.color = Vector4(85/255.f, 151/255.f, 249/255.f, 1);
  //      pl.multiplier = 2.0f;
		//pl.AttenuationStart = 10;
		//pl.AttenuationEnd = 400;
		//m_pLightController->AddLight(pl);

  //      //Omni 4
  //      pl = PointLight();
  //      pl.position = Vector3(154.73f, 13.95f, 207.77f);
  //      pl.color = Vector4(100/255.f, 253/255.f, 100/255.f, 1);
  //      pl.multiplier = 1.0f;
		//pl.AttenuationStart = 0;
		//pl.AttenuationEnd = 800;
  //      m_pLightController->AddLight(pl);
  //    
		////Omni 5
  //      pl = PointLight();
  //      pl.position = Vector3(154.73f, -43.72f, 68.5f);
  //      pl.color = Vector4(249/255.f, 69/255.f, 141/255.f, 1);
  //      pl.multiplier = 1.0f;
		//pl.AttenuationStart = 0;
		//pl.AttenuationEnd = 800;
  //      m_pLightController->AddLight(pl);

		////Omni 6
  //      pl = PointLight();
  //      pl.position = Vector3(0.f, 300.0f, 0.f);
  //      pl.color = Vector4(128/255.f, 128/255.f, 250/255.f, 1);
  //      pl.multiplier = 0.3f;
		//pl.AttenuationStart = 0;
		//pl.AttenuationEnd = 500;
  //      m_pLightController->AddLight(pl);

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

		if (CONTROLS->IsKeyPressed('L'))
		{
			m_bDebug = !m_bDebug;
		}

		if (CONTROLS->IsKeyPressed(VK_RETURN))
		{
			Vector3 look = m_pCamera->GetLook();
			look.Normalize();

			PointLight pl;
			pl = PointLight();
			pl.position = (m_pCamera->GetPosition() + look*200);

			int r = rand() % 255;
			int g = rand() % 255;
			int b = rand() % 255;

			pl.color = Vector4(r/255.f, g/255.f, b/255.f, 1);
			pl.multiplier = 1.0f;
			pl.AttenuationStart = 0;
			pl.AttenuationEnd = 200;
			m_pLightController->AddLight(pl);

			cout << "Added pointlight";
		}
	}
}

void MainGame::DrawScene()
{
	if (m_bResourcesLoaded)
	{
		RenderContext renderContext(m_pCamera, m_pLightController);
		m_pLevel->Draw(&renderContext);

		if (m_bDebug)
			m_pLightController->VisualLightDebugger(m_pCamera);

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->SetFont(_T("Arial"),true,false,12);

		if (m_pTestSound->IsPlaying())
		{
			BLOX_2D->DrawString(	m_pTestSound->GetSoundInfo(),
									RectF(0,0,BLOX_2D->GetWindowSize().width,
									BLOX_2D->GetWindowSize().height),
									Blox2D::HORIZONTAL_ALIGN_RIGHT,
									Blox2D::VERTICAL_ALIGN_TOP);
		}
		else
		{
			BLOX_2D->DrawString(_T("PRESS SPACE"),2,130);
		}

		// if using camera
		if (CONTROLS->LeftMBDown())
		{
			BLOX_2D->DrawBitmap(m_bmpCamera,BLOX_2D->GetWindowSize().width-70,20,0.8f);
		}
	}
	else
	{
		BLOX_2D->SetColor(ColorF(ColorF::CornflowerBlue));
		BLOX_2D->FillBackGround();

		BLOX_2D->SetColor(ColorF(ColorF::LightGray));
		BLOX_2D->DrawGrid(3,RectF(0,0,BLOX_2D->GetWindowSize().width,
									  BLOX_2D->GetWindowSize().height));

		BLOX_2D->SetFont(_T("Arial"),true,false,30);
		BLOX_2D->SetColor(80,80,80);
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
