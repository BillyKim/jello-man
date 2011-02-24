//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "InputStateManager.h"
#include "InputState.h"

//---------------------------
// Defines
//---------------------------
//#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
InputStateManager::InputStateManager()
:m_bLMBPressed(false)
,m_bRMBPressed(false)
,m_WheelPos(0)
{
	m_OldMousePos.x=0;
	m_OldMousePos.y=0;
	m_MousePos = m_OldMousePos;

	m_Timer.Reset();
}

InputStateManager::~InputStateManager()
{

}

//---------------------------
// Eigen methoden
//---------------------------
InputState InputStateManager::GenerateInputState()
{
	//calc changes since last visit
	POINTS mouseDeltaPos;
	mouseDeltaPos.x = m_MousePos.x - m_OldMousePos.x;
	mouseDeltaPos.y = m_MousePos.y - m_OldMousePos.y;
	m_OldMousePos = m_MousePos;

	//get status of keys
	BOOL b = GetKeyboardState(m_Keys);

	//calc timediff since last tick
	m_Timer.Tick();
	//get the time diff
	float dTime = m_Timer.GetDeltaTime();
	//create state object on stack
	InputState inputState(m_bLMBPressed, m_bRMBPressed, m_WheelPos, mouseDeltaPos, m_MousePos, m_Keys, dTime);
	m_WheelPos =0;
	//return copy of input state object
	return inputState;
}

void InputStateManager::SetMousePos(POINTS pos)
{
	m_MousePos=pos;
}
void InputStateManager::SetLMousePressed(bool isPressed)
{
	m_bLMBPressed = isPressed;
}
void InputStateManager::SetRMousePressed(bool isPressed)
{
	m_bRMBPressed = isPressed;
}
void InputStateManager::SetWheelPos(short newWheelPos)
{
	m_WheelPos = newWheelPos;
}
void InputStateManager::Stop()
{
	m_Timer.Stop();
}
void InputStateManager::Start()
{
	m_Timer.Start();
}
float InputStateManager::GetGameTime()
{
	return m_Timer.GetGameTime();
}


