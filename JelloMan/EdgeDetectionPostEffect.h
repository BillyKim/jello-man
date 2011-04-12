#pragma once
#include "D3DUtil.h"
#include "Effect.h"
#include "PostProcessEffect.h"

class EdgeDetectionPostEffect : public Effect, public PostProcessEffect
{
public:
	EdgeDetectionPostEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~EdgeDetectionPostEffect(void);

	void SetColorMap(ID3D10ShaderResourceView* map);
    void SetColorMapSize(int width, int height);
    Effect* GetEffect();
    
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
	ID3D10EffectShaderResourceVariable* m_pColorMap;
    ID3D10EffectScalarVariable* m_pMapWidth;
    ID3D10EffectScalarVariable* m_pMapHeight;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};