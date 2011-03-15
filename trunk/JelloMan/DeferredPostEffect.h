#pragma once
#include "D3DUtil.h"
#include "Effect.h"
#include "Vector3.h"

class DeferredPostEffect : public Effect
{
public:
	DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~DeferredPostEffect(void);

	void SetColorMap(ID3D10ShaderResourceView* map);
	void SetNormalSpecMap(ID3D10ShaderResourceView* map);
	void SetPosGlossMap(ID3D10ShaderResourceView* map);

    void SetLightDirection(const Vector3& v);
    void SetLightColor(const Vector3& v);
    void SetCameraDirection(const Vector3& v);

private:
	ID3D10EffectShaderResourceVariable* m_pColorMap, 
									   *m_pNormalSpecMap, 
									   *m_pPosGlossMap;

    ID3D10EffectVectorVariable* m_pLightDir;
    ID3D10EffectVectorVariable* m_pLightColor;
    ID3D10EffectVectorVariable* m_pCamDir;
};

