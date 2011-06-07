#pragma once

#include "Blox2D.h"
#include "Vector3.h"
#include "RenderContext.h"
#include "ITransformable.h"
#include "ILevelObject.h"
#include "ObjectSelecter.h"
#include "Axis.h"

class MoveGizmo
{
public:
	// CONSTRUCTOR - DESTRUCTOR
	MoveGizmo(ObjectSelecter* pObjectSelecter);
	virtual ~MoveGizmo();

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

	void SnapToGrid(bool bSnap)
	{ m_bSnapToGrid = bSnap; }

private:
    static const int END_ELLIPSE_RADIUS = 5;
    static const int AXIS_LENGTH = 100, AXIS_SMALL_LENGTH = 50;

    // DRAW
    void DrawAxis(const Vector3& pos, Axis axis,
                  const Matrix& world, const Matrix& view, const Matrix& projection);
    void Draw2Axis(const Vector3& pos, Axis axis1, Axis axis2,
                  const Matrix& world, const Matrix& view, const Matrix& projection);
    void DrawGizmo(const Vector3& pos);

    // CONTROLS
    void CheckControls();
    bool PolyCollisionCheck(const Vector3& pos, const Vector3& axis1, const Vector3& axis2, 
                            const Matrix& proj, const Matrix& view, const Matrix& world);

	// DATAMEMBERS
	const RenderContext* m_pRenderContext;

	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;
	bool m_bSnap;
	bool m_bSnapToGrid;
    bool m_bCanCopy;

	float m_SnapSize;

    D3D10_VIEWPORT m_ViewPort;

	TextFormat* m_pAxisFont;

    Vector3 m_vCenterPos;
    Vector3 m_vAnchor;

	ObjectSelecter* m_pObjectSelecter;
};

