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
							m_OldMousePos(Point2F(0,0))
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
KeyboardState Controls::Keyboard()
{
	// getstatus of keys
	BOOL b = GetKeyboardState(m_Keys);

	bool bKeysDown[NUMKEYS];
	bool bKeysPressed[NUMKEYS];

	for (int i = 0; i < NUMKEYS; ++i)
	{
		if (m_Keys[i] & 0x80)
			bKeysDown[i] = true;
		else
			bKeysDown[i] = false;

		bKeysPressed[i] = IsKeyPressed(i);
	}

	// create state object on stack
	KeyboardState keyboardState(bKeysDown,bKeysPressed);

	return keyboardState;
}

bool Controls::IsKeyPressed(int vKey)
{
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

MouseState Controls::Mouse()
{
	// calc mousemovement
	Point2D mouseDeltaPos;
	mouseDeltaPos.x = m_MousePos.x - m_OldMousePos.x;
	mouseDeltaPos.y = m_MousePos.y - m_OldMousePos.y;
	m_OldMousePos = m_MousePos;

	// create state object on stack
	MouseState mouseState(	m_bLMBClicked,
							m_bRMBClicked,
							m_bLMBDown, 
							m_bRMBDown,
							m_MouseWheelPos,
							m_MousePos,
							mouseDeltaPos);

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