#pragma once
#include "D3DUtil.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <vector>

struct VertexPosCol //12 + 16 = 28
{
	VertexPosCol():position(0.0f, 0.0f, 0.0f),color(1.0f,1.0f,1.0f,1.0f){}
	VertexPosCol(float x, float y, float z,	float r, float g, float b, float a):position(x,y,z), color(r,g,b,a){}
	VertexPosCol(const Vector3& pos, const Vector4& col):position(pos), color(col){}

	Vector3 position;
	Vector4 color;
};

//===============================================================
struct VertexPosNormCol //12 + 12 + 16 = 40
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

void GetInputElementDesc(D3D10_INPUT_ELEMENT_DESC& desc, 
  const LPCSTR&              semanticName,
  UINT                       semanticIndex,
  DXGI_FORMAT                format,
  UINT                       inputSlot,
  UINT                       alignedByteOffset,
  D3D10_INPUT_CLASSIFICATION inputSlotClass,
  UINT                       instanceDataStepRate);

template <typename T>
void GetInputElementDesc(vector<D3D10_INPUT_ELEMENT_DESC>& desc, UINT& elements)
{
    if (sizeof(T) == sizeof(VertexPos))
    {
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        GetInputElementDesc(desc[0], "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        elements = desc.size();
    }
    else if (sizeof(T) == sizeof(VertexPosCol))
    {
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        GetInputElementDesc(desc[0], "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        GetInputElementDesc(desc[1], "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 12, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        elements = desc.size();
    }
    else if (sizeof(T) == sizeof(VertexPosNormCol))
    {
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        GetInputElementDesc(desc[0], "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        GetInputElementDesc(desc[1], "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        GetInputElementDesc(desc[2], "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 24, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        elements = desc.size();
    }
    else if (sizeof(T) == sizeof(VertexPosNormTex))
    {
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        GetInputElementDesc(desc[0], "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        GetInputElementDesc(desc[1], "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        GetInputElementDesc(desc[2], "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 24, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        elements = desc.size();
    }
    else if (sizeof(T) == sizeof(VertexPosTex))
    {
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        desc.push_back(D3D10_INPUT_ELEMENT_DESC());
        GetInputElementDesc(desc[0], "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        GetInputElementDesc(desc[1], "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 12, 0, D3D10_INPUT_PER_VERTEX_DATA, 0);
        elements = desc.size();
    }
    ASSERT(false);
}