#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include "Infobar.h"
#include <vector>
#include <map>

class Editor;
class EditorLoader;

class Toolbar
{
public:

	enum GIZMO_MODE
	{
		GIZMO_MODE_NONE,
		GIZMO_MODE_MOVE,
		GIZMO_MODE_ROTATE,
		GIZMO_MODE_SCALE
	};

	enum EDITOR_STATE
	{
		EDITOR_STATE_EDITOR,
		EDITOR_STATE_GAME,
		EDITOR_STATE_PLAY
	};

	enum LIGHT_MODE
	{
		LIGHT_MODE_UNLIT,
		LIGHT_MODE_LIT
	};

	enum GRID_MODE
	{
		GRID_MODE_ON,
		GRID_MODE_OFF
	};

	enum SNAPPING_OPTIONS
	{
		SNAPPING_OPTIONS_NACTIVE,
		SNAPPING_OPTIONS_ACTIVE
	};

	enum POST_EFFECTS
	{
		POST_EFFECTS_OFF,
		POST_EFFECTS_ON
	};

	enum LOAD_STATE
	{
		LOAD_STATE_NONE,
		LOAD_STATE_MODEL,
		LOAD_STATE_LEVEL
	};

	/* CONSTRUCTOR - DESTRUCTOR */
	Toolbar(Editor* pEditor, Infobar* pInfobar, EditorLoader* pLoader);
	virtual ~Toolbar();

	/* GENERAL */
	void LoadButtons();
	void Tick();
	void Draw();

	void DeactivateGizmos();
	void DeactivateLoaders();
	void DeactivateEditorButtons();

	/* GETTERS */
	SNAPPING_OPTIONS GetSnappingOptionsState() const
	{ return m_SnappingOptions; }

	GIZMO_MODE GetGizmoState() const
	{ return m_GizmoMode; }

	EDITOR_STATE GetEditorState() const
	{ return m_EditorState; }

	LIGHT_MODE GetLightMode() const
	{ return m_LightMode; }

	GRID_MODE GetGridMode() const
	{ return m_GridMode; }

	POST_EFFECTS GetPostEffectsState() const
	{ return m_PostEffectsState; }

	LOAD_STATE GetLoadState() const
	{ return m_LoadState; }

private:

	void FlipButtons(vector<Button*>&  buttons);
	void ModeButtons();

	/* DATAMEMBERS */
	bool m_bGameModeDown;
	bool m_bEditorModeDown;
	bool m_bPlayModeDown;

	GIZMO_MODE m_GizmoMode;
	EDITOR_STATE m_EditorState;
	LIGHT_MODE m_LightMode;
	GRID_MODE m_GridMode;
	SNAPPING_OPTIONS m_SnappingOptions;
	POST_EFFECTS m_PostEffectsState;
	LOAD_STATE m_LoadState;

	Editor* m_pEditor;
	Infobar* m_pInfobar;
	EditorLoader* m_pLoader;

	map<string, Button*> m_Buttons;
	map<string, vector<Image*> > m_ButtonImages;

	// copy & assignment operator
	Toolbar(const Toolbar& second);
	Toolbar& operator=(const Toolbar& second);
};

