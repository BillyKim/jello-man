//=============================================================================
// MainGame GP1 by Bart Uyttenhove
//=============================================================================
#include "MainGame.h"

#include "GameConfig.h"

#define BLOX_2D (Blox2D::GetSingleton())

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

void MainGame::UpdateScene(const KeyboardState& refKeyboard, const MouseState& refMouse, const float dTime)
{
	// TEST - van keyboardstate & mousestate word nog singleton gemaakt
	m_dTtime = dTime;
}

void MainGame::DrawScene()
{
	// singleton gemaakt van Blox2D

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->ShowFPS(m_dTtime,true,0.5f);

	BLOX_2D->SetColor(255,0,255);
	BLOX_2D->SetFont(_T("Arial"),true,false,50);
	BLOX_2D->DrawStringCentered(_T("HAPPY ENGINE :D"));
}
