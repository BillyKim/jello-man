#pragma once
#include "D3DUtil.h"

struct Vector2
{
public:
	//---Public Datamembers--->
	float X;
    float Y;
	//<------------------------

	//Constructors
	Vector2();
	Vector2(float x, float y);
	explicit Vector2(const D3DXVECTOR2& v2);
    explicit Vector2(const Point2D& point);

    //Static
    static const Vector2 One;     //Vector2(1, 1);
    static const Vector2 Zero;    //Vector2(0, 0);
    static const Vector2 UnitX;   //Vector2(1, 0);
    static const Vector2 UnitY;   //Vector2(0, 1);

	//Copy en assignment
	Vector2(const Vector2& v);
	Vector2& operator=(const Vector2& v);

    //Destructor
    ~Vector2() {}

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
	Vector2 operator*(const Vector2& a) const;
	Vector2 operator/(const Vector2& a) const;

	float Dot(const Vector2& v) const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float a);
	Vector2& operator/=(float a);

	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;
	//<-----------------------------------------
};


Vector2 Min(const Vector2& a, const Vector2& b);
Vector2 Max(const Vector2& a, const Vector2& b);
Vector2 Abs(const Vector2& v);