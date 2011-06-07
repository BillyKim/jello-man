#pragma once

#include "Blox2D.h"
#include "Vector3.h"
#include "RenderContext.h"
#include "ITransformable.h"
#include "ILevelObject.h"
#include "ObjectSelecter.h"
#include "Axis.h"

class ScaleGizmo
{
public:
	// CONSTRUCTOR - DESTRUCTOR
	ScaleGizmo(ObjectSelecter* pObjectSelecter);
	virtual ~ScaleGizmo();

	// GENERAL
	void Show(ITransformable* pLevelObject, int id);
    void Tick();

    // DRAW
    void Draw(const RenderContext* pRenderContext);

private:
    static const int END_ELLIPSE_RADIUS = 5;
    static const int AXIS_LENGTH = 100;

    // DRAW
    void DrawAxis(const Vector3& pos, Axis axis,
                  const Matrix& world, const Matrix& view, const Matrix& projection);
    void DrawGizmo(const Vector3& pos);

    // CONTROLS
    void CheckControls();

	// DATAMEMBERS
	const RenderContext* m_pRenderContext;

	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;

    D3D10_VIEWPORT m_ViewPort;

	TextFormat* m_pAxisFont;

    Vector3 m_vCenterPos;

	ObjectSelecter* m_pObjectSelecter;
};

