#include "StdAfx.h"
#include "Vector4.h"
#include "MathHelper.h"


//-------------------Constructors-&-Destructor-------------------->
Vector4::Vector4(): X(0.f), Y(0.f), Z(0.f), W(0.f)
{
}
Vector4::Vector4(float x, float y, float z, float w): X(x), Y(y), Z(z), W(w)
{
}
Vector4::Vector4(const Vector3& v3, float w): X(v3.X), Y(v3.Y), Z(v3.Z), W(w)
{
}
Vector4::Vector4(const D3DXCOLOR& color): X(color.r), Y(color.g), Z(color.b), W(color.a)
{
}
Vector4::Vector4(const D3DXVECTOR4& v4): X(v4.x), Y(v4.y), Z(v4.z), W(v4.w) 
{
}

//Copy en assignment
Vector4::Vector4(const Vector4& v4) : X(v4.X), Y(v4.Y), Z(v4.Z), W(v4.W)
{
}
Vector4& Vector4::operator=(const Vector4& v)
{
	X = v.X;
	Y = v.Y;
	Z = v.Z;
	W = v.W;
	return *this;
}

//<-----------------------------------------------------------------


//----------Getters----------------------->
Vector3 Vector4::XYZ() const
{
    return Vector3(X, Y, Z);
}
Vector2 Vector4::XY() const
{
    return Vector2(X, Y);
}
void Vector4::ToFloat4(float* float4) const
{
    float4[0] = X;
    float4[1] = Y;
    float4[2] = Z;
    float4[3] = W;
}
D3DXCOLOR Vector4::ToD3DColor() const
{
    return D3DXCOLOR(X, Y, Z, W);
}
//<-----------------------------------------

//----------Operators--------------------->
Vector4 Vector4::operator-() const
{
	return *this * -1.0f;
}

Vector4 Vector4::operator*(float a) const
{
	return Vector4(X * a, Y * a, Z * a, W * a);
}
Vector4 Vector4::operator/(float a) const
{
	return Vector4(X / a, Y / a, Z / a, W / a);
}

Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(X + v.X, Y + v.Y, Z + v.Z, W +  v.W);
}
Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(X - v.X, Y - v.Y, Z - v.Z, W - v.W);
}

Vector4& Vector4::operator+=(const Vector4& v)
{
	X += v.X;
	Y += v.Y;
	Z += v.Z;
    W += v.W;
	return *this;
}
Vector4& Vector4::operator-=(const Vector4& v)
{
	X -= v.X;
	Y -= v.Y;
	Z -= v.Z;
    W -= v.W;
	return *this;
}
Vector4& Vector4::operator*=(float a)
{
	X *= a;
	Y *= a;
	Z *= a;
    W *= a;
	return *this;
}
Vector4& Vector4::operator/=(float a)
{
	X /= a;
	Y /= a;
	Z /= a;
    W /= a;
	return *this;
}

bool Vector4::operator==(const Vector4& v) const
{
    return X == v.X && Y == v.Y && Z == v.Z && W == v.W;
}
bool Vector4::operator!=(const Vector4& v) const
{
	return !operator==(v);
}
//<--------------------------------------------------