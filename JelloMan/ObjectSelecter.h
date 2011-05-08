#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include "Controls.h"
#include "ILevelObject.h"
#include "RenderContext.h"
#include "PhysX.h"
#include "Actor.h"

class ObjectSelecter
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	ObjectSelecter(PhysX* pPhysXEngine);
	virtual ~ObjectSelecter();

	// GENERAL
    void Tick(const RenderContext* pRenderContext);
	void Draw(const RenderContext* pRenderContext);
	void DeselectAll();
    void AbortControls();
    void CalcCenterPos();

	// GETTERS
    vector<IEditorObject*> GetSelectedObjects() { return m_SelectedObjects; }
    const Vector3& GetCenterPos() const { return m_vCenterPos; }

private:

    bool TrySelectLight(const RenderContext* pRenderContext);
	bool TrySelectObject(const RenderContext* pRenderContext);
	void CheckControls(const RenderContext* pRenderContext);

	// DATAMEMBERS
	bool m_bClick;

    D3D10_VIEWPORT m_ViewPort;

	const PhysX* m_pPhysXEngine;

    Vector3 m_vCenterPos;

    vector<IEditorObject*> m_SelectedObjects;
};