#pragma once

#include "Blox2D.h"
#include "Controls.h"
#include <vector>
#include "RenderContext.h"
#include "LightController.h"
#include "VisualLightDebugger.h"
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

class EditorGUI
{
public:

	enum MODE
	{
		MODE_EDITOR = 0,
		MODE_GAME = 1,
		MODE_PLAY = 2
	};

	// CONSTRUCTOR - DESTRUCTOR
	EditorGUI(PhysX* pPhysXEngine, ID3D10Device* pDXDevice, Level* pLevel);
	virtual ~EditorGUI();

	// GENERAL
	void Initialize();
	void Draw();
	void Tick(const RenderContext* pRenderContext);

	// GETTERS
	Button* GetLightButton() const
	{ return m_pLightButton; }

	Button* GetMoveButton() const
	{ return m_pMoveButton; }

	Button* GetPointlightButton() const
	{ return m_pPointlightButton; }

	Button* GetPlayButton() const
	{ return m_pPlayModeButton; }

	Button* GetLoadModelButton() const
	{ return m_pLoadModelButton; }

	Button* GetShowGridButton() const
	{ return m_pShowGridButton; }

	Button* GetPostFXButton() const
	{ return m_pPostFXButton; }

	MODE GetMode() const
	{ return m_Mode; }

	ILevelObject* GetNewLevelObject();

	bool NewModelLoaded() const
	{ return m_bNewModelLoaded; }

private:

	// DATAMEMBERS

	// BUTTONS
	Button* m_pLightButton;
	vector<Image*> m_pLightButtonBitmaps;
	Button* m_pMoveButton;
	vector<Image*> m_pMoveButtonBitmaps;
	Button* m_pGameModeButton;
	vector<Image*> m_pGameModeButtonBitmaps;
	Button* m_pEditorModeButton;
	vector<Image*> m_pEditorModeButtonBitmaps;
	Button* m_pPointlightButton;
	vector<Image*> m_pPointlightButtonBitmaps;
	Button* m_pSpotlightButton;
	vector<Image*> m_pSpotlightButtonBitmaps;
	Button* m_pColorPickerButton;
	vector<Image*> m_pColorPickerButtonBitmaps;
	Button* m_pApplyButton;
	vector<Image*> m_pApplyButtonBitmaps;
	Button* m_pRotateButton;
	vector<Image*> m_pRotateButtonBitmaps;
	Button* m_pPlayModeButton;
	vector<Image*> m_pPlayModeButtonBitmaps;
	Button* m_pLoadModelButton;
	vector<Image*> m_pLoadModelButtonBitmaps;
	Button* m_pLoadLevelButton;
	vector<Image*> m_pLoadLevelButtonBitmaps;
	Button* m_pShowGridButton;
	vector<Image*> m_pShowGridButtonBitmaps;
	Button* m_pPostFXButton;
	vector<Image*> m_pPostFXButtonBitmaps;
	Button* m_pScaleButton;
	vector<Image*> m_pScaleButtonBitmaps;
	Button* m_pSnappingButton;
	vector<Image*> m_pSnappingButtonBitmaps;

	Image* m_pCameraBitmap;

	bool m_bUsingCamera;

	bool m_bGameModeDown;
	bool m_bEditorModeDown;
	bool m_bPlayModeDown;

	bool m_bMoveable;
	bool m_bRotateable;
	bool m_bScaleable;

	bool m_bNewModelLoaded;

	const RenderContext* m_pRenderContext;

	VisualLightDebugger* m_pLightDebugger;
	ColorPicker* m_pColorPicker;
	MoveGizmo* m_pMoveGizmo;
	RotateGizmo* m_pRotateGizmo;
	ScaleGizmo* m_pScaleGizmo;
	Snapper* m_pSnapper;

	MODE m_Mode;

	LoadModelFromFile* m_pModelLoader;
	LoadLevelFromFile* m_pLevelLoader;

	PhysX* m_pPhysXEngine;

	TextFormat* m_pInfoFont;

    ObjectSelecter* m_pObjectSelecter;

	Level* m_pLevel;

	ID3D10Device* m_pDXDevice;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	EditorGUI(const EditorGUI& t);
	EditorGUI& operator=(const EditorGUI& t);
};

