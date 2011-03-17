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
							m_bResourcesLoaded(false)
{

}

MainGame::~MainGame()
{
	delete m_pCamera;
    delete m_pLightController;
	delete m_pAudioEngine;
	delete m_pTestSound;

	SafeDelete(m_pLevel);
}

void MainGame::Initialize(GameConfig& refGameConfig)
{
	// TEST - gameconfig word nog file!
	refGameConfig.SetTitle(_T("Happy Engine"));
	refGameConfig.SetWindowWidth(1280);
	refGameConfig.SetWindowHeight(720);
	refGameConfig.SetBlox2DAntiAliasing(true);
}

void MainGame::LoadResources(ID3D10Device* pDXDevice)
{
    Content->Init(pDXDevice);

	// CAMERA
	m_pCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
							static_cast<int>(BLOX_2D->GetWindowSize().height)	);
    m_pCamera->LookAt(Vector3(-225, 115, -205), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pCamera->SetLens(BLOX_2D->GetWindowSize().width/BLOX_2D->GetWindowSize().height);

    // LIGHTCONTROLLER
    m_pLightController = new LightController();

    PointLight pl;
        //Omni 1
        pl = PointLight();
        pl.position = Vector3(109.888f,0.0f,-80.406f);
        pl.color = Vector4(240/255.f, 203/255.f, 104/255.f, 1);
        pl.multiplier = 4.0f;
        m_pLightController->AddLight(pl);

        //Omni 2
        pl = PointLight();
        pl.position = Vector3(-104.53f,0.0f,317.178f);
        pl.color = Vector4(255/255.f, 54/255.f, 54/255.f, 1);
        pl.multiplier = 2.0f;
        m_pLightController->AddLight(pl);

  //      //Omni 3
  //      pl = PointLight();
  //      pl.position = Vector3(-36.82f, 13.95f, 61.24f);
  //      pl.color = Vector4(85/255.f, 151/255.f, 249/255.f, 1);
  //      pl.multiplier = 2.0f;
		//m_pLightController->AddLight(pl);
		//
		//	m_LightPosvec.push_back(D3DXVECTOR3(-36.82f, 13.95f, 61.24f));
		//	m_LightWorldVec.push_back(Matrix());
		//	D3DXMatrixTranslation(&m_LightWorldVec[2],-36.82f, 13.95f, 61.24f);

  //      //Omni 4
  //      pl = PointLight();
  //      pl.position = Vector3(154.73f, 13.95f, 207.77f);
  //      pl.color = Vector4(100/255.f, 253/255.f, 100/255.f, 1);
  //      pl.multiplier = 10.0f;
  //      m_pLightController->AddLight(pl);
		//
		//	m_LightPosvec.push_back(D3DXVECTOR3(154.73f, 13.95f, 207.77f));
		//	m_LightWorldVec.push_back(Matrix());
		//	D3DXMatrixTranslation(&m_LightWorldVec[3],154.73f, 13.95f, 207.77f);
  //    
		////Omni 5
  //      pl = PointLight();
  //      pl.position = Vector3(154.73f, -43.72f, 68.5f);
  //      pl.color = Vector4(249/255.f, 69/255.f, 141/255.f, 1);
  //      pl.multiplier = 10.0f;
  //      m_pLightController->AddLight(pl);
		//
		//	m_LightPosvec.push_back(D3DXVECTOR3(154.73f, -43.72f, 68.5f));
		//	m_LightWorldVec.push_back(Matrix());
		//	D3DXMatrixTranslation(&m_LightWorldVec[4],154.73f, -43.72f, 68.5f);

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
	}
}

void MainGame::DrawScene()
{
	if (m_bResourcesLoaded)
	{
		RenderContext renderContext(m_pCamera, m_pLightController);
		m_pLevel->Draw(&renderContext);
		m_pLightController->VisualLightDebugger(m_pCamera);

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->SetFont(_T("Arial"),true,false,16);

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
			BLOX_2D->DrawStringCentered(_T("PRESS SPACE"),0,-300);
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
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawStringCentered(_T("Loading Resources..."));
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