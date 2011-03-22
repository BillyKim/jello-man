#include "StdAfx.h"
#include "Color.h"
#include "MathHelper.h"


//-------------------Constructors-&-Destructor-------------------->
Color::Color(): R(0.f), G(0.f), B(0.f), A(0.f)
{
}
Color::Color(float r, float g, float b, float a): R(r), G(g), B(b), A(a)
{
}
Color::Color(BYTE r, BYTE g, BYTE b, BYTE a): R(r / 255.0f), G(g / 255.0f), B(b / 255.0f), A(a / 255.0f)
{
}
Color::Color(const Vector3& rgb, float a): R(rgb.X), G(rgb.Y), B(rgb.Z), A(a)
{
}
Color::Color(const D3DXCOLOR& color): R(color.r), G(color.g), B(color.b), A(color.a)
{
}

//Copy en assignment
Color::Color(const Color& rgba): R(rgba.R), G(rgba.G), B(rgba.B), A(rgba.A)
{
}
Color& Color::operator=(const Color& v)
{
	R = v.R;
	G = v.G;
	B = v.B;
	A = v.A;
	return *this;
}

//<-----------------------------------------------------------------


//----------Getters----------------------->
D3DXCOLOR Color::ToD3DColor() const
{
    return D3DXCOLOR(R, G, B, A);
}
//<-----------------------------------------

//----------Operators--------------------->
Color Color::operator-() const
{
	return *this * -1.0f;
}

Color Color::operator*(float a) const
{
	return Color(R * a, G * a, B * a, A * a);
}
Color Color::operator/(float a) const
{
	return Color(R / a, G / a, B / a, A / a);
}

Color Color::operator+(const Color& v) const
{
    return Color(R + v.R, G + v.G, B + v.B, A +  v.A);
}
Color Color::operator-(const Color& v) const
{
    return Color(R - v.R, G - v.G, B - v.B, A - v.A);
}

Color& Color::operator+=(const Color& v)
{
	R += v.R;
	G += v.G;
	B += v.B;
    A += v.A;
	return *this;
}
Color& Color::operator-=(const Color& v)
{
	R -= v.R;
	G -= v.G;
	B -= v.B;
    A -= v.A;
	return *this;
}
Color& Color::operator*=(float a)
{
	R *= a;
	G *= a;
	B *= a;
    A *= a;
	return *this;
}
Color& Color::operator/=(float a)
{
	R /= a;
	G /= a;
	B /= a;
    A /= a;
	return *this;
}

bool Color::operator==(const Color& v) const
{
    return R == v.R && G == v.G && B == v.B && A == v.A;
}
bool Color::operator!=(const Color& v) const
{
	return !operator==(v);
}
//<--------------------------------------------------