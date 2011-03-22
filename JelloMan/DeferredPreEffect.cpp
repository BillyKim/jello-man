#include "DeferredPreEffect.h"


DeferredPreEffect::DeferredPreEffect(ID3D10Device* pDXDevice, ID3D10Effect* pEffect): 
                Effect(pDXDevice, pEffect),
                m_pWorld(GetVariableBySemantic("World")->AsMatrix()),
                m_pWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
                m_pDiffuseMap(GetVariableBySemantic("DiffuseMap")->AsShaderResource()),
                m_pSpecMap(GetVariableBySemantic("SpecMap")->AsShaderResource()),
                m_pGlossMap(GetVariableBySemantic("GlossMap")->AsShaderResource())
{    
    ASSERT(m_pWorld->IsValid());
    ASSERT(m_pWVP->IsValid());
    ASSERT(m_pDiffuseMap->IsValid());
    ASSERT(m_pSpecMap->IsValid());
    ASSERT(m_pGlossMap->IsValid());
}


DeferredPreEffect::~DeferredPreEffect(void)
{
}

void DeferredPreEffect::SetWorld(const Matrix& world)
{
    float* p = (float*)const_cast<Matrix&>(world);
    m_pWorld->SetMatrix(p);
}
void DeferredPreEffect::SetWorldViewProjection(const Matrix& wvp)
{
    m_pWVP->SetMatrix((float*)const_cast<Matrix&>(wvp));
}

void DeferredPreEffect::SetDiffuseMap(Texture2D* diffuseMap)
{
    m_pDiffuseMap->SetResource(diffuseMap->GetColorMap());
}
void DeferredPreEffect::SetSpecMap(Texture2D* specMap)
{
    m_pSpecMap->SetResource(specMap->GetColorMap());
}
void DeferredPreEffect::SetGlossMap(Texture2D* glossMap)
{
    m_pGlossMap->SetResource(glossMap->GetColorMap());
}