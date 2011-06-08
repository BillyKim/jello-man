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

	enum KEYBOARD_LAYOUT
	{
		KEYBOARD_LAYOUT_AZERTY = 0,
		KEYBOARD_LAYOUT_QWERTY = 1
	};

	GameConfig();				// Constructor
	virtual ~GameConfig();		// Destructor

	// SETTERS
	void SetTitle(const tstring& titleRef);
	void SetWindowWidth(int width = 800) {m_WindowWidth = width;}
	void SetWindowHeight(int height = 600) {m_Windowheight = height;}
	void SetBlox2DAntiAliasing(bool antiAliasing = true) {m_b2DAntiAliasing = antiAliasing;}
	void SetKeyboardLayout(KEYBOARD_LAYOUT layout) { m_KeyboardLayout = layout; }
	void UsePhysX(bool bUsePhysX) { m_bUsePhysX = bUsePhysX; }

	// GETTERS
	const tstring* GetGameTitle() {return m_pGameTitle;}
	Size2D GetWindowSize();
	bool Blox2DAntiAliasing() { return m_b2DAntiAliasing; }
	KEYBOARD_LAYOUT GetKeyboardLayout() { return m_KeyboardLayout; }
	bool UsingPhysX() { return m_bUsePhysX; }

private: 
	
	tstring* m_pGameTitle;
	int m_WindowWidth, m_Windowheight;
	bool m_b2DAntiAliasing;
	bool m_bUsePhysX;

	KEYBOARD_LAYOUT m_KeyboardLayout;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameConfig(const GameConfig& yRef);									
	GameConfig& operator=(const GameConfig& yRef);	
};

 
