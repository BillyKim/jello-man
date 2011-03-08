#include "StdAfx.h"
#include "Vector2.h"

//-------------------Constructors-&-Destructor-------------------->
Vector2::Vector2(): X(0.f), Y(0.f)
{
}
Vector2::Vector2(float x, float y): X(x), Y(y)
{
}
Vector2::Vector2(const D3DXVECTOR2& v2): X(v2.x), Y(v2.y)
{
}

//Copy en assignment
Vector2::Vector2(const Vector2& v2) : X(v2.X), Y(v2.Y)
{
}
Vector2& Vector2::operator=(const Vector2& v)
{
	X = v.X;
	Y = v.Y;
	return *this;
}

//Destructor
Vector2::~Vector2(void)
{
}

//<-----------------------------------------------------------------


//----------Getters----------------------->
float Vector2::Length() const
{
	return sqrt(sqr(X) + sqr(Y));
}
D3DXVECTOR2 Vector2::ToD3DVector2() const
{
	return D3DXVECTOR2(X, Y);
}
//<-----------------------------------------
	
//---Modifiers------------>
void Vector2::Normalize()
{
	float len = Length();
	X /= len;
	Y /= len;
}
//<-----------------------

//----------Operators--------------------->
Vector2 Vector2::operator-() const
{
	return *this * -1.0f;
}

Vector2 Vector2::operator*(float a) const
{
	return Vector2(X * a, Y * a);
}
Vector2 Vector2::operator/(float a) const
{
	return Vector2(X / a, Y / a);
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(X + v.X, Y + v.Y);
}
Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(X - v.X, Y - v.Y);
}

float Vector2::Dot(const Vector2& v) const
{
	return X * v.X + Y * v.Y;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	X += v.X;
	Y += v.Y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& v)
{
	X -= v.X;
	Y -= v.Y;
	return *this;
}
Vector2& Vector2::operator*=(float a)
{
	X *= a;
	Y *= a;
	return *this;
}
Vector2& Vector2::operator/=(float a)
{
	X /= a;
	Y /= a;
	return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
	return X == v.X && Y == v.Y;
}
bool Vector2::operator!=(const Vector2& v) const
{
	return !operator==(v);
}
//<--------------------------------------------------