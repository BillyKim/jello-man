#pragma once
#include "Vector3.h"
#include "D3DUtil.h"

struct Vector4
{
public:
	//Constructors
	Vector4();
	Vector4(float r, float g, float b, float a);
	Vector4(const Vector3& v3, float w);

	//Copy en assignment
	Vector4(const Vector4& v);
	Vector4& operator=(const Vector4& v);

	//Destructor
	~Vector4(void);

	//---Getters--------->
    Vector3 XYZ() const;
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
	float R;
    float G;
    float B;
    float A;
	//<------------------------
};

