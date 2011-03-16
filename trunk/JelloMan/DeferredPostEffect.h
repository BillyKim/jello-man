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

    void SetPointLights(const vector<PointLight>& lights);

    void SetCameraPosition(const Vector3& camPos);

private:
	ID3D10EffectShaderResourceVariable* m_pColorMap, 
									   *m_pNormalSpecMap, 
									   *m_pPosGlossMap;

    ID3D10EffectVariable* m_pPointLightArr;
    ID3D10EffectScalarVariable* m_pPointLightCount;

    ID3D10EffectVectorVariable* m_pCamPos;
};