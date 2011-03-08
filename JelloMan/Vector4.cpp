#include "StdAfx.h"
#include "Vector4.h"
#include "MathHelper.h"


//-------------------Constructors-&-Destructor-------------------->
Vector4::Vector4(): R(0.f), G(0.f), B(0.f)
{
}
Vector4::Vector4(float r, float g, float b, float a): R(r), G(g), B(b), A(a)
{
}
Vector4::Vector4(const Vector3& v3, float a): R(v3.X), G(v3.Y), B(v3.Z), A(a)
{
}

//Copy en assignment
Vector4::Vector4(const Vector4& v4) : R(v4.R), G(v4.G), B(v4.B), A(v4.A)
{
}
Vector4& Vector4::operator=(const Vector4& v)
{
	R = v.R;
	G = v.G;
	B = v.B;
	A = v.A;
	return *this;
}

//Destructor
Vector4::~Vector4(void)
{
}

//<-----------------------------------------------------------------


//----------Getters----------------------->
void Vector4::ToFloat4(float* float4) const
{
    float4[0] = R;
    float4[1] = G;
    float4[2] = B;
    float4[3] = A;
}
//<-----------------------------------------

//----------Operators--------------------->
Vector4 Vector4::operator-() const
{
	return *this * -1.0f;
}

Vector4 Vector4::operator*(float a) const
{
	return Vector4(R * a, G * a, B * a, A * a);
}
Vector4 Vector4::operator/(float a) const
{
	return Vector4(R / a, G / a, B / a, A / a);
}

Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(R + v.R, G + v.G, B + v.B, A +  v.A);
}
Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(R - v.R, G - v.G, B - v.B, A - v.A);
}

Vector4& Vector4::operator+=(const Vector4& v)
{
	R += v.R;
	G += v.G;
	B += v.B;
    A += v.A;
	return *this;
}
Vector4& Vector4::operator-=(const Vector4& v)
{
	R -= v.R;
	G -= v.G;
	B -= v.B;
    A -= v.A;
	return *this;
}
Vector4& Vector4::operator*=(float a)
{
	R *= a;
	G *= a;
	B *= a;
    A *= a;
	return *this;
}
Vector4& Vector4::operator/=(float a)
{
	R /= a;
	G /= a;
	B /= a;
    A /= a;
	return *this;
}

bool Vector4::operator==(const Vector4& v) const
{
    return R == v.R && G == v.G && B == v.B && A == v.A;
}
bool Vector4::operator!=(const Vector4& v) const
{
	return !operator==(v);
}
//<--------------------------------------------------