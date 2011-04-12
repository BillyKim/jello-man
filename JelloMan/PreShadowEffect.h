#pragma once
#include "Effect.h"
#include "Matrix.h"

class PreShadowEffect : public Effect
{
public:
    PreShadowEffect(ID3D10Device* pDXDevice, ID3D10Effect* pEffect);
    virtual ~PreShadowEffect(void);

    void SetWorldViewProjection(const Matrix& wvp);

private:
    ID3D10EffectMatrixVariable* m_pWVP;
};
