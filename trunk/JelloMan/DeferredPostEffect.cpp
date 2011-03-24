#include "DeferredPostEffect.h"

DeferredPostEffect::DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                    m_pColorMap(GetVariableBySemantic("ColorMap")->AsShaderResource()), 
                    m_pNormalSpecMap(GetVariableBySemantic("NormalSpecMap")->AsShaderResource()), 
                    m_pPosGlossMap(GetVariableBySemantic("PositionGlossMap")->AsShaderResource()),
                    m_pPointLight(GetVariableBySemantic("PointLight")),
                    m_pSpotLight(GetVariableBySemantic("SpotLight")),
                    m_pCamPos(GetVariableBySemantic("CameraPosition")->AsVector())
{
    ASSERT(m_pColorMap->IsValid());
    ASSERT(m_pNormalSpecMap->IsValid());
    ASSERT(m_pPosGlossMap->IsValid());
    ASSERT(m_pPointLight->IsValid());
    ASSERT(m_pCamPos->IsValid());
}


DeferredPostEffect::~DeferredPostEffect(void)
{
}

void DeferredPostEffect::SetPointLight(const PointLight& light)
{
    m_pPointLight->SetRawValue(reinterpret_cast<void*>(&const_cast<PointLight&>(light)), 0, sizeof(PointLight));
}
void DeferredPostEffect::SetSpotLight(const SpotLight& light)
{
    m_pSpotLight->SetRawValue(reinterpret_cast<void*>(&const_cast<SpotLight&>(light)), 0, sizeof(SpotLight));
}

void DeferredPostEffect::SetColorMap(ID3D10ShaderResourceView* map)
{
	m_pColorMap->SetResource(map);
}
void DeferredPostEffect::SetNormalSpecMap(ID3D10ShaderResourceView* map)
{
	m_pNormalSpecMap->SetResource(map);
}
void DeferredPostEffect::SetPosGlossMap(ID3D10ShaderResourceView* map)
{
	m_pPosGlossMap->SetResource(map);
}
void DeferredPostEffect::SetCameraPosition(const Vector3& camPos)
{
    float f[3];
    camPos.ToFloat(f);
    m_pCamPos->SetFloatVector(f);
}