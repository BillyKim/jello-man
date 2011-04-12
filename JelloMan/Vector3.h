#pragma once
#include "Vector2.h"
#include "D3DUtil.h"
#include "Matrix.h"
#include "PhysX.h"

struct Vector4;

struct Vector3
{
public:
	//Constructors
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector2& v2, float z);
	explicit Vector3(const D3DXVECTOR3& v3);
	explicit Vector3(const NxVec3& v3);

    //Static const
    static const Vector3 Up;
    static const Vector3 Right;
    static const Vector3 Forward;
    static const Vector3 One;
    static const Vector3 Zero;

	//Copy en assignment
	Vector3(const Vector3& v);
	Vector3& operator=(const Vector3& v);

    //Destructor
    ~Vector3() {}

	//---Getters--------->
	Vector2 XY() const; 
	Vector2 XZ() const; 
	Vector2 YZ() const; 
	float Length() const; 
    void ToFloat(float* f) const;
	D3DXVECTOR3 ToD3DVector3() const;
	//<------------------
	
	//---Modifiers--------->
	void Normalize();
	//<---------------------

	//----------Operators--------------------->
	Vector3 operator-() const;

	Vector3 operator*(float a) const;
	Vector3 operator/(float a) const;

	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator*(const Vector3 a) const;
	Vector3 operator/(const Vector3 a) const;

	float Dot(const Vector3& v) const;
	Vector3 Cross(const Vector3& v) const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const Vector3& a);
	Vector3& operator/=(const Vector3& a);
	Vector3& operator*=(float a);
	Vector3& operator/=(float a);

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	//cast
	operator NxVec3() const;
	operator D3DXVECTOR3() const;

	//<-----------------------------------------

    //----------Static--------------------------->
    
    static Vector4 Transform(const Vector3& v, const Matrix& m);
    static Vector3 Project(const Vector3& v, CONST D3D10_VIEWPORT *pViewport,
                           const Matrix& projection, const Matrix& view, const Matrix& world);
    static Vector3 UnProject(const Vector3& v, CONST D3D10_VIEWPORT *pViewport,
                           const Matrix& projection, const Matrix& view, const Matrix& world);
    static Vector3 Normalize(const Vector3& vec);

    //<--------------------------------------------------

	//---Public Datamembers--->
	float X;
    float Y;
    float Z;
	//<------------------------
};

Vector3 Min(const Vector3& a, const Vector3& b);
Vector3 Max(const Vector3& a, const Vector3& b);
Vector3 Abs(const Vector3& v);