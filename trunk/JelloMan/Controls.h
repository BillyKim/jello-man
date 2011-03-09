#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include "MouseState.h"
#include "KeyboardState.h"

//-----------------------------------------------------
// Controls Class									
//-----------------------------------------------------
class Controls
{
private:
	// singleton implementation
	Controls();	// Constructor
	static Controls* m_pSingleton;

public:
	virtual ~Controls();		// Destructor
	static Controls* GetSingleton();

	// GETTERS
	KeyboardState Keyboard();
	MouseState Mouse();

	// SETTERS
	void SetMousePos(Point2D pos);
	void SetLeftMBClicked(bool clicked) {m_bLMBClicked = clicked;}
	void SetRightMBClicked(bool clicked) {m_bRMBClicked = clicked;}
	void SetLeftMBDown(bool down) {m_bLMBDown = down;}
	void SetRightMBDown(bool down) {m_bRMBDown = down;}
	void SetMouseWheelPos(short pos) {m_MouseWheelPos = pos;}

	// CONSTANTS
	static const int NUMKEYS = 256;

private:
	bool m_bLMBClicked, m_bRMBClicked;
	bool m_bLMBDown, m_bRMBDown;
	short m_MouseWheelPos;
	Point2D m_MousePos;
	unsigned char m_Keys[NUMKEYS];

	//MouseState* m_MouseState;
	//KeyboardState* m_KeyboardState;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Controls(const Controls& yRef);									
	Controls& operator=(const Controls& yRef);	
};

 
