#pragma once

#include "D3DUtil.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "TestSoftbody.h"
#include "Blox2D.h"
#include "RenderContext.h"
#include "PostProcessor.h"
#include "EdgeDetectionPostEffect.h"
#include "PhysX.h"
#include "BaseGrid.h"
#include "Softbody.h"
#include "PreShadowEffect.h"
#include "ILevelObject.h"
#include "Fluid.h"
#include "SoftbodyCharacter.h"

#include "FluidPostEffect.h"

#include "FollowCamera.h"



class Level
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Level(ID3D10Device* pDXDevice);
	virtual ~Level();

	// GENERAL
	void Initialize(PhysX* pPhysXEngine, Graphics::Camera::FollowCamera* pTrackingCamera);
	void Tick(const float dTime);
	void DrawDeferred(RenderContext* pRenderContext);
	void DrawForward(const RenderContext* pRenderContext);
    void DrawShadowMap(RenderContext* pRenderContext, PreShadowEffect* pPreShadowEffect);

	void AddLevelObject(ILevelObject* pLevelObject);

    // SERIALISATION
    void Serialize(const string& path);
    void Deserialize(const string& path);
    void Clear();

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

	SoftbodyCharacter* m_pCharacter;

	PhysX* m_pPhysXEngine;

	const RenderContext* m_pRenderContext;

	vector<ILevelObject*> m_pLevelObjects;

	BaseGrid* m_pBaseGrid;

	bool m_bShowGrid;
	bool m_bTickCharacter;
	bool m_bEditor;

	Fluid* m_pTestFluid;
	NxFluidEmitter* m_pEmitter;

    PostProcessor* m_pFluidPostProcessor;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

