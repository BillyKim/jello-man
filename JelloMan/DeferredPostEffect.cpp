#include "DeferredPostEffect.h"

DeferredPostEffect::DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                    m_pColorMap(GetVariableBySemantic("ColorMap")->AsShaderResource()), 
                    m_pNormalSpecMap(GetVariableBySemantic("NormalSpecMap")->AsShaderResource()), 
                    m_pPosGlossMap(GetVariableBySemantic("PositionGlossMap")->AsShaderResource()),
                    m_pLightDir(GetVariableBySemantic("LightDir")->AsVector()),
                    m_pLightColor(GetVariableBySemantic("LightColor")->AsVector()),
                    m_pCamPos(GetVariableBySemantic("CamPosition")->AsVector())
{
    ASSERT(m_pColorMap->IsValid());
    ASSERT(m_pNormalSpecMap->IsValid());
    ASSERT(m_pPosGlossMap->IsValid());
    ASSERT(m_pLightDir->IsValid());
    ASSERT(m_pLightColor->IsValid());
    ASSERT(m_pCamPos->IsValid());
}


DeferredPostEffect::~DeferredPostEffect(void)
{
}

void DeferredPostEffect::SetLightDirection(const Vector3& v)
{
    float f[3];
    v.ToFloat(f);
    m_pLightDir->SetFloatVector(f);
}
void DeferredPostEffect::SetLightColor(const Vector3& v)
{
    float f[3];
    v.ToFloat(f);
    m_pLightColor->SetFloatVector(f);
}
void DeferredPostEffect::SetCameraPosition(const Vector3& v)
{
    float f[3];
    v.ToFloat(f);
    m_pCamPos->SetFloatVector(f);
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