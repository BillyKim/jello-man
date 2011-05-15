#pragma once
#include "D3DUtil.h"
#include "Effect.h"
#include "PostProcessEffect.h"

class FluidPostEffect : public Effect, public PostProcessEffect
{
public:
	FluidPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~FluidPostEffect(void);

    virtual void SetBackbufferMap(ID3D10ShaderResourceView* map);
    virtual void SetColorGlowMap(ID3D10ShaderResourceView* map) {};
    virtual void SetNormalMap(ID3D10ShaderResourceView* map) {};
    virtual void SetDepthMap(ID3D10ShaderResourceView* map);
    virtual void SetBackbufferSize(int width, int height);
    Effect* GetEffect();
    
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
	ID3D10EffectShaderResourceVariable* m_pBackbufferMap;
	ID3D10EffectShaderResourceVariable* m_pDepthMap;
    ID3D10EffectVectorVariable* m_pTexelSize;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};