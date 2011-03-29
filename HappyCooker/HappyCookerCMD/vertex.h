#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <vector>

//===============================================================
struct VertexPosTex //12 + 8 = 20
{
	VertexPosTex():
		position(0.0f, 0.0f, 0.0f),
		tex(0.0f, 0.0f)
	{
	}
	VertexPosTex(float x, float y, float z, float u, float v): position(x,y,z),  tex(u,v){}
	VertexPosTex(const Vector3& pos, const Vector2& uv): position(pos), tex(uv){}

	Vector3 position;
	Vector2 tex;
};

//===============================================================
struct VertexPosNormTex //12 + 12 + 8 = 32
{
	VertexPosNormTex():
		position(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex(0.0f, 0.0f)
	{
	}

	VertexPosNormTex(	float x, float y, float z, 
						float nx, float ny, float nz,
						float u, float v):
		position(x,y,z), 
		normal(nx,ny,nz), 
		tex(u,v)
	{
	}

	VertexPosNormTex(const Vector3& p, const Vector3& n, const Vector2& t)
		:position(p),normal(n), tex(t)
	{
	}

	Vector3 position;
	Vector3 normal;
	Vector2 tex;
};

//===============================================================
struct VertexPos //12
{
    VertexPos(): position(0.0f, 0.0f, 0.0f) {}
    VertexPos(float x, float y, float z): position(x, y, z) {}
    VertexPos(const Vector3& pos): position(pos) {}

    Vector3 position;
};