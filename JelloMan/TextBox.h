#pragma once

#include "D3DUtil.h"
#include "Blox2D_Engine.h"

// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
// Adapted & modified by Sebastiaan Sprengers
//-----------------------------------------------------------------
// TextBox Class
//-----------------------------------------------------------------

class TextBox
{
public:
	TextBox(tstring const& text);
	TextBox();

	virtual ~TextBox();

	void SetBounds(int x, int y, int width, int height);
	tstring GetText();
	void SetText(tstring const& text);
	void SetFont(tstring const& fontName, bool bold, bool italic, bool underline, int size);
	void SetBackcolor( COLORREF color );
	void SetForecolor( COLORREF color );
	COLORREF GetForecolor();
	COLORREF GetBackcolor();
	HBRUSH GetBackcolorBrush();
	RECT GetRect();
	void SetEnabled(bool bEnable);
	void Update(void);
	void Show();
	void Hide();

	bool Entered()
	{
		if (m_bEntered)
		{
			m_bEntered = false;
			return true;
		}
		else
			return false;
	}

	void LoseFocus() const
	{
		SetFocus(BLOX_2D->GetWindowHandle());
	}

private:
	// -------------------------
	// Datamembers
	// -------------------------
	int m_x, m_y;
	HWND m_hWndEdit;
	WNDPROC m_procOldEdit;
	COLORREF m_BgColor, m_ForeColor;
	HBRUSH m_BgColorBrush;
	HFONT m_Font, m_OldFont;

	bool m_bEntered;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK EditProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT EditProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	TextBox(const TextBox& t);
	TextBox& operator=(const TextBox& t);
};