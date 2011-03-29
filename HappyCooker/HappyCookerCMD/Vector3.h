#pragma once
#include "Vector2.h"

struct Vector3
{
public:
	//Constructors
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector2& v2, float z);
	//Vector3(const NxVec3& v3);

    //Static const
    static const Vector3 Up;
    static const Vector3 Right;
    static const Vector3 Forward;
    static const Vector3 One;
    static const Vector3 Zero;

	//Copy en assignment
	Vector3(const Vector3& v);
	Vector3& operator=(const Vector3& v);

	//---Getters--------->
	Vector2 XY() const; 
	Vector2 XZ() const; 
	Vector2 YZ() const; 
	float Length() const; 
    void ToFloat(float* f) const;
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

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const Vector3& a);
	Vector3& operator/=(const Vector3& a);
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

