#include "StdAfx.h"
#include "Vector3.h"
#include "MathHelper.h"


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
Vector3::Vector3(const D3DXVECTOR3& v3) : X(v3.x), Y(v3.y), Z(v3.z)
{
}
//Vector3(const NxVec3& v3);

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

//Destructor
Vector3::~Vector3(void)
{
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
D3DXVECTOR3 Vector3::ToD3DVector3() const
{
	return D3DXVECTOR3(X, Y, Z);
}
//<-----------------------------------------
	
//---Modifiers------------>
void Vector3::Normalize()
{
	float len = Length();
	X /= len;
	Y /= len;
	Z /= len;
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
Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3(*D3DXVec3Cross(0, &this->ToD3DVector3(), &v.ToD3DVector3()));
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