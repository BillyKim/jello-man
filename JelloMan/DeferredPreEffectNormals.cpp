#include "DeferredPreEffectNormals.h"


DeferredPreEffectNormals::DeferredPreEffectNormals(ID3D10Device* pDXDevice, ID3D10Effect* pEffect): 
                Effect(pDXDevice, pEffect),
                m_pWorld(GetVariableBySemantic("World")->AsMatrix()),
                m_pWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
                m_pDiffuseMap(GetVariableBySemantic("DiffuseMap")->AsShaderResource()),
                m_pSpecMap(GetVariableBySemantic("SpecMap")->AsShaderResource()),
                m_pGlossMap(GetVariableBySemantic("GlossMap")->AsShaderResource()),
				m_pNormalMap(GetVariableBySemantic("NormalMap")->AsShaderResource()),
				m_bSelected(GetVariableBySemantic("Selected")->AsScalar())
{    
    ASSERT(m_pWorld->IsValid());
    ASSERT(m_pWVP->IsValid());
    ASSERT(m_pDiffuseMap->IsValid());
    ASSERT(m_pSpecMap->IsValid());
    ASSERT(m_pGlossMap->IsValid());
	ASSERT(m_pNormalMap->IsValid());
}


DeferredPreEffectNormals::~DeferredPreEffectNormals(void)
{
}

void DeferredPreEffectNormals::SetWorld(const Matrix& world)
{
    float* p = (float*)const_cast<Matrix&>(world);
    m_pWorld->SetMatrix(p);
}
void DeferredPreEffectNormals::SetWorldViewProjection(const Matrix& wvp)
{
    m_pWVP->SetMatrix((float*)const_cast<Matrix&>(wvp));
}

void DeferredPreEffectNormals::SetDiffuseMap(Texture2D* diffuseMap)
{
    m_pDiffuseMap->SetResource(diffuseMap->GetColorMap());
}
void DeferredPreEffectNormals::SetSpecMap(Texture2D* specMap)
{
    m_pSpecMap->SetResource(specMap->GetColorMap());
}
void DeferredPreEffectNormals::SetGlossMap(Texture2D* glossMap)
{
    m_pGlossMap->SetResource(glossMap->GetColorMap());
}
void DeferredPreEffectNormals::SetNormalMap(Texture2D* normalMap)
{
	m_pNormalMap->SetResource(normalMap->GetColorMap());
}
void DeferredPreEffectNormals::Selected(bool selected)
{
	m_bSelected->SetBool(selected);
}