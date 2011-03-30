#pragma once

#include "Blox2D.h"
#include "Vector3.h"
#include "RenderContext.h"

class MoveGizmo
{
public:

	enum TYPE
	{
		TYPE_POINTLIGHT = 0,
		TYPE_SPOTLIGHT = 1,
		TYPE_DIRECTIONALLIGHT = 2,
		TYPE_MODEL = 3
	};

	// CONSTRUCTOR - DESTRUCTOR
	MoveGizmo();
	virtual ~MoveGizmo();

	// GENERAL
	void Show(Vector3& position, TYPE type, int id);
	void Tick(const RenderContext* pRenderContext);

private:

	// DATAMEMBERS
	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;

	vector<D3DXVECTOR3> m_OldPointLightPos;
	vector<D3DXVECTOR3> m_OldSpotLightPos;

	const RenderContext* m_pRenderContext;
};

