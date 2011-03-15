#pragma once
#include "Effect.h"
#include "Vector3.h"
#include "Matrix.h"

class PhongEffect : public Effect
{
public:
	PhongEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~PhongEffect(void);

    void SetWorldViewProjection(const Matrix& wvp);
    void SetWorld(const Matrix& world);

	//void SetDiffuseMap(ID3D10ShaderResourceView* map);
	//void SetNormalMap(ID3D10ShaderResourceView* map);
	//void SetSpecMap(ID3D10ShaderResourceView* map);
	//void SetGlossMap(ID3D10ShaderResourceView* map);

    //void SetLightDirection(const Vector3& v);
    //void SetLightColor(const Vector3& v);
    //void SetCameraDirection(const Vector3& v);

private:
	/*ID3D10EffectShaderResourceVariable* m_pColorMap, 
									   *m_pNormalSpecMap, 
									   *m_pPosGlossMap;

    ID3D10EffectVectorVariable* m_pLightDir;
    ID3D10EffectVectorVariable* m_pLightColor;
    ID3D10EffectVectorVariable* m_pCamDir;*/

    ID3D10EffectMatrixVariable* m_pMtxWVP;
    ID3D10EffectMatrixVariable* m_pMtxWorld;
};

