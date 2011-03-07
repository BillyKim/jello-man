//---------------------------
// Includes
//---------------------------
#include "KeyboardState.h"

//---------------------------
// Constructor & Destructor
//---------------------------
KeyboardState::KeyboardState(byte* pKeys)
{
	memcpy_s(m_Keys, NUMKEYS, pKeys, NUMKEYS);

	for(int i = 0; i < NUMKEYS; ++i)
	{
		m_bKeysPressed[i] = false;
	}
}

KeyboardState::~KeyboardState(void)
{
}

// assignment operator & copy constructor
KeyboardState& KeyboardState::operator=(const KeyboardState& ref)
{
	//deep copy keys
	memcpy_s(m_Keys,NUMKEYS,ref.m_Keys,NUMKEYS);

	return *this;
}
KeyboardState::KeyboardState(const KeyboardState& ref)
{
	//deep copy keys
	memcpy_s(m_Keys,NUMKEYS,ref.m_Keys,NUMKEYS);
}

// GETTERS
bool KeyboardState::isKeyDown(int vKey) const
{
	if (m_Keys[vKey] & 0x80) return true;
	return false;
}

/*bool KeyboardState::isKeyPressed(int vKey)
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
}*/
