#pragma once
#include "Effect.h"
#include "Vector3.h"
#include "Matrix.h"

class FluidEffect : public Effect
{
public:
	FluidEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~FluidEffect(void);

    void SetWorldViewProjection(const Matrix& wvp);
        
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
    ID3D10EffectMatrixVariable* m_pMtxWVP;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};

