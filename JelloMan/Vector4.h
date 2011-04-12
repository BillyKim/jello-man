#pragma once
#include "D3DUtil.h"
#include "Vector3.h"

struct Vector4
{
public:
	//Constructors
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector3& v3, float w);
	explicit Vector4(const D3DXCOLOR& color);
    explicit Vector4(const D3DXVECTOR4& v4);

	//Copy en assignment
	Vector4(const Vector4& v);
	Vector4& operator=(const Vector4& v);
    
    //Destructor
    ~Vector4() {}

	//---Getters--------->
    Vector3 XYZ() const;
    Vector2 XY() const;
	void ToFloat4(float* float4) const;
    D3DXCOLOR ToD3DColor() const; 
	//<------------------

	//----------Operators--------------------->
	Vector4 operator-() const;

	Vector4 operator*(float a) const;
	Vector4 operator/(float a) const;

	Vector4 operator+(const Vector4& v) const;
	Vector4 operator-(const Vector4& v) const;

	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float a);
	Vector4& operator/=(float a);

	bool operator==(const Vector4& v) const;
	bool operator!=(const Vector4& v) const;

	//<-----------------------------------------

	//---Public Datamembers--->
	float X;
    float Y;
    float Z;
    float W;
	//<------------------------
};

