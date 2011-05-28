#pragma once
#include "D3DUtil.h"
#include "Effect.h"
#include "PostProcessEffect.h"

class FluidPostEffect : public Effect
{
public:
	FluidPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~FluidPostEffect(void);

    virtual void SetBackbufferMap(ID3D10ShaderResourceView* map);
    virtual void SetDepthMaps(ID3D10ShaderResourceView* backbufferDepthMap, ID3D10ShaderResourceView* fluidDepthMap);
    virtual void SetBackbufferSize(int width, int height);
    virtual void SetCamDir(const Vector3& dir);
    Effect* GetEffect();
    
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
	ID3D10EffectShaderResourceVariable* m_pBackbufferMap;
	ID3D10EffectShaderResourceVariable* m_pBackbufferDepthMap;
	ID3D10EffectShaderResourceVariable* m_pFluidDepthMap;
    ID3D10EffectVectorVariable* m_pTexelSize;
    ID3D10EffectVectorVariable* m_pCamDir;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};