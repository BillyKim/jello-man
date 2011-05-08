#pragma once

#include "Blox2D.h"
#include "Color.h"
#include "Vector3.h"

class ColorPicker
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	ColorPicker();
	virtual ~ColorPicker();

	// GENERAL
	void Show();

	// SETTERS
	void PreviousColorSet(bool prevColorSet)
	{ m_bPreviousColorSet = prevColorSet; }

	void SetCurrentColor(Color curColor)
	{ m_CurrentColor = curColor; }

	void SetPreviousColor(Color prevColor)
	{ m_PreviousColor = prevColor; }

	// GETTERS
	Color& GetCurrentColor()
	{ return m_CurrentColor; }

	Color& GetPreviousColor()
	{ return m_PreviousColor; }

	Point2D GetColorPickerPos()
	{ return m_ColorPickerPos; }

private:

	Vector3 GetHue(int i);
	Vector3 HsvToRgb(double h, double S, double V);
	void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );
	
	// DATAMEMBERS
	bool m_bPreviousColorSet;

	Color m_PreviousColor;
	Color m_CurrentColor;
	Color m_HueColor;

	Point2D m_ColorPickerPos;
	Point2D m_ColorPickerSelectPos;

	TextFormat* m_pFont;

	int m_Hue;
};

