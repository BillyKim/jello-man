//---------------------------
// Includes
//---------------------------
#include "Controls.h"

//-----------------------------------------------------------------
// Static Variable Initialization
//-----------------------------------------------------------------
Controls* Controls::m_pSingleton = NULL;

//---------------------------
// Constructor & Destructor
//---------------------------
Controls::Controls()	:	m_bLMBClicked(false),
							m_bRMBClicked(false),
							m_bLMBDown(false),
							m_bRMBDown(false),
							m_MouseWheelPos(0),
							m_MousePos(Point2F(0,0)),
							m_OldMousePos(Point2F(400,300)),
							m_bLC(false),
							m_bRC(false)
{
	for(int i = 0; i < NUMKEYS; ++i)
	{
		m_bKeysPressed[i] = false;
	}
}

Controls::~Controls()
{
	m_pSingleton = 0;
}

// STATIC METHOD
Controls* Controls::GetSingleton()
{
	if (m_pSingleton == NULL) m_pSingleton = new Controls();
	return m_pSingleton;
}

// GETTERS
bool Controls::IsKeyPressed(int vKey)
{
	// getstatus of keys
	BOOL b = GetKeyboardState(m_Keys);

	if (m_Keys[vKey] & 0x80)
	{
		if (m_bKeysPressed[vKey] == false)
		{	
			m_bKeysPressed[vKey] = true;
			return true;
		}
		else return false;
	}
	else
	{
		m_bKeysPressed[vKey] = false;
		return false;
	}
}

bool Controls::IsKeyDown(int vKey)
{
	// getstatus of keys
	BOOL b = GetKeyboardState(m_Keys);

	if (m_Keys[vKey] & 0x80)
		return true;
	else
		return false;
}

Point2D Controls::GetMouseMovement()
{
	// calc mousemovement
	Point2D mouseDeltaPos;
	mouseDeltaPos.x = m_MousePos.x - m_OldMousePos.x;
	mouseDeltaPos.y = m_MousePos.y - m_OldMousePos.y;
	m_OldMousePos = m_MousePos;

	return mouseDeltaPos;
}

short Controls::GetMouseWheelPos()
{
	int mwPos = m_MouseWheelPos;
	m_MouseWheelPos = 0;

	return mwPos;
}

bool Controls::LeftMBUp()
{
	return m_bLMBClicked;
}

bool Controls::RightMBUp()
{
	return m_bRMBClicked;
}

// SETTERS
void Controls::SetMousePos(Point2D pos)
{
	m_MousePos.x = pos.x;
	m_MousePos.y = pos.y;
}
void Controls::SetOldMousePos(Point2D oldPos)
{
	m_OldMousePos.x = oldPos.x;
	m_OldMousePos.y = oldPos.y;
}
void Controls::SetLeftMBDown(bool down)
{
	m_bLMBDown = down;
	if (down == true)
	{
		m_bLC = true;
	}
}
void Controls::SetRightMBDown(bool down)
{
	m_bRMBDown = down;
	if (down == true)
	{
		m_bRC = true;
	}
}
void Controls::ResetMouse()
{
	if (m_bRC && m_bRMBClicked)
		m_bRC = false;
	if (m_bLC && m_bLMBClicked)
		m_bLC = false;
}