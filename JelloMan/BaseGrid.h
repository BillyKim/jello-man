#pragma once

#include "D3DUtil.h"
#include <vector>
#include "vertex.h"
#include "Effect.h"
#include "PhongEffect.h"
#include "RenderContext.h"

class BaseGrid
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	BaseGrid(ID3D10Device* device);
	virtual ~BaseGrid();

	// GENERAL
	void Init();
	void Draw(const RenderContext* pRenderContext);

private:

	void DefineInputLayout();
	void BuildVertexBuffer();

	// DATAMEMBERS
	ID3D10Device* m_pDevice;

	ID3D10Buffer* m_pVertexBuffer;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexBufferStride;

	vector<VertexPosCol> m_VecVertices;

	PhongEffect* m_pEffect;

	Matrix m_mtxWorld;
};

