#include "UnlitTexEffect.h"

UnlitTexEffect::UnlitTexEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pMtxWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
                m_pDiffuseMap(GetVariableByName("diffuseMap")->AsShaderResource()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pMtxWVP->IsValid() != 0, "");
	ASSERT(m_pDiffuseMap->IsValid() != 0, "");

    CreateInputLayout<VertexPosNormTanTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


UnlitTexEffect::~UnlitTexEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void UnlitTexEffect::SetWorldViewProjection(const Matrix& wvp)
{
	D3DXMATRIX mat = (D3DXMATRIX)wvp;
    m_pMtxWVP->SetMatrix((float*)mat);
}
void UnlitTexEffect::SetDiffuseMap(const Texture2D* map)
{
    m_pDiffuseMap->SetResource(map->GetColorMap());
}

ID3D10InputLayout* UnlitTexEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT UnlitTexEffect::GetVertexStride() const
{
    return m_VertexStride;
}