#pragma once

#include "D3DUtil.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "TestObject.h"
#include "TestObject2.h"
#include "Blox2D.h"
#include "RenderContext.h"
#include "PostProcessor.h"
#include "EdgeDetectionPostEffect.h"
#include "Character.h"
#include "PhysX.h"
#include "LevelObject.h"
#include "BaseGrid.h"

class Level
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Level(ID3D10Device* pDXDevice);
	virtual ~Level();

	// GENERAL
	void Initialize(PhysX* pPhysXEngine, Camera* pTrackingCamera);
	void Tick(const float dTime);
	void DrawDeferred(const RenderContext* pRenderContext);
	void DrawForward(const RenderContext* pRenderContext);

	void AddLevelObject(LevelObject* pLevelObject);

	// SETTERS
	void ShowGrid(bool show)
	{ m_bShowGrid = show; }

	void TickCharacter(bool tick)
	{ m_bTickCharacter = tick; }

private:

	// DATAMEMBERS
	ID3D10Device* m_pDXDevice;

    TestObject* m_pTestObject;
    TestObject2* m_pTestObject2;
	vector<Actor*> m_vecActor;
	Character* m_pCharacter;

	PhysX* m_pPhysXEngine;

	const RenderContext* m_pRenderContext;

	vector<LevelObject*> m_pLevelObjects;

	BaseGrid* m_pBaseGrid;

	bool m_bShowGrid;
	bool m_bTickCharacter;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

