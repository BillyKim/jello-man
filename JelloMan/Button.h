#pragma once

#include "D3DUtil.h"
#include <vector>
#include "Controls.h"
#include "GameTimer.h"
#include "Blox2DEngine.h"

//-----------------------------------------------------------------
// Button Class
//-----------------------------------------------------------------

class Button
{
public:

	enum BUTTON_STATE
	{
		STATE_NORMAL = 0,
		STATE_HOVER = 1,
		STATE_DOWN = 2,
		STATE_DEACTIVATED = 3,
		STATE_DEACTIVATED_HOVER = 4,
		STATE_DEACTIVATED_DOWN = 5
	};

	enum BUTTON_MODE
	{
		MODE_MOUSE = 0,
		MODE_MANUAL = 1
	};

	// constructor
	Button(int posX, int posY, int width, int height, bool bToggleable = false);

	// destructor
	virtual  ~Button();

	// general
	void Tick();
	void ShowHitRegion(bool fill = false);
	void Show();

	// setters
	void SetNormalState(Image* normalState);
	void SetHoverState(Image* hoverState);
	void SetDownState(Image* downState);
	void SetDeactivatedState(Image* deacivatedState);
	void SetDeactivatedStateHover(Image* deactivatedHover);
	void SetDeactivatedStateDown(Image* deactivatedDown);
	void SetState(BUTTON_STATE state);
	void SetMode(BUTTON_MODE mode);
	void SetPosition(int x, int y);
	void SetTransparency(float alpha)
	{m_Opacity = alpha;}
	void Deactivate();

	// getters
	bool Clicked() const;
	bool Hover() const
	{ return m_bHover; }
	bool Down() const
	{ return m_bDown; }
	int GetWith() const
	{return (int)m_Size.width;}
	int GetHeight() const
	{return (int)m_Size.height;}
	bool IsActive() const
	{ return m_bActivated; }
	HitRegion* GetHitRect() const
	{ return m_pHitRect; }

private:
	Image* m_pNormalBitmap;
	Image* m_pHoverBitmap;
	Image* m_pDownBitmap;
	Image* m_pDeactivatedBitmap;
	Image* m_pDeactivatedHoverBitmap;
	Image* m_pDeactivatedDownBitmap;

	D2D1_SIZE_F m_Size;
	D2D1_POINT_2F m_Pos;

	HitRegion* m_pHitRect;

	BUTTON_STATE m_State;
	BUTTON_MODE m_Mode;

	bool m_bClicked;
	bool m_bToggleable;
	bool m_bActivated;
	bool m_bHover;
	bool m_bClick;
	bool m_bDown;

	float m_Opacity;

	// copy & assignment operator
	Button(const Button& second);
	Button& operator=(const Button& second);
};