#pragma once
#include "D3DUtil.h"
#include "Vector4.h"

struct Color
{
public:
	//---Public Datamembers--->
	float R;
    float G;
    float B;
    float A;
	//<------------------------

	//Constructors
	Color();
	Color(float r, float g, float b, float a = 1.0f);
	Color(BYTE r, BYTE g, BYTE b, BYTE a = 255);
	Color(const Vector3& rgb, float a = 1.0f);
	explicit Color(const D3DXCOLOR& color);

    ~Color() {};

	//Copy en assignment
	Color(const Color& rgba);
	Color& operator=(const Color& rgba);

	//---Getters--------->
    D3DXCOLOR ToD3DColor() const; 
	//<------------------

	//----------Operators--------------------->
	Color operator-() const;

	Color operator*(float a) const;
	Color operator/(float a) const;

	Color operator+(const Color& v) const;
	Color operator-(const Color& v) const;

	Color& operator+=(const Color& v);
	Color& operator-=(const Color& v);
	Color& operator*=(float a);
	Color& operator/=(float a);

	bool operator==(const Color& v) const;
	bool operator!=(const Color& v) const;

	//<-----------------------------------------
};

