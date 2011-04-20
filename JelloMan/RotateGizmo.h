#pragma once

#include "Blox2D_Engine.h"
#include "Vector3.h"
#include "RenderContext.h"
#include "LevelObject.h"

class RotateGizmo
{
public:

	enum TYPE
	{
		TYPE_SPOTLIGHT = 0,
		TYPE_DIRECTIONALLIGHT = 1,
		TYPE_MODEL = 2
	};

	// CONSTRUCTOR - DESTRUCTOR
	RotateGizmo();
	virtual ~RotateGizmo();

	// GENERAL
	void Show(Vector3& position, Vector3& direction, TYPE type, int id, ILevelObject* pLevelObject = 0);
	void Tick(const RenderContext* pRenderContext, vector<ILevelObject*> pLevelObjects);

private:

	// DATAMEMBERS
	const RenderContext* m_pRenderContext;

	bool m_bLockXY;
	bool m_bLockYZ;
	bool m_bLockZX;

	vector<D3DXVECTOR3> m_OldLightPos;
	vector<D3DXVECTOR3> m_OldModelPos;

	vector<Vector3> m_OldModelRot;

	TextFormat* m_pAxisFont;
};

