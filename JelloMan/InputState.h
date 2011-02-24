#pragma once

class InputState
{
public:
	InputState(bool bLMBPressed, bool bRMBPressed, int iWheelPos, POINTS mouseDelta, POINTS mousePos,unsigned char *keys, float dTime);
	virtual ~InputState(void);
	
	// these getters return the state of mouse and keyboard
	// the status of input devices is updated once per frame 
	// in the UpdateScene method calling GetInputState()
	bool IsKeyDown(int vKey) const;
	POINTS GetMouseMovement() const;
	POINTS GetMousePos() const;
	short GetWheelPos() const;
	bool IsLeftMouseButtonDown() const{return m_bLMBPressed;}
	bool IsMiddleMouseButtonDown() const;
	bool IsRightMouseButtonDown() const{return m_bRMBPressed;}
	float GetDeltaTime() const{return m_DTime;}

	//implement assignment operator en copy constructor
	InputState& operator=(const InputState& g);
	InputState(const InputState& g);

	//Constants
	static const int NUMKEYS=256;

protected:
	//mouse pos
	bool m_bLMBPressed,m_bRMBPressed;
	short m_WheelPos;
	POINTS m_MousePos, m_MouseDeltaPos;
	unsigned char m_Keys[NUMKEYS];
	float m_DTime;
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, the declarations are deliberately made without implementation because they should never be used.
	// -------------------------
private:
	
};
