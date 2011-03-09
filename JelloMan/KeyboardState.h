#pragma once

#include "D3DUtil.h"

//-----------------------------------------------------
// KeyboardState Class									
//-----------------------------------------------------
class KeyboardState
{
public:

	// constructor & destructor
	KeyboardState(bool bDownKeys[256], bool bPressedKeys[256]);
	virtual ~KeyboardState(void);

	// assignment operator & copy constructor
	KeyboardState& operator=(const KeyboardState& g);
	KeyboardState(const KeyboardState& g);

	// GETTERS
	bool IsKeyDown(int vKey) const;
	bool IsKeyPressed(int vKey) const;

	// CONSTANTS
	static const int NUMKEYS = 256;

private: 
	bool m_bKeysDown[NUMKEYS];
	bool m_bKeysPressed[NUMKEYS];
};

 
