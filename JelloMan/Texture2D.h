#pragma once
#include "D3DUtil.h"

class Texture2D
{
public:
	Texture2D(ID3D10ShaderResourceView* pTex);
	virtual ~Texture2D(void);
    void Release();

    ID3D10ShaderResourceView* GetShaderResourceView() { return m_pTex;}

private:
	ID3D10ShaderResourceView* m_pTex;
};

