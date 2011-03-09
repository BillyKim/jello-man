//=============================================================================
// MainGame GP1 by Bart Uyttenhove
//=============================================================================
#include "MainGame.h"

#include "GameConfig.h"

// CONTROLS & BLOX2D SINGLETON
#define BLOX_2D (Blox2D::GetSingleton())
#define CONTROLS (Controls::GetSingleton())

MainGame::MainGame()	:	m_dTtime(0)
{

}

MainGame::~MainGame()
{
	
}

void MainGame::Initialize(GameConfig& refGameConfig)
{
	// TEST - gameconfig word nog file!
	refGameConfig.SetTitle(_T("Happy Engine"));
	refGameConfig.SetWindowWidth(800);
	refGameConfig.SetWindowHeight(600);
}

void MainGame::UpdateScene(const float dTime)
{
	// dtime
	m_dTtime = dTime;
}

void MainGame::DrawScene()
{
	// singleton gemaakt van Blox2D
	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

	// singleton gemaakt van Controls
	if (CONTROLS->Keyboard().isKeyDown('X'))
	{
		BLOX_2D->SetColor(255,0,255);
		BLOX_2D->SetFont(_T("Arial"),true,false,150);
		BLOX_2D->SetTransform(Matrix3x2F::Rotation(90,Point2F(BLOX_2D->GetWindowSize().width/2,BLOX_2D->GetWindowSize().height/2)));
		BLOX_2D->DrawStringCentered(_T(":D"));
		BLOX_2D->ResetTransform();
	}
	else
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->SetFont(_T("Arial"),true,false,24);
		BLOX_2D->DrawStringCentered(_T("PRESS X"));
	}
}
