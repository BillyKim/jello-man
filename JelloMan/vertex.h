#pragma once
#include "D3DUtil.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

struct VertexPosCol
{
	VertexPosCol():position(0.0f, 0.0f, 0.0f),color(1.0f,1.0f,1.0f,1.0f){}
	VertexPosCol(float x, float y, float z,	float r, float g, float b, float a):position(x,y,z), color(r,g,b,a){}
	VertexPosCol(const Vector3& pos, const Vector4& col):position(pos), color(col){}

	Vector3 position;
	Vector4 color;
};

//===============================================================
struct VertexPosNormCol
{
	VertexPosNormCol():position(0.0f, 0.0f, 0.0f),normal(0.0f, 0.0f, 0.0f),color(1.0f,1.0f,1.0f,1.0f){}
	VertexPosNormCol(float x, float y, float z ,
                     float nx, float ny, float nz,	
                     float r, float g, float b, float a): 
             position(x,y,z), normal(nx,ny,nz), color(r,g,b,a) {}
    VertexPosNormCol(const Vector3& pos, const Vector3& norm, const Vector4& col):
             position(pos), normal(norm), color(col) {}

	Vector3 position;
	Vector3 normal;
	Vector4 color;
};

//===============================================================
struct VertexPosTex
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
struct VertexPosNormTex
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
struct VertexPos
{
    VertexPos(): position(0.0f, 0.0f, 0.0f) {}
    VertexPos(float x, float y, float z): position(x, y, z) {}
    VertexPos(const Vector3& pos): position(pos) {}

    Vector3 position;
};