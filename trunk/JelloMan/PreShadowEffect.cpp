#include "PreShadowEffect.h"


PreShadowEffect::PreShadowEffect(ID3D10Device* pDXDevice, ID3D10Effect* pEffect): 
                Effect(pDXDevice, pEffect),
                m_pWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
                m_pInputLayout(0), m_VertexStride(0)
{    
    ASSERT(m_pWVP->IsValid());

    CreateInputLayout<VertexPosNormTanTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


PreShadowEffect::~PreShadowEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void PreShadowEffect::SetWorldViewProjection(const Matrix& wvp)
{
    m_pWVP->SetMatrix((float*)const_cast<Matrix&>(wvp));
}

ID3D10InputLayout* PreShadowEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0);
    return m_pInputLayout;
}
UINT PreShadowEffect::GetVertexStride() const
{
    return m_VertexStride;
}