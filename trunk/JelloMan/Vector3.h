#pragma once
#include "Vector2.h"
#include "D3DUtil.h"

struct Vector3
{
public:
	//Constructors
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector2& v2, float z);
	Vector3(const D3DXVECTOR3& v3);
	//Vector3(const NxVec3& v3);

	//Copy en assignment
	Vector3(const Vector3& v);
	Vector3& operator=(const Vector3& v);

	//Destructor
	~Vector3(void);

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

	float Dot(const Vector3& v) const;
	Vector3 Cross(const Vector3& v) const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float a);
	Vector3& operator/=(float a);

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	//<-----------------------------------------

	//---Public Datamembers--->
	float X;
    float Y;
    float Z;
	//<------------------------
};

