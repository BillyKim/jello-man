#pragma once

#include "D3DUtil.h"

//-----------------------------------------------------
// MouseState Class									
//-----------------------------------------------------
class MouseState
{
public:

	// constructor & destructor
	MouseState(bool bLMBClicked, bool bRMBClicked, bool bLMBDown, bool bRBMDown, int iMWPos, Point2D mousePos, Point2D mouseMovement);
	virtual ~MouseState(void);

	// assignment operator & copy constructor
	MouseState& operator=(const MouseState& g);
	MouseState(const MouseState& g);

	// GETTERS
	Point2D GetMousePos() const {return m_MousePos;}
	short GetMouseWheelPos() const {return m_MouseWheelPos;}
	bool LeftMBClicked() const {return m_bLMBClicked;}
	bool RightMBClicked() const {return m_bRMBClicked;}
	bool LeftMBDown() const {return m_bLMBDown;}
	bool RightMBDown() const {return m_bRMBDown;}
	Point2D GetMouseMovement() const;

private: 
	bool m_bLMBClicked, m_bRMBClicked;
	bool m_bLMBDown, m_bRMBDown;
	short m_MouseWheelPos;
	Point2D m_MousePos;
	Point2D m_MouseDeltaPos;
};

 
