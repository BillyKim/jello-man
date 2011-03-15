#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"

#define CONTROLS (Controls::GetSingleton())

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
	bool IsKeyDown(int vKey);
	bool IsKeyPressed(int vKey);

	Point2D GetMousePos() const {return m_MousePos;}
	short GetMouseWheelPos();
	bool LeftMBClicked() const { return m_bLMBClicked; }
	bool RightMBClicked() const { return m_bRMBClicked; }
	bool LeftMBDown() const { return m_bLMBDown; }
	bool RightMBDown() const { return m_bRMBDown; }
	Point2D GetMouseMovement();

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
	Point2D m_OldMousePos;
	unsigned char m_Keys[NUMKEYS];
	bool m_bKeysPressed[NUMKEYS];

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Controls(const Controls& yRef);									
	Controls& operator=(const Controls& yRef);	
};

 
