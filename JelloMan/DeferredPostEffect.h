#pragma once
#include "D3DUtil.h"
#include "Effect.h"
#include "Vector3.h"
#include <vector>
#include "Light.h"

class DeferredPostEffect : public Effect
{
public:
	DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~DeferredPostEffect(void);

	void SetColorMap(ID3D10ShaderResourceView* map);
	void SetNormalSpecMap(ID3D10ShaderResourceView* map);
	void SetPosGlossMap(ID3D10ShaderResourceView* map);

    void SetPointLight(const PointLight& light);
    void SetSpotLight(const SpotLight& light);

    void SetCameraPosition(const Vector3& camPos);

private:
	ID3D10EffectShaderResourceVariable* m_pColorMap, 
									   *m_pNormalSpecMap, 
									   *m_pPosGlossMap;

    ID3D10EffectVariable* m_pPointLight;
    ID3D10EffectVariable* m_pSpotLight;

    ID3D10EffectVectorVariable* m_pCamPos;
};