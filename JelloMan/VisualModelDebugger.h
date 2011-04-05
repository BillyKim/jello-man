#pragma once

#include "D3DUtil.h"
#include "Blox2D_Engine.h"
#include "Controls.h"
#include "LevelObject.h"
#include "RenderContext.h"
#include "PhysX.h"

class VisualModelDebugger
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	VisualModelDebugger(PhysX* pPhysXEngine);
	virtual ~VisualModelDebugger();

	// GENERAL
	void Tick(const RenderContext* pRenderContex, vector<LevelObject*> pLevelObjects);
	void CheckControls();
	void Draw();
	void DeselectAll();

	// GETTERS
	vector<bool> GetModelsSelected() const
	{ return m_ModelsSelected; }

private:

	void RayCast();

	// DATAMEMBERS
	bool m_bClick;

	int m_TotalSelectedModels;
	int m_TotalModels;

	const RenderContext* m_pRenderContext;

	const PhysX* m_pPhysXEngine;

	vector<bool> m_ModelsSelected;
	vector<LevelObject*> m_pLevelObjects;

	vector<bool> m_ActorsSelected;
	vector<Actor*> m_pActors;
};