#pragma once

#include "D3DUtil.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "FluidRenderer.h"
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
#include "FluidsCharacter.h"


#include "FollowCamera.h"

#include "InstancingManager.h"

class EditorGUI;

class Level
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Level(ID3D10Device* pDXDevice);
	virtual ~Level();

	// GENERAL
	void Initialize(PhysX* pPhysXEngine, EditorGUI* pGUI, Graphics::Camera::FollowCamera* pTrackingCamera);
	void Tick(const float dTime);
	void DrawDeferred(RenderContext* pRenderContext);
	void DrawForward(RenderContext* pRenderContext);
    void DrawShadowMap(RenderContext* pRenderContext);

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
	FluidsCharacter* m_pFluidsCharacter;

	PhysX* m_pPhysXEngine;

	const RenderContext* m_pRenderContext;

	vector<ILevelObject*> m_pLevelObjects;

	vector<IDrawable*> m_pDrawableObjects;

	Instancing::InstancingManager* m_pInstancingManager;

	BaseGrid* m_pBaseGrid;

	bool m_bShowGrid;
	bool m_bTickCharacter;
	bool m_bEditor;

    FluidRenderer* m_pFluidRenderer;

    EditorGUI* m_pGUI;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

