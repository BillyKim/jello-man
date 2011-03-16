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
							m_bTest(false)
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
	refGameConfig.SetWindowWidth(800);
	refGameConfig.SetWindowHeight(600);
	refGameConfig.SetBlox2DAntiAliasing(true);
}

void MainGame::LoadResources(ID3D10Device* pDXDevice)
{
    Content->Init(pDXDevice);

	// CAMERA
	m_pCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
							static_cast<int>(BLOX_2D->GetWindowSize().height)	);
    m_pCamera->LookAt(Vector3(-225, 115, -205), Vector3(0, 0, 0), Vector3(0, 1, 0));
    m_pCamera->SetLens();

    // LIGHTCONTROLLER
    m_pLightController = new LightController();

    PointLight pl;
        //Omni 1
        pl = PointLight();
        pl.position = Vector3(-62.07f, 46.88f, -112.47f);
        pl.color = Vector4(240/255.f, 203/255.f, 104/255.f, 1);
        pl.multiplier = 4.0f;
        m_pLightController->AddLight(pl);
        //Omni 2
        pl = PointLight();
        pl.position = Vector3(-14.64f, 63.71f, -45.00f);
        pl.color = Vector4(255/255.f, 54/255.f, 54/255.f, 1);
        pl.multiplier = 2.0f;
        m_pLightController->AddLight(pl);
        //Omni 3
        pl = PointLight();
        pl.position = Vector3(-36.82f, 13.95f, 61.24f);
        pl.color = Vector4(85/255.f, 151/255.f, 249/255.f, 1);
        pl.multiplier = 2.0f;
        m_pLightController->AddLight(pl);
        //Omni 4
        pl = PointLight();
        pl.position = Vector3(154.73f, 13.95f, 207.77f);
        pl.color = Vector4(100/255.f, 253/255.f, 100/255.f, 1);
        pl.multiplier = 10.0f;
        m_pLightController->AddLight(pl);
        //Omni 5
        pl = PointLight();
        pl.position = Vector3(154.73f, -43.72f, 68.5f);
        pl.color = Vector4(249/255.f, 69/255.f, 141/255.f, 1);
        pl.multiplier = 10.0f;
        m_pLightController->AddLight(pl);

	// LEVEL
	m_pLevel = new Level(pDXDevice);
	m_pLevel->Initialize();
	m_bTest = true;

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

void MainGame::DrawScene()
{
	RenderContext renderContext(m_pCamera, m_pLightController);
	m_pLevel->Draw(&renderContext);

	tstringstream stream;
	stream << _T("look - ");
	stream << _T("X: ") << m_pCamera->GetLook().X << _T(" ");
	stream << _T("Y: ") << m_pCamera->GetLook().Y << _T(" ");
	stream << _T("Z: ") << m_pCamera->GetLook().Z << _T(" ");

	BLOX_2D->DrawString(stream.str(),400,20);

	tstringstream stream2;
	stream2 << _T("pos - ");
	stream2 << _T("X: ") << m_pCamera->GetPosition().X << _T(" ");
	stream2 << _T("Y: ") << m_pCamera->GetPosition().Y << _T(" ");
	stream2 << _T("Z: ") << m_pCamera->GetPosition().Z << _T(" ");

	BLOX_2D->DrawString(stream2.str(),400,40);

	/* ---------------------------------------------- */
	/*					TEST STUFF					  */
	/* ---------------------------------------------- */

	// singleton gemaakt van Blox2D
	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

	// singleton gemaakt van Controls
	if (CONTROLS->IsKeyDown('X'))
	{
		BLOX_2D->SetColor(255,0,255);
		BLOX_2D->SetFont(_T("Arial"),true,false,150);
		BLOX_2D->SetTransform(Matrix3x2F::Rotation(90,Point2F(	BLOX_2D->GetWindowSize().width/2,
																BLOX_2D->GetWindowSize().height/2)));
		BLOX_2D->DrawStringCentered(_T(":D"));
		BLOX_2D->ResetTransform();
	}
	else
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->SetFont(_T("Arial"),true,false,24);
		BLOX_2D->DrawStringCentered(_T("PRESS X"));

		tstringstream stream;
		stream << _T("X: ") << CONTROLS->GetMousePos().x;
		stream << _T("  Y: ") << CONTROLS->GetMousePos().y;
		BLOX_2D->DrawStringCentered(stream.str(),0,50);
	}

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->SetFont(_T("Arial"),true,false,16);

	if (m_pTestSound->IsPlaying())
	{
		BLOX_2D->DrawStringCentered(m_pTestSound->GetSoundInfo(),0,-120);
	}
	else
	{
		BLOX_2D->DrawStringCentered(_T("PRESS SPACE"),0,-120);
	}

	/* ---------------------------------------------- */
}

void MainGame::OnResize(ID3D10RenderTargetView* pRTView)
{
	if (m_bTest)
		m_pLevel->OnResize(pRTView);
}
void MainGame::Release()
{
	if (m_bTest)
		m_pLevel->Release();
}
