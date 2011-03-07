//=============================================================================
// MainGame GP1 by Bart Uyttenhove
//=============================================================================
#include "MainGame.h"

#include "GameConfig.h"
#include "Blox2D.h"


MainGame::MainGame()	:	m_dTtime(0),
							m_bDraw(false)
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

	/*if (refKeyboard.isKeyDown('X')) m_bDraw = true;
	else m_bDraw = false;*/
}

void MainGame::DrawScene(Blox2D& refBlox2D)
{
	// TEST - van blox 2d gaat nog singleton worden gemaakt!
	refBlox2D.SetColor(255,255,255);
	refBlox2D.ShowFPS(m_dTtime,true,0.5f);

	refBlox2D.SetColor(0,255,0);
	refBlox2D.SetFont(_T("Arial"),true,false,50);
	refBlox2D.DrawStringCentered(_T("IT WORKS!"));


}
