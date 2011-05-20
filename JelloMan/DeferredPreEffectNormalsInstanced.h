#pragma once
#include "Effect.h"
#include "Matrix.h"
#include "Texture2D.h"

class DeferredPreEffectNormalsInstanced : public Effect
{
public:
    DeferredPreEffectNormalsInstanced(ID3D10Device* pDXDevice, ID3D10Effect* pEffect);
    virtual ~DeferredPreEffectNormalsInstanced(void);

    void SetViewProjection(const Matrix& wvp);

    void SetDiffuseMap(Texture2D* diffuseMap);
    void SetSpecMap(Texture2D* specMap);
    void SetGlossMap(Texture2D* glossMap);
	void SetNormalMap(Texture2D* normalMap);

	void Selected(bool selected);
    
    virtual ID3D10InputLayout* GetInputLayout() const;
    virtual UINT GetVertexStride() const;

private:
    ID3D10EffectMatrixVariable* m_pVP;

    ID3D10EffectShaderResourceVariable* m_pDiffuseMap;
    ID3D10EffectShaderResourceVariable* m_pSpecMap;
    ID3D10EffectShaderResourceVariable* m_pGlossMap;
	ID3D10EffectShaderResourceVariable* m_pNormalMap;

	ID3D10EffectScalarVariable* m_bSelected;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexStride;
};
