#include "DeferredPostEffect.h"

DeferredPostEffect::DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                    m_pColorMap(GetVariableBySemantic("ColorMap")->AsShaderResource()), 
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
    ASSERT(m_pColorMap->IsValid(), "");
    ASSERT(m_pNormalSpecMap->IsValid(), "");
    ASSERT(m_pPosGlossMap->IsValid(), "");
    ASSERT(m_pShadowMap->IsValid(), "");
    ASSERT(m_pPointLight->IsValid(), "");
    ASSERT(m_pCamPos->IsValid(), "");
    ASSERT(m_pMtxLightWVP->IsValid(), "");
    ASSERT(m_pTexelSize->IsValid(), "");

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