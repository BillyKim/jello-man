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

private:

	// DATAMEMBERS
	ID3D10Device* m_pDXDevice;

    TestObject* m_pTestObject;
    TestObject2* m_pTestObject2;
	vector<Actor*> m_vecActor;
	Character* m_pCharacter;

	PhysX* m_pPhysXEngine;

	const RenderContext* m_pRenderContext;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

