#pragma once
#include "Effect.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Texture2D.h"

class UnlitTexEffect : public Effect
{
public:
	UnlitTexEffect(ID3D10Device* pDevice, ID3D10Effect* effect);
	virtual ~UnlitTexEffect(void);

    void SetWorldViewProjection(const Matrix& wvp);
	void SetDiffuseMap(const Texture2D* map);
        
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:

    ID3D10EffectMatrixVariable* m_pMtxWVP;
    ID3D10EffectShaderResourceVariable* m_pDiffuseMap;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};

