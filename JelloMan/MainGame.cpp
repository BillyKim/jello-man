//=============================================================================
// MainGame by Bastian Damman & Sebastiaan Sprengers
//=============================================================================
#include "MainGame.h"
#include "ContentManager.h"

MainGame::MainGame()	:	m_dTtime(0),
							m_pLevel(0),
							m_pCamera(0)
{

}

MainGame::~MainGame()
{
	delete m_pCamera;
	delete m_pLevel;
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

	m_pCamera = new Camera(	static_cast<int>(BLOX_2D->GetWindowSize().width),
							static_cast<int>(BLOX_2D->GetWindowSize().height)	);
	m_pCamera->LookAt(Vector3(4,4,-4),Vector3(0,2,0),Vector3(0,1,0));

	m_pLevel = new Level(pDXDevice);
	m_pLevel->Initialize();

	// TEST
	D3DXVECTOR3 lightDir(-.1f,-0.1f,-1);
	D3DXVec3Normalize(&m_DirLight.DirW,&lightDir);
	m_DirLight.Diffuse = m_DirLight.Ambient = m_DirLight.Specular = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
}

void MainGame::UpdateScene(const float dTime)
{
	// dtime
	m_dTtime = dTime;

	m_pCamera->Tick(dTime);
	m_pLevel->Tick(dTime);
}

void MainGame::DrawScene()
{
	RenderContext renderContext(m_pCamera, m_DirLight); // hoe lichten doorgeven, vector van alle lights?
	m_pLevel->Draw(&renderContext);


	/* ---------------------------------------------- */
	/*					TEST STUFF					  */
	/* ---------------------------------------------- */

	// singleton gemaakt van Blox2D
	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

	// singleton gemaakt van Controls
	if (CONTROLS->Keyboard().IsKeyDown('X'))
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
		stream << _T("X: ") << CONTROLS->Mouse().GetMousePos().x;
		stream << _T("  Y: ") << CONTROLS->Mouse().GetMousePos().y;
		BLOX_2D->DrawStringCentered(stream.str(),0,50);
	}

	/* ---------------------------------------------- */
}
