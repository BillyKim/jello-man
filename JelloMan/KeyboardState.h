#pragma once

#include "D3DUtil.h"

//-----------------------------------------------------
// KeyboardState Class									
//-----------------------------------------------------
class KeyboardState
{
public:

	// constructor & destructor
	KeyboardState(unsigned char* pKeys);
	virtual ~KeyboardState(void);

	// assignment operator & copy constructor
	KeyboardState& operator=(const KeyboardState& g);
	KeyboardState(const KeyboardState& g);

	// GETTERS
	bool isKeyDown(int vKey) const;
	//bool isKeyPressed(int vKey);

	// CONSTANTS
	static const int NUMKEYS = 256;

private: 
	unsigned char m_Keys[NUMKEYS];
	bool m_bKeysPressed[NUMKEYS];
};

 
