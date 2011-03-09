#pragma once
#include "D3DUtil.h"
#include <vector>
#include "vertex.h"
#include "Effect.h"

class ModelMesh
{
public:
    ModelMesh(ID3D10Device* device, const tstring& name);
    virtual ~ModelMesh(void);

    void SetIndices(vector<DWORD> indices);
    void SetVertices(vector<VertexPosNormTex> vertices);
    void SetEffect(Effect* effect);

    Effect* GetEffect() { return m_pEffect; }

private:
    ID3D10Device* m_pDevice;

	ID3D10Buffer* m_pVertexBuffer;
	ID3D10Buffer* m_pIndexBuffer;

    UINT m_VertexBufferStride;
	vector<VertexPosNormTex> m_VecVertices;
	vector<DWORD> m_VecIndices;

    tstring m_Name;

    Effect* m_pEffect;
    ID3D10InputLayout* m_pInputLayout;
};

