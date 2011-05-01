#include "StdAfx.h"
#include "Vector3.h"
#include "MathHelper.h"
#include "Vector4.h"


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
Vector3::Vector3(const NxVec3& v3) : X(v3.x), Y(v3.y), Z(v3.z)
{
}

//Static
const Vector3 Vector3::Up = Vector3(0, 1, 0);
const Vector3 Vector3::Right = Vector3(1, 0, 0);
const Vector3 Vector3::Forward = Vector3(0, 0, -1);
const Vector3 Vector3::One = Vector3(1, 1, 1);
const Vector3 Vector3::Zero = Vector3(0, 0, 0);
const Vector3 Vector3::Infinity = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

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
float Vector3::LengthSqr() const
{
	return sqr(X) + sqr(Y) + sqr(Z);
}

void Vector3::ToFloat(float* f) const
{
    f[0] = X;
    f[1] = Y;
    f[2] = Z;
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
Vector3 Vector3::Cross(const Vector3& v) const
{
	D3DXVECTOR3 temp;
	
	D3DXVec3Cross(&temp, &this->ToD3DVector3(), &v.ToD3DVector3());

	return Vector3(temp);
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

//cast
Vector3::operator NxVec3() const
{
	return NxVec3(X, Y, Z);
}
Vector3::operator D3DXVECTOR3() const
{
	return D3DXVECTOR3(X, Y, Z);
}
//<--------------------------------------------------

//Static
Vector4 Vector3::Transform(const Vector3& vec, const Matrix& matrix)
{
    D3DXVECTOR4 temp;
    D3DXMATRIX m = (D3DXMATRIX)matrix;
    D3DXVECTOR3 v = vec.ToD3DVector3();
    D3DXVec3Transform(&temp, &v, &m);

    return Vector4(temp);
}
Vector3 Vector3::Project(const Vector3& vec, CONST D3D10_VIEWPORT *pViewport,
                           const Matrix& projection, const Matrix& view, const Matrix& world)
{
    D3DXVECTOR3 temp;
    D3DXMATRIX pr = (D3DXMATRIX)projection, 
               vw = (D3DXMATRIX)view, 
               wd = (D3DXMATRIX)world;
    D3DXVECTOR3 v = vec.ToD3DVector3();
    D3DXVec3Project(&temp, &v, pViewport, &pr, &vw, &wd);
    return Vector3(temp);
}
Vector3 Vector3::UnProject(const Vector3& vec, CONST D3D10_VIEWPORT *pViewport,
                        const Matrix& projection, const Matrix& view, const Matrix& world)
{
    D3DXVECTOR3 temp;
    D3DXMATRIX pr = (D3DXMATRIX)projection, 
               vw = (D3DXMATRIX)view, 
               wd = (D3DXMATRIX)world;
    D3DXVECTOR3 v = vec.ToD3DVector3();
    D3DXVec3Unproject(&temp, &v, pViewport, &pr, &vw, &wd);
    return Vector3(temp);
}
Vector3 Vector3::Normalize(const Vector3& vec)
{
    Vector3 temp(vec);
    temp.Normalize();
    return temp;
}

Vector3 Min(const Vector3& a, const Vector3& b)
{
	return Vector3(
        a.X < b.X? a.X : b.X,
        a.Y < b.Y? a.Y : b.Y,
        a.Z < b.Z? a.Z : b.Z);
}
Vector3 Max(const Vector3& a, const Vector3& b)
{
	return Vector3(
        a.X > b.X? a.X : b.X,
        a.Y > b.Y? a.Y : b.Y,
        a.Z > b.Z? a.Z : b.Z);
}
Vector3 Abs(const Vector3& v)
{
    return Vector3(abs(v.X), abs(v.Y), abs(v.Z));
}