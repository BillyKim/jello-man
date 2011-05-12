#include "EdgeDetectionPostEffect.h"

EdgeDetectionPostEffect::EdgeDetectionPostEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pColorMap(GetVariableByName("backBuffer")->AsShaderResource()),
                m_pNormalMap(GetVariableByName("normalMap")->AsShaderResource()),
                m_pDepthMap(GetVariableByName("depthMap")->AsShaderResource()),
                m_pMapWidth(GetVariableByName("bbWidth")->AsScalar()),
                m_pMapHeight(GetVariableByName("bbHeight")->AsScalar()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pColorMap->IsValid() != 0, "");
    ASSERT(m_pDepthMap->IsValid() != 0, "");
    ASSERT(m_pNormalMap->IsValid() != 0, "");
    ASSERT(m_pMapWidth->IsValid() != 0, "");
    ASSERT(m_pMapHeight->IsValid() != 0, "");

    CreateInputLayout<VertexPosTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


EdgeDetectionPostEffect::~EdgeDetectionPostEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void EdgeDetectionPostEffect::SetBackbufferMap(ID3D10ShaderResourceView* map)
{
	m_pColorMap->SetResource(map);
}
void EdgeDetectionPostEffect::SetNormalMap(ID3D10ShaderResourceView* map)
{
    m_pNormalMap->SetResource(map);
}
void EdgeDetectionPostEffect::SetDepthMap(ID3D10ShaderResourceView* map)
{
    m_pDepthMap->SetResource(map);
}
void EdgeDetectionPostEffect::SetBackbufferSize(int width, int height)
{
    m_pMapWidth->SetInt(width);
    m_pMapHeight->SetInt(height);
}
Effect* EdgeDetectionPostEffect::GetEffect()
{
    return this;
}

ID3D10InputLayout* EdgeDetectionPostEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT EdgeDetectionPostEffect::GetVertexStride() const
{
    return m_VertexStride;
}