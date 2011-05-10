#pragma once
#include "D3DUtil.h"
#include "Effect.h"
#include "Vector3.h"
#include <vector>
#include "LightDesc.h"
#include "Light.h"

class DeferredPostEffect : public Effect
{
public:
	DeferredPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~DeferredPostEffect(void);

	void SetColorGlowMap(ID3D10ShaderResourceView* map);
	void SetNormalSpecMap(ID3D10ShaderResourceView* map);
	void SetPosGlossMap(ID3D10ShaderResourceView* map);

    void SetShadowMap(ID3D10ShaderResourceView* map);
    void SetShadowWVP(const Matrix& wvp);
    void SetShadowMapType(ShadowMapType type);

    void SetPointLight(const PointLightDesc& light);
    void SetSpotLight(const SpotLightDesc& light);

    void SetCameraPosition(const Vector3& camPos);
    
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
	ID3D10EffectShaderResourceVariable *m_pColorGlowMap, 
									   *m_pNormalSpecMap, 
									   *m_pPosGlossMap,
                                       *m_pShadowMap;

    ID3D10EffectVariable* m_pPointLight;
    ID3D10EffectVariable* m_pSpotLight;

    ID3D10EffectVectorVariable* m_pCamPos;

    ID3D10EffectMatrixVariable* m_pMtxLightWVP;
    ID3D10EffectScalarVariable* m_pTexelSize;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};