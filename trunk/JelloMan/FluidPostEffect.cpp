#include "FluidPostEffect.h"

FluidPostEffect::FluidPostEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pBackbufferMap(GetVariableByName("texBackbuffer")->AsShaderResource()),
                m_pBackbufferDepthMap(GetVariableByName("texDepthBackbuffer")->AsShaderResource()),
                m_pFluidDepthMap(GetVariableByName("texDepthFluids")->AsShaderResource()),
                m_pTexelSize(GetVariableByName("t0")->AsVector()),
                m_pCamDir(GetVariableByName("vCamDir")->AsVector()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pBackbufferMap->IsValid() != 0, "");
    ASSERT(m_pBackbufferDepthMap->IsValid() != 0, "");
    ASSERT(m_pFluidDepthMap->IsValid() != 0, "");
    ASSERT(m_pTexelSize->IsValid() != 0, "");
    ASSERT(m_pCamDir->IsValid() != 0, "");

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
void FluidPostEffect::SetCamDir(const Vector3& dir)
{
    float v[3];
    dir.ToFloat(v);
    m_pCamDir->SetFloatVector(v);
}

void FluidPostEffect::SetDepthMaps(ID3D10ShaderResourceView* backbufferDepthMap, ID3D10ShaderResourceView* fluidDepthMap)
{
    m_pBackbufferDepthMap->SetResource(backbufferDepthMap);
    m_pFluidDepthMap->SetResource(fluidDepthMap);
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