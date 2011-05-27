#include "PreShadowEffectInstanced.h"


PreShadowEffectInstanced::PreShadowEffectInstanced(ID3D10Device* pDXDevice, ID3D10Effect* pEffect): 
                Effect(pDXDevice, pEffect),
                m_pVP(GetVariableBySemantic("ViewProjection")->AsMatrix()),
                m_pInputLayout(0), m_VertexStride(0)
{    
    ASSERT(m_pVP->IsValid() != 0, "");

    CreateInputLayout<VertexPosNormTanTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride, true);
}


PreShadowEffectInstanced::~PreShadowEffectInstanced(void)
{
    SafeRelease(m_pInputLayout);
}

void PreShadowEffectInstanced::SetViewProjection(const Matrix& wvp)
{
    m_pVP->SetMatrix((float*)const_cast<Matrix&>(wvp));
}

ID3D10InputLayout* PreShadowEffectInstanced::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT PreShadowEffectInstanced::GetVertexStride() const
{
    return m_VertexStride;
}