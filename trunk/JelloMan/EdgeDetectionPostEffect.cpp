#include "EdgeDetectionPostEffect.h"

EdgeDetectionPostEffect::EdgeDetectionPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                m_pColorMap(GetVariableByName("backBuffer")->AsShaderResource()),
                m_pMapWidth(GetVariableByName("bbWidth")->AsScalar()),
                m_pMapHeight(GetVariableByName("bbHeight")->AsScalar())
{
    ASSERT(m_pColorMap->IsValid());
    ASSERT(m_pMapWidth->IsValid());
    ASSERT(m_pMapHeight->IsValid());
}


EdgeDetectionPostEffect::~EdgeDetectionPostEffect(void)
{
}

void EdgeDetectionPostEffect::SetColorMap(ID3D10ShaderResourceView* map)
{
	m_pColorMap->SetResource(map);
}
void EdgeDetectionPostEffect::SetColorMapSize(int width, int height)
{
    m_pMapWidth->SetInt(width);
    m_pMapHeight->SetInt(height);
}
Effect* EdgeDetectionPostEffect::GetEffect()
{
    return this;
}