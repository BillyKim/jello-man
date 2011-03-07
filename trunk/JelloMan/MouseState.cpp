//---------------------------
// Includes
//---------------------------
#include "MouseState.h"

//---------------------------
// Constructor & Destructor
//---------------------------
MouseState::MouseState(	bool bLMBClicked,
						bool bRMBClicked,
						bool bLMBDown,
						bool bRMBDown,
						int iMWPos,
						Point2D mousePos
						):
						m_bLMBClicked(bLMBClicked),
						m_bRMBClicked(bRMBClicked),
						m_bLMBDown(bLMBDown),
						m_bRMBDown(bRMBDown),
						m_MouseWheelPos(iMWPos),
						m_MousePos(mousePos)
{
}

MouseState::~MouseState(void)
{
}

// assignment operator & copy constructor
MouseState& MouseState::operator=(const MouseState& ref)
{
	m_bLMBClicked = ref.m_bLMBClicked;
	m_bRMBClicked = ref.m_bRMBClicked;
	m_bLMBDown = ref.m_bLMBDown;
	m_bRMBDown = ref.m_bRMBDown;
	m_MouseWheelPos = ref.m_MouseWheelPos;
	m_MousePos = ref.m_MousePos;

	return *this;
}
MouseState::MouseState(const MouseState& ref)
{
	m_bLMBClicked = ref.m_bLMBClicked;
	m_bRMBClicked = ref.m_bRMBClicked;
	m_bLMBDown = ref.m_bLMBDown;
	m_bRMBDown = ref.m_bRMBDown;
	m_MouseWheelPos = ref.m_MouseWheelPos;
	m_MousePos = ref.m_MousePos;
}
