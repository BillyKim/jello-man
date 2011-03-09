//---------------------------
// Includes
//---------------------------
#include "KeyboardState.h"

//---------------------------
// Constructor & Destructor
//---------------------------
KeyboardState::KeyboardState(bool bDownKeys[256], bool bPressedKeys[256])
{
	for (int i = 0; i < NUMKEYS; ++i)
	{
		m_bKeysDown[i] = bDownKeys[i];
		m_bKeysPressed[i] = bPressedKeys[i];
	}
}

KeyboardState::~KeyboardState(void)
{
}

// assignment operator & copy constructor
KeyboardState& KeyboardState::operator=(const KeyboardState& ref)
{
	for (int i = 0; i < NUMKEYS; ++i)
	{
		m_bKeysDown[i] = ref.m_bKeysDown[i];
		m_bKeysPressed[i] = ref.m_bKeysPressed[i];
	}

	return *this;
}
KeyboardState::KeyboardState(const KeyboardState& ref)
{
	for (int i = 0; i < NUMKEYS; ++i)
	{
		m_bKeysDown[i] = ref.m_bKeysDown[i];
		m_bKeysPressed[i] = ref.m_bKeysPressed[i];
	}
}

// GETTERS
bool KeyboardState::IsKeyDown(int vKey) const
{
	return m_bKeysDown[vKey];
}

bool KeyboardState::IsKeyPressed(int vKey) const
{
	return m_bKeysPressed[vKey];
}
