#pragma once

#include "D3DUtil.h"
#include "InstancingManager.h"
#include "PhysX.h"

#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "FluidRenderer.h"

#include "Blox2D.h"
#include "RenderContext.h"
#include "PostProcessor.h"

#include "EdgeDetectionPostEffect.h"
#include "PreShadowEffect.h"

#include "BaseGrid.h"

#include "ILevelObject.h"

#include <map>
#include "Trigger.h"

#include "FollowCamera.h"


class Editor;
class CharacterController;

class Level
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Level(ID3D10Device* pDXDevice);
	virtual ~Level();

	// GENERAL
	void Initialize(PhysX* pPhysXEngine, Editor* pGUI, Graphics::Camera::FollowCamera* pTrackingCamera);
	void Tick(const float dTime);
	void DrawDeferred(const RenderContext* pRenderContext);
	void DrawForward(const RenderContext* pRenderContext);
    void DrawShadowMap(const RenderContext* pRenderContext);

	void AddLevelObject(ILevelObject* pLevelObject);
    void RemoveLevelObject(ILevelObject* pLevelObject);
    void AddLight(Light* pLight);
    void RemoveLight(Light* pLight);

    void WakeUpAll();

    // SERIALISATION
    void Serialize(const string& path) const ;
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
	map<tstring, Trigger*>& GetTriggers()
	{ return m_pTriggers; }

	bool Triggered(const tstring& triggerName);

private:

	// DATAMEMBERS
	ID3D10Device* m_pDXDevice;

	PhysX* m_pPhysXEngine;
	const RenderContext* m_pRenderContext;

	vector<ILevelObject*> m_pLevelObjects;
	vector<IDrawable*> m_pDrawableObjects;
	map<tstring, Trigger*> m_pTriggers;
	Instancing::InstancingManager* m_pInstancingManager;

	BaseGrid* m_pBaseGrid;

	bool m_bShowGrid;
	bool m_bTickCharacter;
	bool m_bEditor;

    FluidRenderer* m_pFluidRenderer;

    Editor* m_pGUI;
    CharacterController* m_pCharacterController;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

