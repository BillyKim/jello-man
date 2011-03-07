//---------------------------
// Includes
//---------------------------
#include "Controls.h"

//---------------------------
// Constructor & Destructor
//---------------------------c
Controls::Controls()	:	m_bLMBClicked(false),
							m_bRMBClicked(false),
							m_bLMBDown(false),
							m_bRMBDown(false),
							m_MouseWheelPos(0),
							m_MousePos(Point2F(0,0))
{

}

Controls::~Controls()
{
}

// GETTERS
KeyboardState Controls::Keyboard()
{
	// getstatus of keys
	BOOL b = GetKeyboardState(m_Keys);

	// create state object on stack
	KeyboardState keyboardState(m_Keys);

	return keyboardState;
}
MouseState Controls::Mouse()
{
	// create state object on stack
	MouseState mouseState(m_bLMBClicked, m_bRMBClicked, m_bLMBDown, m_bRMBDown, m_MouseWheelPos, m_MousePos);

	// reset data
	m_MouseWheelPos = 0;
	m_bLMBClicked = m_bRMBClicked = false;
	m_bLMBDown = m_bRMBDown= false;

	return mouseState;
}

// SETTERS
void Controls::SetMousePos(Point2D pos)
{
	m_MousePos.x = pos.x;
	m_MousePos.y = pos.y;
}