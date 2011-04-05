#pragma once

#include "Blox2D_Engine.h"
#include "Vector3.h"
#include "RenderContext.h"

class RotateGizmo
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
	RotateGizmo();
	virtual ~RotateGizmo();

	// GENERAL
	void Show(Vector3& position, TYPE type, int id);
	void Tick(const RenderContext* pRenderContext);

private:

	// DATAMEMBERS
	const RenderContext* m_pRenderContext;
};

