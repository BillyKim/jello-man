#include "PhongEffect.h"

PhongEffect::PhongEffect(ID3D10Device* pDevice, ID3D10Effect* effect): 
				Effect(pDevice, effect), 
                m_pMtxWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
                m_pMtxWorld(GetVariableBySemantic("World")->AsMatrix())
                    /*m_pColorMap(GetVariableBySemantic("ColorMap")->AsShaderResource()), 
                    m_pNormalSpecMap(GetVariableBySemantic("NormalSpecMap")->AsShaderResource()), 
                    m_pPosGlossMap(GetVariableBySemantic("PositionGlossMap")->AsShaderResource()),
                    m_pLightDir(GetVariableBySemantic("LightDir")->AsVector()),
                    m_pLightColor(GetVariableBySemantic("LightColor")->AsVector()),
                    m_pCamDir(GetVariableBySemantic("CamDirection")->AsVector())*/
{
    ASSERT(m_pMtxWVP->IsValid());
    ASSERT(m_pMtxWorld->IsValid());
}


PhongEffect::~PhongEffect(void)
{
}

void PhongEffect::SetWorldViewProjection(const Matrix& wvp)
{
	D3DXMATRIX mat = (D3DXMATRIX)wvp;
    m_pMtxWVP->SetMatrix((float*)mat);
}
void PhongEffect::SetWorld(const Matrix& world)
{
	D3DXMATRIX mat = (D3DXMATRIX)world;
    m_pMtxWorld->SetMatrix((float*)mat);
}

//void PhongEffect::SetLightDirection(const Vector3& v)
//{
//    float f[3];
//    v.ToFloat(f);
//    m_pLightDir->SetFloatVector(f);
//}
//void PhongEffect::SetLightColor(const Vector3& v)
//{
//    float f[3];
//    v.ToFloat(f);
//    m_pLightColor->SetFloatVector(f);
//}
//void PhongEffect::SetCameraDirection(const Vector3& v)
//{
//    float f[3];
//    v.ToFloat(f);
//    m_pCamDir->SetFloatVector(f);
//}
//
//void PhongEffect::SetColorMap(ID3D10ShaderResourceView* map)
//{
//	m_pColorMap->SetResource(map);
//}
//void PhongEffect::SetNormalSpecMap(ID3D10ShaderResourceView* map)
//{
//	m_pNormalSpecMap->SetResource(map);
//}
//void PhongEffect::SetPosGlossMap(ID3D10ShaderResourceView* map)
//{
//	m_pPosGlossMap->SetResource(map);
//}