#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"

//-----------------------------------------------------
// GameConfig Class									
//-----------------------------------------------------
class GameConfig
{
public:
	GameConfig();				// Constructor
	virtual ~GameConfig();		// Destructor

	// SETTERS
	void SetTitle(const tstring& titleRef);
	void SetWindowWidth(int width = 800) {m_WindowWidth = width;}
	void SetWindowHeight(int height = 600) {m_Windowheight = height;}
	//void Resizable(bool resizable = false) {m_bResizable = resizable;}
	//void SetIcon(WORD wIcon) {m_wIcon = wIcon;}
	void SetBlox2DAntiAliasing(bool antiAliasing = true) {m_b2DAntiAliasing = antiAliasing;}

	// GETTERS
	const tstring* GetGameTitle() {return m_pGameTitle;}
	Size2D GetWindowSize();
	//bool isWindowResizable() {return m_bResizable;}
	//WORD GetIcon() {return m_wIcon;}
	bool Blox2DAntiAliasing() { return m_b2DAntiAliasing; }

private: 
	
	tstring* m_pGameTitle;
	int m_WindowWidth, m_Windowheight;
	//bool m_bResizable;
	bool m_b2DAntiAliasing;
	//WORD m_wIcon;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameConfig(const GameConfig& yRef);									
	GameConfig& operator=(const GameConfig& yRef);	
};

 
