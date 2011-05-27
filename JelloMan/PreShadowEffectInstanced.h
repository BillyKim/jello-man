#pragma once
#include "Effect.h"
#include "Matrix.h"

class PreShadowEffectInstanced : public Effect
{
public:
    PreShadowEffectInstanced(ID3D10Device* pDXDevice, ID3D10Effect* pEffect);
    virtual ~PreShadowEffectInstanced(void);

    void SetViewProjection(const Matrix& wvp);
    
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
    ID3D10EffectMatrixVariable* m_pVP;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};
