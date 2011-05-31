#pragma once
#include "Effect.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Texture2D.h"

class FluidEffect : public Effect
{
public:
	FluidEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~FluidEffect(void);

    void SetWorldViewProjection(const Matrix& wvp);
    void SetCamPos(const Vector3& camPos);
    void SetRainbowTex(const Texture2D* tex);
        
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
    ID3D10EffectMatrixVariable* m_pMtxWVP;
    ID3D10EffectShaderResourceVariable* m_pRainbowTex;
    ID3D10EffectVectorVariable* m_pCamPos;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};

