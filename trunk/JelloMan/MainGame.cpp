//=============================================================================
// MainGame GP1 by Bart Uyttenhove
//=============================================================================
#include "MainGame.h"

#include "InputState.h"
#include "GameConfig.h"
#include "Blox2D.h"


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

void MainGame::UpdateScene(const InputState & refInputState)
{
	m_dTtime = refInputState.GetDeltaTime();
}

void MainGame::DrawScene(Blox2D& refBlox2D)
{
	// TEST - van blox 2d gaat nog singleton worden gemaakt!
	refBlox2D.SetColor(255,255,255);
	refBlox2D.ShowFPS(m_dTtime, true);
}
