#pragma once

#include "D3DUtil.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "TestObject.h"
#include "TestObject2.h"
#include "TestSoftbody.h"
#include "Blox2D_Engine.h"
#include "RenderContext.h"
#include "PostProcessor.h"
#include "EdgeDetectionPostEffect.h"
#include "Character.h"
#include "PhysX.h"
#include "LevelObject.h"
#include "BaseGrid.h"
#include "Softbody.h"
#include "PreShadowEffect.h"

#include "ILevelObject.h"

class Level
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Level(ID3D10Device* pDXDevice);
	virtual ~Level();

	// GENERAL
	void Initialize(PhysX* pPhysXEngine, Camera* pTrackingCamera);
	void Tick(const float dTime);
	void DrawDeferred(RenderContext* pRenderContext);
	void DrawForward(const RenderContext* pRenderContext);
    void DrawShadowMap(RenderContext* pRenderContext, PreShadowEffect* pPreShadowEffect);

	void AddLevelObject(ILevelObject* pLevelObject);

	// SETTERS
	void ShowGrid(bool show)
	{ m_bShowGrid = show; }

	void EditorMode(bool editor)
	{ m_bEditor = editor; }

	void TickCharacter(bool tick)
	{ m_bTickCharacter = tick; }

	// GETTERS
	vector<ILevelObject*>& GetLevelObjects()
	{ return m_pLevelObjects; }

private:

	// DATAMEMBERS
	ID3D10Device* m_pDXDevice;

	Character* m_pCharacter;

	PhysX* m_pPhysXEngine;

	const RenderContext* m_pRenderContext;

	vector<ILevelObject*> m_pLevelObjects;

	BaseGrid* m_pBaseGrid;

	bool m_bShowGrid;
	bool m_bTickCharacter;
	bool m_bEditor;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

