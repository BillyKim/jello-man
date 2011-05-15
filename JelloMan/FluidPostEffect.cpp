#include "FluidPostEffect.h"

FluidPostEffect::FluidPostEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pBackbufferMap(GetVariableByName("texBackbuffer")->AsShaderResource()),
                m_pDepthMap(GetVariableByName("texDepth")->AsShaderResource()),
                m_pTexelSize(GetVariableByName("t0")->AsVector()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pBackbufferMap->IsValid() != 0, "");
    ASSERT(m_pDepthMap->IsValid() != 0, "");
    ASSERT(m_pTexelSize->IsValid() != 0, "");

    CreateInputLayout<VertexPosTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


FluidPostEffect::~FluidPostEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void FluidPostEffect::SetBackbufferMap(ID3D10ShaderResourceView* map)
{
	m_pBackbufferMap->SetResource(map);
}

void FluidPostEffect::SetDepthMap(ID3D10ShaderResourceView* map)
{
    m_pDepthMap->SetResource(map);
}
void FluidPostEffect::SetBackbufferSize(int width, int height)
{
    float v[2] = { 1.0f / width, 1.0f / height };
    m_pTexelSize->SetFloatVector(v);
}
Effect* FluidPostEffect::GetEffect()
{
    return this;
}

ID3D10InputLayout* FluidPostEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT FluidPostEffect::GetVertexStride() const
{
    return m_VertexStride;
}