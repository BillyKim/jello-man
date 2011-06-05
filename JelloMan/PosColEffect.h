#pragma once
#include "Effect.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Color.h"

class PosColEffect : public Effect
{
public:
	PosColEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~PosColEffect(void);

    void SetWorldViewProjection(const Matrix& wvp);
    void SetWorld(const Matrix& world);
	void SetColor(const Color& color);
        
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:

    ID3D10EffectMatrixVariable* m_pMtxWVP;
    ID3D10EffectMatrixVariable* m_pMtxWorld;
	ID3D10EffectVectorVariable* m_pColor;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};

