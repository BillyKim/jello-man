#pragma once

#include "Blox2D.h"
#include "Controls.h"
#include <vector>
#include "RenderContext.h"
#include "LightController.h"
#include "LightInfo.h"
#include "ColorPicker.h"
#include "MoveGizmo.h"
#include "RotateGizmo.h"
#include "LoadModelFromFile.h"
#include "ILevelObject.h"
#include "ObjectSelecter.h"
#include "Level.h"
#include "LoadLevelFromFile.h"
#include "ScaleGizmo.h"
#include "Snapper.h"
#include "LevelObjectInfo.h"
#include "SimpleObject.h"

class Toolbar;
#include "Infopane.h"
class Infobar;
class EditorLoader;

class Editor
{
public:

	enum EDITOR_MODE
	{
		EDITOR_MODE_EDITOR,
		EDITOR_MODE_GAME,
		EDITOR_MODE_PLAY
	};

	enum LIGHT_MODE
	{
		LIGHT_MODE_UNLIT,
		LIGHT_MODE_LIT
	};

	enum POST_EFFECTS
	{
		POST_EFFECTS_OFF,
		POST_EFFECTS_ON
	};

	/* CONSTRUCTOR - DESTRUCTOR */
	Editor(PhysX* pPhysXEngine, ID3D10Device* pDXDevice, Level* pLevel);
	virtual ~Editor();

	/* GENERAL */
	void Initialize();
	void Tick(const RenderContext* pRenderContext);
	void Draw();

    void Clear();
	 
	/* GETTERS */
	EDITOR_MODE GetEditorMode() const
	{ return m_Mode; }

	LIGHT_MODE GetLightMode() const;

	POST_EFFECTS GetPostFXMode() const;

	SSAOSettings GetSSAOSettings() const;

private:

	void DrawSelectedObjects();

	/* DATAMEMBERS */
	Image* m_pCameraBitmap;

	bool m_bUsingCamera;

	bool m_bMoveable;
	bool m_bRotateable;
	bool m_bScaleable;

	const RenderContext* m_pRenderContext;

	MoveGizmo* m_pMoveGizmo;
	RotateGizmo* m_pRotateGizmo;
	ScaleGizmo* m_pScaleGizmo;
	Snapper* m_pSnapper;

	EDITOR_MODE m_Mode;

	PhysX* m_pPhysXEngine;

    ObjectSelecter* m_pObjectSelecter;

	Level* m_pLevel;

	ID3D10Device* m_pDXDevice;

	EditorLoader* m_pEditorLoader;

	// gui
	Toolbar* m_pToolbar;
	Infopane* m_pInfopane;
	Infobar* m_pInfobar;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Editor(const Editor& t);
	Editor& operator=(const Editor& t);
};

