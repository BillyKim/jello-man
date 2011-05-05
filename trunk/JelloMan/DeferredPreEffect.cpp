#include "DeferredPreEffect.h"


DeferredPreEffect::DeferredPreEffect(ID3D10Device* pDXDevice, ID3D10Effect* pEffect): 
                Effect(pDXDevice, pEffect),
                m_pWorld(GetVariableBySemantic("World")->AsMatrix()),
                m_pWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
                m_pDiffuseMap(GetVariableBySemantic("DiffuseMap")->AsShaderResource()),
                m_pSpecMap(GetVariableBySemantic("SpecMap")->AsShaderResource()),
                m_pGlossMap(GetVariableBySemantic("GlossMap")->AsShaderResource()),
				m_bSelected(GetVariableBySemantic("Selected")->AsScalar()),
                m_pInputLayout(0), m_VertexStride(0)
{    
    ASSERT(m_pWorld->IsValid(), "");
    ASSERT(m_pWVP->IsValid(), "");
    ASSERT(m_pDiffuseMap->IsValid(), "");
    ASSERT(m_pSpecMap->IsValid(), "");
    ASSERT(m_pGlossMap->IsValid(), "");

    CreateInputLayout<VertexPosNormTanTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


DeferredPreEffect::~DeferredPreEffect(void)
{
    SafeRelease(m_pInputLayout);
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
void DeferredPreEffect::Selected(bool selected)
{
	m_bSelected->SetBool(selected);
}

ID3D10InputLayout* DeferredPreEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT DeferredPreEffect::GetVertexStride() const
{
    return m_VertexStride;
}