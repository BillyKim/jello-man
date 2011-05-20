#include "DeferredPreEffectNormalsInstanced.h"


DeferredPreEffectNormalsInstanced::DeferredPreEffectNormalsInstanced(ID3D10Device* pDXDevice, ID3D10Effect* pEffect): 
                Effect(pDXDevice, pEffect),
                m_pVP(GetVariableBySemantic("ViewProjection")->AsMatrix()),
                m_pDiffuseMap(GetVariableBySemantic("DiffuseMap")->AsShaderResource()),
                m_pSpecMap(GetVariableBySemantic("SpecMap")->AsShaderResource()),
                m_pGlossMap(GetVariableBySemantic("GlossMap")->AsShaderResource()),
				m_pNormalMap(GetVariableBySemantic("NormalMap")->AsShaderResource()),
				m_bSelected(GetVariableBySemantic("Selected")->AsScalar()),
                m_pInputLayout(0), m_VertexStride(0)
{    
    ASSERT(m_pVP->IsValid() != 0, "");
    ASSERT(m_pDiffuseMap->IsValid() != 0, "");
    ASSERT(m_pSpecMap->IsValid() != 0, "");
    ASSERT(m_pGlossMap->IsValid() != 0, "");
	ASSERT(m_pNormalMap->IsValid() != 0, "");

    CreateInputLayout<VertexPosNormTanTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride, true);
}


DeferredPreEffectNormalsInstanced::~DeferredPreEffectNormalsInstanced(void)
{
    SafeRelease(m_pInputLayout);
}

void DeferredPreEffectNormalsInstanced::SetViewProjection(const Matrix& vp)
{
    m_pVP->SetMatrix((float*)const_cast<Matrix&>(vp));
}

void DeferredPreEffectNormalsInstanced::SetDiffuseMap(Texture2D* diffuseMap)
{
    m_pDiffuseMap->SetResource(diffuseMap->GetColorMap());
}
void DeferredPreEffectNormalsInstanced::SetSpecMap(Texture2D* specMap)
{
    m_pSpecMap->SetResource(specMap->GetColorMap());
}
void DeferredPreEffectNormalsInstanced::SetGlossMap(Texture2D* glossMap)
{
    m_pGlossMap->SetResource(glossMap->GetColorMap());
}
void DeferredPreEffectNormalsInstanced::SetNormalMap(Texture2D* normalMap)
{
	m_pNormalMap->SetResource(normalMap->GetColorMap());
}
void DeferredPreEffectNormalsInstanced::Selected(bool selected)
{
	m_bSelected->SetBool(selected);
}

ID3D10InputLayout* DeferredPreEffectNormalsInstanced::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT DeferredPreEffectNormalsInstanced::GetVertexStride() const
{
    return m_VertexStride;
}