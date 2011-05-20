#pragma once
#include "D3DUtil.h"
#include "vertex.h"

class Effect
{
public:
    //---------Constructor-&-Destructor------------------------->
	Effect(ID3D10Device* graphicsDevice, ID3D10Effect* effect);
	virtual ~Effect(void);
    //<----------------------------------------------------------

    void SetTechnique(int index);
    void SetTechnique(const string& name);

    ID3D10EffectTechnique* GetCurrentTechnique() { return m_pCurrentTechnique; }

    virtual ID3D10InputLayout* GetInputLayout() const = 0;
    virtual UINT GetVertexStride() const = 0;
    UINT GetNumPasses() const;

    ID3D10EffectVariable* GetVariableByName(const string& name) const;
    ID3D10EffectVariable* GetVariableBySemantic(const string& semantic) const;

protected:
	ID3D10Device* m_pDevice;
	ID3D10Effect* m_pEffect;
	ID3D10EffectTechnique* m_pCurrentTechnique;
    UINT m_NumPasses;

private:
	Effect(const Effect& effect);
	Effect& operator=(const Effect& effect);
};

template <typename T>
void CreateInputLayout(ID3D10Device* pDXDevice, Effect* pEffect, ID3D10InputLayout** ppInputLayout, UINT& vertexStride, bool useForInstancing = false)
{    
    SafeRelease(*ppInputLayout);

    // Define the input layout
    vector<D3D10_INPUT_ELEMENT_DESC> veclayout;
    UINT numElements;
    GetInputElementDesc<T>(veclayout, numElements, useForInstancing);

    D3D10_PASS_DESC passDesc;
    pEffect->GetCurrentTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

    HR(pDXDevice->CreateInputLayout(&veclayout[0], numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, ppInputLayout));

    vertexStride = sizeof(T);
}
