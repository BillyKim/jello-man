#pragma once

#include "D3DUtil.h"
#include <vector>
#include "vertex.h"
#include "Effect.h"
#include "PosColEffect.h"
#include "RenderContext.h"
#include "Model.h"

class BaseGrid
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	BaseGrid();
	virtual ~BaseGrid();

	// GENERAL
	void Init();
	void Draw(const RenderContext* pRenderContext);

private:

	// DATAMEMBERS
	Model<VertexPos>* m_pBaseGrid;
	Model<VertexPos>* m_pBaseGridCenter;

	PosColEffect* m_pEffect;

	Matrix m_mtxWorld;
};

