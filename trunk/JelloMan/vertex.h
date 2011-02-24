#pragma once
struct VertexPosCol
{
	VertexPosCol():pos(0.0f, 0.0f, 0.0f),color(1.0f,1.0f,1.0f,1.0f){}
	VertexPosCol(float x, float y, float z,	float r, float g, float b, float a):pos(x,y,z), color(r,g,b,a){}

	D3DXVECTOR3 pos;
	D3DXCOLOR color;
};

struct VertexPosNormCol
{
	VertexPosNormCol():pos(0.0f, 0.0f, 0.0f),color(1.0f,1.0f,1.0f,1.0f),normal(0.0f, 0.0f, 0.0f){}
	VertexPosNormCol(float x, float y, float z,	float r, float g, float b, float a, float nx, float ny, float nz):pos(x,y,z), color(r,g,b,a),normal(nx,ny,nz){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXCOLOR color;
};
//===============================================================
struct VertexPosTex
{
	VertexPosTex():
		pos(0.0f, 0.0f, 0.0f),
		tex(0.0f, 0.0f)
	{

	}
	VertexPosTex(float x, float y, float z, float u, float v):pos(x,y,z),  tex(u,v){}
	VertexPosTex(const D3DXVECTOR3& v, const D3DXVECTOR2& uv):pos(v), tex(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex;
};

//===============================================================
struct VertexPosNormTex
{
	//default constructor
	VertexPosNormTex():
		pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex(0.0f, 0.0f)
	{

	}

	//overloaded constructor
	VertexPosNormTex(	float x, float y, float z, 
						float nx, float ny, float nz,
						float u, float v):
		pos(x,y,z), 
		normal(nx,ny,nz), 
		tex(u,v)
	{

	}

	//overloaded constructor
	VertexPosNormTex(const D3DXVECTOR3& p, const D3DXVECTOR3& n, const D3DXVECTOR2& t)
		:pos(p),normal(n), tex(t)
	{

	}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex;
};
