#pragma once
#include "D3DUtil.h"

struct Vector2
{
public:
	//Constructors
	Vector2();
	Vector2(float x, float y);
	Vector2(const D3DXVECTOR2& v2);

	//Copy en assignment
	Vector2(const Vector2& v);
	Vector2& operator=(const Vector2& v);

	//Destructor
	~Vector2(void);

	//---Getters--------->
	float Length() const; 
	D3DXVECTOR2 ToD3DVector2() const;
	//<------------------

	//---Modifiers--------->
	void Normalize();
	//<---------------------

	//----------Operators--------------------->
	Vector2 operator-() const;

	Vector2 operator*(float a) const;
	Vector2 operator/(float a) const;

	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;

	float Dot(const Vector2& v) const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float a);
	Vector2& operator/=(float a);

	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;
	//<-----------------------------------------

	//---Public Datamembers--->
	float X, Y;
	//<------------------------
};

