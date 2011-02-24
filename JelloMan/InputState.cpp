#include "StdAfx.h"
#include "InputState.h"

InputState::InputState(
	bool bLMBPressed, 
	bool bRMBPressed, 
	int iWheelPos, 
	POINTS mouseDelta, 
	POINTS mousePos,
	byte *pKeys,
	float dTime) 
:m_bLMBPressed(bLMBPressed)
,m_bRMBPressed(bRMBPressed)
,m_WheelPos(iWheelPos)
,m_MouseDeltaPos(mouseDelta)
,m_MousePos(mousePos)
,m_DTime(dTime)
{
	memcpy_s(m_Keys, NUMKEYS, pKeys, NUMKEYS);
}

InputState::~InputState(void)
{

}

bool InputState::IsKeyDown(int vKey) const
{
	if(m_Keys[vKey] & 0x80)return true;
	return false;
}

POINTS InputState::GetMouseMovement() const
{
	return m_MouseDeltaPos;
}

POINTS InputState::GetMousePos() const
{
	return m_MousePos;
}

short  InputState::GetWheelPos() const
{
	return m_WheelPos;
}

InputState& InputState::operator=(const InputState& g)
{
	m_bLMBPressed=g.m_bLMBPressed;
	m_bRMBPressed=g.m_bRMBPressed;
	m_WheelPos=g.m_WheelPos;
	m_MouseDeltaPos=g.m_MouseDeltaPos;
	m_MousePos=g.m_MousePos;
	//deep copy keys
	memcpy_s(m_Keys,NUMKEYS, g.m_Keys,NUMKEYS);
	m_DTime = g.m_DTime;

	return *this;
}

InputState::InputState(const InputState& g)
{
	m_bLMBPressed=g.m_bLMBPressed;
	m_bRMBPressed=g.m_bRMBPressed;
	m_WheelPos=g.m_WheelPos;
	m_MouseDeltaPos=g.m_MouseDeltaPos;
	m_MousePos=g.m_MousePos;
	memcpy_s(m_Keys,NUMKEYS, g.m_Keys,NUMKEYS);
	m_DTime = g.m_DTime;
}