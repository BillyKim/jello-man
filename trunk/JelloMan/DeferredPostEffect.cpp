#include "DeferredPostEffect.h"

DeferredPostEffect::DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                    m_pColorMap(GetVariableBySemantic("ColorMap")->AsShaderResource()), 
                    m_pNormalSpecMap(GetVariableBySemantic("NormalSpecMap")->AsShaderResource()), 
                    m_pPosGlossMap(GetVariableBySemantic("PositionGlossMap")->AsShaderResource()),
                    m_pPointLightArr(GetVariableBySemantic("PointLightArr")),
                    m_pPointLightCount(GetVariableBySemantic("PointLightCount")->AsScalar()),
                    m_pCamPos(GetVariableBySemantic("CameraPosition")->AsVector())
{
    ASSERT(m_pColorMap->IsValid());
    ASSERT(m_pNormalSpecMap->IsValid());
    ASSERT(m_pPosGlossMap->IsValid());
    ASSERT(m_pPointLightArr->IsValid());
    ASSERT(m_pPointLightCount->IsValid());
    ASSERT(m_pCamPos->IsValid());
}


DeferredPostEffect::~DeferredPostEffect(void)
{
}

void DeferredPostEffect::SetPointLights(const vector<PointLight>& lights)
{
    PointLight* p = const_cast<vector<PointLight>&>(lights).data();
    m_pPointLightArr->SetRawValue(reinterpret_cast<void*>(p), 0, sizeof(PointLight) * lights.size());
    m_pPointLightCount->SetInt(lights.size());
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