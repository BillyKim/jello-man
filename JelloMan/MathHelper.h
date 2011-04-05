#pragma once
#include <D3DX10.h>

//------Math Const-------------------
const float INFINITY = FLT_MAX;
const float EPSILON  = 0.001f;
const float Pi		 = 3.1415927f;
const float TwoPi	 = 6.2831853f;
const float PiOver2  = 1.5707963f;
const float PiOver4  = 0.7853982f;
//<----------------------------------

template<typename T>
D3DX10INLINE T Min(const T& a, const T& b)
{
	return a < b ? a : b;
}
template<typename T>
D3DX10INLINE T Max(const T& a, const T& b)
{
	return a > b ? a : b;
}
 
template<typename T>
D3DX10INLINE T Lerp(const T& a, const T& b, float t)
{
	return a + (b-a)*t;
}

template<typename T>
D3DX10INLINE T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x); 
}

template<typename T>
D3DX10INLINE T sqr(const T& x)
{
	return x * x; 
}

D3DX10INLINE float ToDegrees(float radians)
{
	return radians / Pi * 180.0f;
}
D3DX10INLINE float ToRadians(float degrees)
{
	return degrees * Pi / 180.0f;
}