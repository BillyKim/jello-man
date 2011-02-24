#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "InputState.h"
#include "GameTimer.h"
//-----------------------------------------------------
// InputStateManager Class									
//-----------------------------------------------------
class InputStateManager
{
public:
	InputStateManager();				// Constructor
	virtual ~InputStateManager();		// Destructor


	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	// next setters are called by windows message loop proc
	void SetMousePos(POINTS pos);
	void SetLMousePressed(bool isPressed);
	void SetRMousePressed(bool isPressed);
	void SetWheelPos(short zDelta);

	//reads the status of keyboard and calcs the changed mouse coords
	InputState GenerateInputState();
	void Stop();
	void Start();
	float GetGameTime();
private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	//Timer object: keeps track of time
	GameTimer m_Timer;

	POINTS m_OldMousePos, m_MousePos;
	unsigned char m_Keys[256];
	bool m_bLMBPressed,m_bRMBPressed;
	short m_WheelPos;
	// Disabling default copy constructor and default 
	// assignment operator.
	InputStateManager(const InputStateManager& yRef);									
	InputStateManager& operator=(const InputStateManager& yRef);	
};

 
