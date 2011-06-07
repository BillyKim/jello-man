#pragma once

#include "Blox2D.h"
#include "Vector3.h"
#include "RenderContext.h"
#include "ITransformable.h"
#include "ILevelObject.h"
#include "ObjectSelecter.h"
#include "Axis.h"

class RotateGizmo
{
public:
	// CONSTRUCTOR - DESTRUCTOR
	RotateGizmo(ObjectSelecter* pObjectSelecter);
	virtual ~RotateGizmo();

	// GENERAL
	void Show(ITransformable* pLevelObject, int id);
    void Tick();

    // DRAW
    void Draw(const RenderContext* pRenderContext);

	// SETTERS
	void UseSnapping(bool bSnap)
	{ m_bSnap = bSnap; }

	void SetSnapSize(float snapSize)
	{ m_SnapSize = snapSize; }

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
	bool m_bSnap;

	float m_SnapSize;
	float m_Move;

    D3D10_VIEWPORT m_ViewPort;

	TextFormat* m_pAxisFont;

    Vector3 m_vCenterPos;

	ObjectSelecter* m_pObjectSelecter;
};

