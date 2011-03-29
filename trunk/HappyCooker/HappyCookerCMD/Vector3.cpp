#include "StdAfx.h"
#include "Vector3.h"


//-------------------Constructors-&-Destructor-------------------->
Vector3::Vector3(): X(0.f), Y(0.f), Z(0.f)
{
}
Vector3::Vector3(float x, float y, float z): X(x), Y(y), Z(z)
{
}
Vector3::Vector3(const Vector2& v2, float z): X(v2.X), Y(v2.Y), Z(z)
{
}

//Static
const Vector3 Vector3::Up = Vector3(0, 1, 0);
const Vector3 Vector3::Right = Vector3(1, 0, 0);
const Vector3 Vector3::Forward = Vector3(0, 0, -1);
const Vector3 Vector3::One = Vector3(1, 1, 1);
const Vector3 Vector3::Zero = Vector3(0, 0, 0);

//Copy en assignment
Vector3::Vector3(const Vector3& v3) : X(v3.X), Y(v3.Y), Z(v3.Z)
{
}
Vector3& Vector3::operator=(const Vector3& v)
{
	X = v.X;
	Y = v.Y;
	Z = v.Z;
	return *this;
}

//<-----------------------------------------------------------------


//----------Getters----------------------->
Vector2 Vector3::XY() const
{
	return Vector2(X, Y);
}
Vector2 Vector3::XZ() const
{
	return Vector2(X, Z);
}
Vector2 Vector3::YZ() const
{
	return Vector2(Y, Z);
}
float Vector3::Length() const
{
	return sqrt(sqr(X) + sqr(Y) + sqr(Z));
}

void Vector3::ToFloat(float* f) const
{
    f[0] = X;
    f[1] = Y;
    f[2] = Z;
}
//<-----------------------------------------
	
//---Modifiers------------>
void Vector3::Normalize()
{
	float len = Length();
    if (len != 0)
    {
	    X /= len;
	    Y /= len;
	    Z /= len;
    }
}
//<-----------------------

//----------Operators--------------------->
Vector3 Vector3::operator-() const
{
	return *this * -1.0f;
}

Vector3 Vector3::operator*(float a) const
{
	return Vector3(X * a, Y * a, Z * a);
}
Vector3 Vector3::operator/(float a) const
{
	return Vector3(X / a, Y / a, Z / a);
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(X + v.X, Y + v.Y, Z + v.Z);
}
Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(X - v.X, Y - v.Y, Z - v.Z);
}

float Vector3::Dot(const Vector3& v) const
{
	return X * v.X + Y * v.Y + Z * v.Z;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	X += v.X;
	Y += v.Y;
	Z += v.Z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v)
{
	X -= v.X;
	Y -= v.Y;
	Z -= v.Z;
	return *this;
}
Vector3& Vector3::operator*=(const Vector3& v)
{
    X *= v.X;
    Y *= v.Y;
    Z *= v.Z;
	return *this;
}
Vector3& Vector3::operator/=(const Vector3& v)
{
    X /= v.X;
    Y /= v.Y;
    Z /= v.Z;
	return *this;
}
Vector3& Vector3::operator*=(float a)
{
	X *= a;
	Y *= a;
	Z *= a;
	return *this;
}
Vector3& Vector3::operator/=(float a)
{
	X /= a;
	Y /= a;
	Z /= a;
	return *this;
}

bool Vector3::operator==(const Vector3& v) const
{
	return X == v.X && Y == v.Y && Z == v.Z;
}
bool Vector3::operator!=(const Vector3& v) const
{
	return !operator==(v);
}

//<--------------------------------------------------