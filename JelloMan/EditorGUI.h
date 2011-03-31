#pragma once

#include "Blox2D.h"
#include "Controls.h"
#include <vector>
#include "Camera.h"
#include "RenderContext.h"
#include "LightController.h"
#include "VisualLightDebugger.h"
#include "ColorPicker.h"
#include "MoveGizmo.h"
#include "RotateGizmo.h"

class EditorGUI
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	EditorGUI();
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
	bool EditorMode() const;

private:

	// DATAMEMBERS

	// BUTTONS
	Button* m_pLightButton;
	vector<Bitmap*> m_pLightButtonBitmaps;
	Button* m_pMoveButton;
	vector<Bitmap*> m_pMoveButtonBitmaps;
	Button* m_pGameModeButton;
	vector<Bitmap*> m_pGameModeButtonBitmaps;
	Button* m_pEditorModeButton;
	vector<Bitmap*> m_pEditorModeButtonBitmaps;
	Button* m_pPointlightButton;
	vector<Bitmap*> m_pPointlightButtonBitmaps;
	Button* m_pSpotlightButton;
	vector<Bitmap*> m_pSpotlightButtonBitmaps;
	Button* m_pColorPickerButton;
	vector<Bitmap*> m_pColorPickerButtonBitmaps;
	Button* m_pApplyButton;
	vector<Bitmap*> m_pApplyButtonBitmaps;
	Button* m_pRotateButton;
	vector<Bitmap*> m_pRotateButtonBitmaps;

	Bitmap* m_pCameraBitmap;

	bool m_bUsingCamera;
	bool m_bGameModeDown;
	bool m_bEditorModeDown;

	bool m_bMoveable;
	bool m_bRotateable;

	int m_GameEditorModeSelect;

	const RenderContext* m_pRenderContext;

	VisualLightDebugger* m_pLightDebugger;
	ColorPicker* m_pColorPicker;
	MoveGizmo* m_pMoveGizmo;
	RotateGizmo* m_pRotateGizmo;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	EditorGUI(const EditorGUI& t);
	EditorGUI& operator=(const EditorGUI& t);
};
