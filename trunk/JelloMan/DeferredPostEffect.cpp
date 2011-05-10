#include "DeferredPostEffect.h"

DeferredPostEffect::DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                    m_pColorGlowMap(GetVariableBySemantic("ColorGlowMap")->AsShaderResource()), 
                    m_pNormalSpecMap(GetVariableBySemantic("NormalSpecMap")->AsShaderResource()), 
                    m_pPosGlossMap(GetVariableBySemantic("PositionGlossMap")->AsShaderResource()),
                    m_pShadowMap(GetVariableBySemantic("ShadowMap")->AsShaderResource()),
                    m_pPointLight(GetVariableBySemantic("PointLight")),
                    m_pSpotLight(GetVariableBySemantic("SpotLight")),
                    m_pCamPos(GetVariableBySemantic("CameraPosition")->AsVector()),
                    m_pMtxLightWVP(GetVariableByName("mtxLightWVP")->AsMatrix()),
                    m_pTexelSize(GetVariableByName("t0")->AsScalar()),
                    m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pColorGlowMap->IsValid() != 0, "");
    ASSERT(m_pNormalSpecMap->IsValid() != 0, "");
    ASSERT(m_pPosGlossMap->IsValid() != 0, "");
    ASSERT(m_pShadowMap->IsValid() != 0, "");
    ASSERT(m_pPointLight->IsValid() != 0, "");
    ASSERT(m_pCamPos->IsValid() != 0, "");
    ASSERT(m_pMtxLightWVP->IsValid() != 0, "");
    ASSERT(m_pTexelSize->IsValid() != 0, "");

    CreateInputLayout<VertexPosTex>(pDevice, this, &m_pInputLayout, m_VertexStride);
}


DeferredPostEffect::~DeferredPostEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void DeferredPostEffect::SetPointLight(const PointLightDesc& light)
{
    m_pPointLight->SetRawValue(&const_cast<PointLightDesc&>(light), 0, sizeof(PointLightDesc));
}
void DeferredPostEffect::SetSpotLight(const SpotLightDesc& light)
{
    m_pSpotLight->SetRawValue(&const_cast<SpotLightDesc&>(light), 0, sizeof(SpotLightDesc));
}

void DeferredPostEffect::SetColorGlowMap(ID3D10ShaderResourceView* map)
{
	m_pColorGlowMap->SetResource(map);
}
void DeferredPostEffect::SetNormalSpecMap(ID3D10ShaderResourceView* map)
{
	m_pNormalSpecMap->SetResource(map);
}
void DeferredPostEffect::SetPosGlossMap(ID3D10ShaderResourceView* map)
{
	m_pPosGlossMap->SetResource(map);
}
void DeferredPostEffect::SetShadowMap(ID3D10ShaderResourceView* map)
{
    m_pShadowMap->SetResource(map);
}
void DeferredPostEffect::SetShadowWVP(const Matrix& wvp)
{
    m_pMtxLightWVP->SetMatrix((float*)const_cast<Matrix&>(wvp));
}
void DeferredPostEffect::SetShadowMapType(ShadowMapType type)
{
    m_pTexelSize->SetFloat(1.0f / type);
}
void DeferredPostEffect::SetCameraPosition(const Vector3& camPos)
{
    float f[3];
    camPos.ToFloat(f);
    m_pCamPos->SetFloatVector(f);
}
ID3D10InputLayout* DeferredPostEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT DeferredPostEffect::GetVertexStride() const
{
    return m_VertexStride;
}