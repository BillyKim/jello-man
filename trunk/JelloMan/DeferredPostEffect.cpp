#include "DeferredPostEffect.h"

DeferredPostEffect::DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                    m_pColorMap(GetVariableBySemantic("ColorMap")->AsShaderResource()), 
                    m_pNormalSpecMap(GetVariableBySemantic("NormalSpecMap")->AsShaderResource()), 
                    m_pPosGlossMap(GetVariableBySemantic("PositionGlossMap")->AsShaderResource()),
                    m_pLightDir(GetVariableBySemantic("LightDir")->AsVector()),
                    m_pLightColor(GetVariableBySemantic("LightColor")->AsVector()),
                    m_pCamDir(GetVariableBySemantic("CamDirection")->AsVector())
{
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
void DeferredPostEffect::SetCameraDirection(const Vector3& v)
{
    float f[3];
    v.ToFloat(f);
    m_pCamDir->SetFloatVector(f);
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