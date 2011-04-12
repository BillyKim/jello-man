#include "PreShadowEffect.h"


PreShadowEffect::PreShadowEffect(ID3D10Device* pDXDevice, ID3D10Effect* pEffect): 
                Effect(pDXDevice, pEffect),
                m_pWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix())
{    
    ASSERT(m_pWVP->IsValid());
}


PreShadowEffect::~PreShadowEffect(void)
{
}

void PreShadowEffect::SetWorldViewProjection(const Matrix& wvp)
{
    m_pWVP->SetMatrix((float*)const_cast<Matrix&>(wvp));
}