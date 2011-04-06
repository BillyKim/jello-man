#pragma once

#include "Blox2D_Engine.h"
#include "Vector3.h"
#include "RenderContext.h"
#include "LevelObject.h"

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
	void Show(Vector3& position, TYPE type, int id, LevelObject* pLevelObject = 0);
	void Tick(const RenderContext* pRenderContext, vector<LevelObject*> pLevelObjects);

	// GETTERS
	bool IsMoving() const;

private:

	// DATAMEMBERS
	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;

	vector<D3DXVECTOR3> m_OldPointLightPos;
	vector<D3DXVECTOR3> m_OldSpotLightPos;
	vector<D3DXVECTOR3> m_OldModelPos;

	const RenderContext* m_pRenderContext;
};

