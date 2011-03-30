#include "ColorPicker.h"

// CONSTRUCTOR - DESTRUCTOR
ColorPicker::ColorPicker()	:	m_bPreviousColorSet(false),
								m_Hue(0)
{
	m_PreviousColor = Vector3(0.0f,0.0f,0.0f);
	m_CurrentColor = Vector3(0.0f,0.0f,0.0f);
	m_ColorPickerPos = Point2F(210.0f,60.0f);
	m_HueColor = Vector3(255.0f,0.0f,0.0f);
	m_ColorPickerSelectPos = Point2F(0.0f,0.0f);
}

ColorPicker::~ColorPicker()
{
}

void ColorPicker::Show(Color& color)
{
	if (!m_bPreviousColorSet)
	{
		m_PreviousColor.R = (color.R * 255);
		m_PreviousColor.G = (color.G * 255);
		m_PreviousColor.B = (color.B * 255);

		m_CurrentColor = m_PreviousColor;

		float *h,*s, *v;
		h = new float(0.0f);
		s = new float(0.0f);
		v = new float(0.0f);

		RGBtoHSV(color.R, color.G, color.B, h, s, v);

		m_HueColor.R = GetHue((int)((255 - ((*h / 360) * 255)))).X;
		m_HueColor.G = GetHue((int)((255 - ((*h / 360) * 255)))).Y;
		m_HueColor.B = GetHue((int)((255 - ((*h / 360) * 255)))).Z;

		m_Hue = (int)(*h);

		m_ColorPickerSelectPos = Point2F(m_ColorPickerPos.x + (*s * 255), m_ColorPickerPos.y + (255 - (*v * 255)));

		delete h;
		delete s;
		delete v;

		m_bPreviousColorSet = true;
	}

	// DRAW
	BLOX_2D->SetAntiAliasing(false);

	BLOX_2D->SetColor(43, 43, 43);
	BLOX_2D->FillRect((int)(m_ColorPickerPos.x - 5), (int)(m_ColorPickerPos.y - 5), 405, 265);
	BLOX_2D->SetColor(120, 120, 120);
	BLOX_2D->DrawRect((int)(m_ColorPickerPos.x - 5), (int)(m_ColorPickerPos.y - 5), 405, 265);

	HitRegion hHitRect(
		HitRegion::TYPE_RECTANGLE,
		(int)(m_ColorPickerPos.x + 280),
		(int)(m_ColorPickerPos.y + 1),
		20,
		255);

	if (hHitRect.HitTest(CONTROLS->GetMousePos()) && CONTROLS->LeftMBDown())
	{
		m_HueColor.R = GetHue((int)(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)).X;
		m_HueColor.G = GetHue((int)(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)).Y;
		m_HueColor.B = GetHue((int)(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)).Z;

		m_Hue = (int)(((255-(CONTROLS->GetMousePos().y - m_ColorPickerPos.y))/255.0f)*360.0f);

		Vector3 rgb = HsvToRgb(m_Hue,(double)((m_ColorPickerSelectPos.x - m_ColorPickerPos.x)/255.0f), (double)((255-(m_ColorPickerSelectPos.y - m_ColorPickerPos.y))/255.0f));

		m_CurrentColor = rgb;
	}

	BLOX_2D->SetColor((int)m_HueColor.R, (int)m_HueColor.G, (int)m_HueColor.B);
	BLOX_2D->FillRect((int)m_ColorPickerPos.x,(int)m_ColorPickerPos.y,255,255);

	HitRegion cHitRect(
		HitRegion::TYPE_RECTANGLE,
		(int)m_ColorPickerPos.x,
		(int)m_ColorPickerPos.y,
		255,
		255);

	if (cHitRect.HitTest(CONTROLS->GetMousePos()) && CONTROLS->LeftMBDown())
	{
		Vector3 rgb = HsvToRgb(m_Hue,(double)((CONTROLS->GetMousePos().x - m_ColorPickerPos.x)/255.0f), (double)((255-(CONTROLS->GetMousePos().y - m_ColorPickerPos.y))/255.0f));

		m_CurrentColor = rgb;

		m_ColorPickerSelectPos = Point2F((CONTROLS->GetMousePos().x - m_ColorPickerPos.x)+m_ColorPickerPos.x,(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)+m_ColorPickerPos.y);
	}

	for (int i = 0; i < 255; ++i)
	{
		BLOX_2D->SetColor(255,255,255,(255-i)/255.0f);

		BLOX_2D->DrawLine((int)(m_ColorPickerPos.x+1+i),(int)m_ColorPickerPos.y,(int)(m_ColorPickerPos.x+1+i),(int)(m_ColorPickerPos.y+255));
	}

	for (int i = 0; i < 255; ++i)
	{
		BLOX_2D->SetColor(0,0,0,i/255.0f);

		BLOX_2D->DrawLine((int)m_ColorPickerPos.x,(int)(m_ColorPickerPos.y+i+1),(int)(m_ColorPickerPos.x+255),(int)(m_ColorPickerPos.y+i+1));
	}

	for (int i = 0; i < 255; ++i)
	{
		BLOX_2D->SetColor((int)GetHue(i).X,(int)GetHue(i).Y,(int)GetHue(i).Z);

		BLOX_2D->DrawLine((int)(m_ColorPickerPos.x+280),(int)(m_ColorPickerPos.y+i+1),(int)(m_ColorPickerPos.x+300),(int)(m_ColorPickerPos.y+i+1));
	}

	BLOX_2D->SetColor((int)m_PreviousColor.R, (int)m_PreviousColor.G, (int)m_PreviousColor.B);
	BLOX_2D->FillRect((int)(m_ColorPickerPos.x+310),(int)(m_ColorPickerPos.y),80,40);

	BLOX_2D->SetColor((int)m_CurrentColor.R, (int)m_CurrentColor.G, (int)m_CurrentColor.B);
	BLOX_2D->FillRect((int)(m_ColorPickerPos.x+310),(int)(m_ColorPickerPos.y+45),80,40);

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->DrawEllipse((int)m_ColorPickerSelectPos.x,(int)m_ColorPickerSelectPos.y,5,5);
	BLOX_2D->SetColor(0,0,0);
	BLOX_2D->DrawEllipse((int)m_ColorPickerSelectPos.x,(int)m_ColorPickerSelectPos.y,6,6);

	D2D1_POINT_2F r[4];
	r[0].x = m_ColorPickerPos.x+273;
	r[0].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1-5);
	r[1].x = m_ColorPickerPos.x+278;
	r[1].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1);
	r[2].x = m_ColorPickerPos.x+273;
	r[2].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1+5);
	r[3].x = m_ColorPickerPos.x+273;
	r[3].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1-5);

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->FillPolygon(r,4);

	tstringstream strm;
	strm << _T("R: ") << (int)m_CurrentColor.R << _T("\n");
	strm << _T("G: ") << (int)m_CurrentColor.G << _T("\n");
	strm << _T("B: ") << (int)m_CurrentColor.B << _T("\n");
	
	BLOX_2D->SetFont(_T("Verdana"),false,false,10);
	BLOX_2D->DrawString(strm.str(),(int)(m_ColorPickerPos.x+310),(int)(m_ColorPickerPos.y+90));

	BLOX_2D->SetAntiAliasing(true);
}

Vector3 ColorPicker::GetHue(int i)
{
	if (i < 255/6)
		return Vector3(255.0f,0.0f,i*6.0f);
	else if (i < 255/3)
		return Vector3(255.0f-((i-(255.0f/6.0f))*6.0f),0.0f,255.0f);
	else if (i < 255/2)
		return Vector3(0.0f,((i-(255.0f/3.0f))*6.0f),255.0f);
	else if (i < ((255*2)/3))
		return Vector3(0.0f,255.0f,255.0f-((i-(255.0f/2.0f))*6.0f));
	else if (i < ((255*5)/6))
		return Vector3(((i-((255.0f*2.0f)/3.0f))*6.0f),255.0f,0.0f);
	else
		return Vector3(255.0f,255.0f-((i-((255.0f*5.0f)/6.0f))*6.0f),0.0f);
}

Vector3 ColorPicker::HsvToRgb(double h, double S, double V)
{
  // ######################################################################
  // T. Nathan Mundhenk
  // mundhenk@usc.edu
  // C/C++ Macro HSV to RGB

  double H = h;
  while (H < 0) { H += 360; };
  while (H >= 360) { H -= 360; };
  double R, G, B;
  if (V <= 0)
    { R = G = B = 0; }
  else if (S <= 0)
  {
    R = G = B = V;
  }
  else
  {
    double hf = H / 60.0;
    int i = (int)hf;
    double f = hf - i;
    double pv = V * (1 - S);
    double qv = V * (1 - S * f);
    double tv = V * (1 - S * (1 - f));
    switch (i)
    {
      // Red is the dominant color
      case 0:
        R = V;
        G = tv;
        B = pv;
        break;
      // Green is the dominant color
      case 1:
        R = qv;
        G = V;
        B = pv;
        break;
      case 2:
        R = pv;
        G = V;
        B = tv;
        break;
      // Blue is the dominant color
      case 3:
        R = pv;
        G = qv;
        B = V;
        break;
      case 4:
        R = tv;
        G = pv;
        B = V;
        break;
      // Red is the dominant color
      case 5:
        R = V;
        G = pv;
        B = qv;
        break;
      // Just in case we overshoot on our math by a little, we put these here. Since its a switch it won't slow us down at all to put these here.
      case 6:
        R = V;
        G = tv;
        B = pv;
        break;
      case -1:
        R = V;
        G = pv;
        B = qv;
        break;
      // The color is not defined, we should throw an error.
      default:
        //LFATAL("i Value error in Pixel conversion, Value is %d", i);
        R = G = B = V; // Just pretend its black/white
        break;
    }
  }

  Vector3 RGB;
  RGB.X = (float)(R * 255.0f);
  RGB.Y = (float)(G * 255.0f);
  RGB.Z = (float)(B * 255.0f);

  return RGB;
}

void ColorPicker::RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;

	max = r;
	if (g > r)
		max = g;
	if (b > g)
		max = b;

	min = r;
	if (g < r)
		min = g;
	if (b < g)
		min = b;

	*v = max;				// v

	delta = max - min;

	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan

	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;
}