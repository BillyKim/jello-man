#include "stdafx.h"
#include "Texture2D.h"


Texture2D::Texture2D(ID3D10ShaderResourceView* pTex)
{
}


Texture2D::~Texture2D(void)
{
}
void Texture2D::Release()
{
    SafeRelease(m_pTex);
}