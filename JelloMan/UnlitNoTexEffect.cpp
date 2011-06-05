#include "UnlitNoTexEffect.h"

UnlitNoTexEffect::UnlitNoTexEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pMtxWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
				m_pColor(GetVariableBySemantic("Color")->AsVector()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pMtxWVP->IsValid() != 0, "");
	ASSERT(m_pColor->IsValid() != 0, "");

    CreateInputLayout<VertexPosNormTanTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


UnlitNoTexEffect::~UnlitNoTexEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void UnlitNoTexEffect::SetWorldViewProjection(const Matrix& wvp)
{
	D3DXMATRIX mat = (D3DXMATRIX)wvp;
    m_pMtxWVP->SetMatrix((float*)mat);
}
void UnlitNoTexEffect::SetColor(const Color& color)
{
    float vec[4] = {color.R, color.G, color.B, color.A};
	m_pColor->SetFloatVector(vec);
}

ID3D10InputLayout* UnlitNoTexEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT UnlitNoTexEffect::GetVertexStride() const
{
    return m_VertexStride;
}