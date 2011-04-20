#pragma once

#include "Blox2D_Engine.h"
#include "Vector3.h"
#include "RenderContext.h"
#include "ILevelObject.h"
#include "Light.h"

class MoveGizmo
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	MoveGizmo();
	virtual ~MoveGizmo();

	// GENERAL
	void Show(Light* pLight, int id);
	void Show(ILevelObject* pLevelObject, int id);
	void Tick(const RenderContext* pRenderContext, vector<ILevelObject*> pLevelObjects);

	// GETTERS
	bool IsMoving() const;

private:

	// DATAMEMBERS
	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;

	vector<D3DXVECTOR3> m_OldLightPos;
	vector<D3DXVECTOR3> m_OldModelPos;

	const RenderContext* m_pRenderContext;

	TextFormat* m_pAxisFont;
};

