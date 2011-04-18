#include "Button.h"
#include "HitRegion.h"
#include "Image.h"

//-----------------------------------------------------------------
// Button Class
//-----------------------------------------------------------------

// constructor
Button::Button(int posX, int posY, int width, int height, bool bToggleable)	:	m_bClicked(false),
																				m_pNormalBitmap(0),
																				m_pHoverBitmap(0),
																				m_pDownBitmap(0),
																				m_pDeactivatedBitmap(0),
																				m_pDeactivatedHoverBitmap(0),
																				m_pDeactivatedDownBitmap(0),
																				m_bToggleable(bToggleable),
																				m_bActivated(true),
																				m_bHover(false),
																				m_bClick(false),
																				m_bDown(false)
{
	m_Pos.x = (float)posX;
	m_Pos.y = (float)posY;

	m_Size.width = (float)width;
	m_Size.height = (float)height;

	m_State = STATE_NORMAL;
	m_Mode = MODE_MOUSE;

	m_Opacity = 1.0f;

	m_pHitRect = new HitRegion(HitRegion::TYPE_RECTANGLE,(float)posX,(float)posY,(float)width,(float)height);
}

// destructor
Button::~Button()
{
	delete m_pHitRect;
}

// general
void Button::Tick()
{
	Point2D mousePos = CONTROLS->GetMousePos();

	if (CONTROLS->LeftMBDown())
		m_bClick = true;

	if (CONTROLS->LeftMBUp())
	{
		if (m_bClick)
		{
			if (m_pHitRect->HitTest(mousePos))
			{
				m_bClicked = true;

				if (m_bToggleable)
					m_bActivated = !m_bActivated;
			}
			else
			{
				m_bClicked = false;
			}			
		}
		else
			m_bClicked = false;

		m_bClick = false;
	}

	// setting states
	if (m_Mode == MODE_MOUSE) // if state has been set manual, don't check with mouseaction
	{
		if (m_bActivated)
		{
			if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == false)
			{
				m_State = STATE_HOVER;
				m_bHover = true;
				m_bDown = false;
			}
			else if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == true)
			{
				m_State = STATE_DOWN;
				m_bDown = true;
				m_bHover = false;
			}
			else
			{
				m_State = STATE_NORMAL;
				m_bHover = false;
				m_bDown = false;
			}
		}
		else
		{
			if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == false)
			{
				m_State = STATE_DEACTIVATED_HOVER;
				m_bHover = true;
				m_bDown = false;
			}
			else if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == true)
			{
				m_State = STATE_DEACTIVATED_DOWN;
				m_bDown = true;
				m_bHover = false;
			}
			else
			{
				m_State = STATE_DEACTIVATED;
				m_bHover = false;
				m_bDown = false;
			}
		}
	}
}

void Button::Show()
{
	Tick();

	if (m_State == STATE_NORMAL)
	{
		if (m_pNormalBitmap)
			BX2D->DrawBitmap(m_pNormalBitmap, m_Pos.x, m_Pos.y, true, m_Opacity);
	}
	else if (m_State == STATE_HOVER)
	{
		if (m_pHoverBitmap)
			BX2D->DrawBitmap(m_pHoverBitmap, m_Pos.x, m_Pos.y, true, m_Opacity);
	}
	else if (m_State == STATE_DOWN)
	{
		if (m_pDownBitmap)
			BX2D->DrawBitmap(m_pDownBitmap, m_Pos.x, m_Pos.y, true, m_Opacity);
	}
	else if (m_State == STATE_DEACTIVATED)
	{
		if (m_pDeactivatedBitmap)
			BX2D->DrawBitmap(m_pDeactivatedBitmap, m_Pos.x, m_Pos.y, true, m_Opacity);
	}
	else if (m_State == STATE_DEACTIVATED_HOVER)
	{
		if (m_pDeactivatedHoverBitmap)
			BX2D->DrawBitmap(m_pDeactivatedHoverBitmap, m_Pos.x, m_Pos.y, true, m_Opacity);
	}
	else if (m_State == STATE_DEACTIVATED_DOWN)
	{
		if (m_pDeactivatedDownBitmap)
			BX2D->DrawBitmap(m_pDeactivatedDownBitmap, m_Pos.x, m_Pos.y, true, m_Opacity);
	}
}

void Button::ShowHitRegion(bool fill)
{
	if (fill)
	{
		D2D1_RECT_F rect = m_pHitRect->GetDimension();
		BX2D->FillRect(rect);
	}
	else
	{
		D2D1_RECT_F rect = m_pHitRect->GetDimension();
		BX2D->DrawRect(rect);
	}
}

// setters
void Button::SetNormalState(Image* normalState)
{
	m_pNormalBitmap = normalState;
}
void Button::SetHoverState(Image* hoverState)
{
	m_pHoverBitmap = hoverState;
}
void Button::SetDownState(Image* downState)
{
	m_pDownBitmap = downState;
}
void Button::SetDeactivatedState(Image* deacivatedState)
{
	m_pDeactivatedBitmap = deacivatedState;
}
void Button::SetDeactivatedStateHover(Image* deactivatedHover)
{
	m_pDeactivatedHoverBitmap = deactivatedHover;
}
void Button::SetDeactivatedStateDown(Image* deactivatedDown)
{
	m_pDeactivatedDownBitmap = deactivatedDown;
}
void Button::SetState(BUTTON_STATE state)
{
	m_State = state;
	if (state == STATE_DEACTIVATED)
		m_bActivated = false;
}
void Button::SetMode(BUTTON_MODE mode)
{
	m_Mode = mode;
}
void Button::SetPosition(int x, int y)
{
	m_Pos.x = (float)x;
	m_Pos.y = (float)y;

	m_pHitRect->SetPosition(Point2F((float)x,(float)y));
}
void Button::Deactivate()
{
	m_State = STATE_DEACTIVATED;
	m_bActivated = false;
}

// getters
bool Button::Clicked()
{
	return m_bClicked;
}