#pragma once
#include "Effect.h"
#include "Matrix.h"

class PreShadowEffect : public Effect
{
public:
    PreShadowEffect(ID3D10Device* pDXDevice, ID3D10Effect* pEffect);
    virtual ~PreShadowEffect(void);

    void SetWorldViewProjection(const Matrix& wvp);
    
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
    ID3D10EffectMatrixVariable* m_pWVP;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};
