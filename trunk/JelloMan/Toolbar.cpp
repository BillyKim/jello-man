#include "Toolbar.h"
#include "Level.h"
#include "ContentManager.h"
#include "EditorLoader.h"

/* CONSTRUCTOR - DESTRUCTOR */
Toolbar::Toolbar(Editor* pEditor, Infobar* pInfobar, EditorLoader* pLoader) :	m_pEditor(pEditor),
																				m_pInfobar(pInfobar),
																				m_GizmoMode(GIZMO_MODE_NONE),
																				m_EditorState(EDITOR_STATE_EDITOR),
																				m_bGameModeDown(false),
																				m_bEditorModeDown(false),
																				m_bPlayModeDown(false),
																				m_LightMode(LIGHT_MODE_UNLIT),
																				m_GridMode(GRID_MODE_ON),
																				m_SnappingOptions(SNAPPING_OPTIONS_NACTIVE),
																				m_PostEffectsState(POST_EFFECTS_OFF),
																				m_LoadState(LOAD_STATE_NONE),
																				m_pLoader(pLoader)
{
}

Toolbar::~Toolbar()
{
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		delete p.second;
	});
}

/* GENERAL */
void Toolbar::LoadButtons()
{
	// LIGHT BUTTON
	m_Buttons["LIGHT"] = new Button(76,7,36,36,true);

	m_ButtonImages["LIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_on_normal.png")));
	m_ButtonImages["LIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_on_hover.png")));
	m_ButtonImages["LIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_off_normal.png")));
	m_ButtonImages["LIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_off_hover.png")));

	m_Buttons["LIGHT"]->SetNormalState(m_ButtonImages["LIGHT"][0]);
	m_Buttons["LIGHT"]->SetHoverState(m_ButtonImages["LIGHT"][1]);
	m_Buttons["LIGHT"]->SetDownState(m_ButtonImages["LIGHT"][1]);
	m_Buttons["LIGHT"]->SetDeactivatedState(m_ButtonImages["LIGHT"][2]);
	m_Buttons["LIGHT"]->SetDeactivatedStateHover(m_ButtonImages["LIGHT"][3]);
	m_Buttons["LIGHT"]->SetDeactivatedStateDown(m_ButtonImages["LIGHT"][3]);

	m_Buttons["LIGHT"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["LIGHT"], _T("Switch between lit and unlit mode."));

	// MOVE BUTTON
	m_Buttons["MOVE"] = new Button(168,7,36,36,true);

	m_ButtonImages["MOVE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/move_on_normal.png")));
	m_ButtonImages["MOVE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/move_on_hover.png")));
	m_ButtonImages["MOVE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/move_off_normal.png")));
	m_ButtonImages["MOVE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/move_off_hover.png")));

	m_Buttons["MOVE"]->SetNormalState(m_ButtonImages["MOVE"][0]);
	m_Buttons["MOVE"]->SetHoverState(m_ButtonImages["MOVE"][1]);
	m_Buttons["MOVE"]->SetDownState(m_ButtonImages["MOVE"][1]);
	m_Buttons["MOVE"]->SetDeactivatedState(m_ButtonImages["MOVE"][2]);
	m_Buttons["MOVE"]->SetDeactivatedStateHover(m_ButtonImages["MOVE"][3]);
	m_Buttons["MOVE"]->SetDeactivatedStateDown(m_ButtonImages["MOVE"][3]);

	m_Buttons["MOVE"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["MOVE"], _T("Use the move gizmo to move objects and lights present in the scene."));

	// GAME MODE BUTTON
	m_Buttons["GAME"] = new Button(20,7,36,36);

	m_ButtonImages["GAME"].push_back(Content->LoadImage(_T("../Content/Images/Editor/game_mode_normal.png")));
	m_ButtonImages["GAME"].push_back(Content->LoadImage(_T("../Content/Images/Editor/game_mode_hover.png")));

	m_Buttons["GAME"]->SetNormalState(m_ButtonImages["GAME"][0]);
	m_Buttons["GAME"]->SetHoverState(m_ButtonImages["GAME"][1]);
	m_Buttons["GAME"]->SetDownState(m_ButtonImages["GAME"][1]);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["GAME"], _T("Switch between game - editor - play mode."));

	// EDITOR MODE BUTTON
	m_Buttons["EDITOR"] = new Button(20,43,36,36);

	m_ButtonImages["EDITOR"].push_back(Content->LoadImage(_T("../Content/Images/Editor/editor_mode_normal.png")));
	m_ButtonImages["EDITOR"].push_back(Content->LoadImage(_T("../Content/Images/Editor/editor_mode_hover.png")));

	m_Buttons["EDITOR"]->SetNormalState(m_ButtonImages["EDITOR"][0]);
	m_Buttons["EDITOR"]->SetHoverState(m_ButtonImages["EDITOR"][1]);
	m_Buttons["EDITOR"]->SetDownState(m_ButtonImages["EDITOR"][1]);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["EDITOR"], _T("Switch between game - editor - play mode."));

	// POINTLIGHT BUTTON
	m_Buttons["POINTLIGHT"] = new Button(352,7,36,36);

	m_ButtonImages["POINTLIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/pointlight_normal.png")));
	m_ButtonImages["POINTLIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/pointlight_hover.png")));

	m_Buttons["POINTLIGHT"]->SetNormalState(m_ButtonImages["POINTLIGHT"][0]);
	m_Buttons["POINTLIGHT"]->SetHoverState(m_ButtonImages["POINTLIGHT"][1]);
	m_Buttons["POINTLIGHT"]->SetDownState(m_ButtonImages["POINTLIGHT"][1]);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["POINTLIGHT"], _T("Add a pointlight to the scene."));

	// SPOTLIGHT BUTTON
	m_Buttons["SPOTLIGHT"] = new Button(388,7,36,36);

	m_ButtonImages["SPOTLIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/spotlight_normal.png")));
	m_ButtonImages["SPOTLIGHT"].push_back(Content->LoadImage(_T("../Content/Images/Editor/spotlight_hover.png")));

	m_Buttons["SPOTLIGHT"]->SetNormalState(m_ButtonImages["SPOTLIGHT"][0]);
	m_Buttons["SPOTLIGHT"]->SetHoverState(m_ButtonImages["SPOTLIGHT"][1]);
	m_Buttons["SPOTLIGHT"]->SetDownState(m_ButtonImages["SPOTLIGHT"][1]);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["SPOTLIGHT"], _T("Add a spotlight to the scene."));

	// ROTATE BUTTON
	m_Buttons["ROTATE"] = new Button(204,7,36,36,true);

	m_ButtonImages["ROTATE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_on_normal.png")));
	m_ButtonImages["ROTATE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_on_hover.png")));
	m_ButtonImages["ROTATE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_off_normal.png")));
	m_ButtonImages["ROTATE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_off_hover.png")));

	m_Buttons["ROTATE"]->SetNormalState(m_ButtonImages["ROTATE"][0]);
	m_Buttons["ROTATE"]->SetHoverState(m_ButtonImages["ROTATE"][1]);
	m_Buttons["ROTATE"]->SetDownState(m_ButtonImages["ROTATE"][1]);
	m_Buttons["ROTATE"]->SetDeactivatedState(m_ButtonImages["ROTATE"][2]);
	m_Buttons["ROTATE"]->SetDeactivatedStateHover(m_ButtonImages["ROTATE"][3]);
	m_Buttons["ROTATE"]->SetDeactivatedStateDown(m_ButtonImages["ROTATE"][3]);

	m_Buttons["ROTATE"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["ROTATE"], _T("Use the rotate gizmo to rotate objects and lights present in the scene."));

	// SCALE BUTTON
	m_Buttons["SCALE"] = new Button(240,7,36,36,true);

	m_ButtonImages["SCALE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_on_normal.png")));
	m_ButtonImages["SCALE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_on_hover.png")));
	m_ButtonImages["SCALE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_off_normal.png")));
	m_ButtonImages["SCALE"].push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_off_hover.png")));

	m_Buttons["SCALE"]->SetNormalState(m_ButtonImages["SCALE"][0]);
	m_Buttons["SCALE"]->SetHoverState(m_ButtonImages["SCALE"][1]);
	m_Buttons["SCALE"]->SetDownState(m_ButtonImages["SCALE"][1]);
	m_Buttons["SCALE"]->SetDeactivatedState(m_ButtonImages["SCALE"][2]);
	m_Buttons["SCALE"]->SetDeactivatedStateHover(m_ButtonImages["SCALE"][3]);
	m_Buttons["SCALE"]->SetDeactivatedStateDown(m_ButtonImages["SCALE"][3]);

	m_Buttons["SCALE"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["SCALE"], _T("DISABLED!"));

	// SNAPPING BUTTON
	m_Buttons["SNAPPING"] = new Button(296,7,36,36,true);

	m_ButtonImages["SNAPPING"].push_back(Content->LoadImage(_T("../Content/Images/Editor/snapping_normal.png")));
	m_ButtonImages["SNAPPING"].push_back(Content->LoadImage(_T("../Content/Images/Editor/snapping_hover.png")));

	m_Buttons["SNAPPING"]->SetNormalState(m_ButtonImages["SNAPPING"][1]);
	m_Buttons["SNAPPING"]->SetHoverState(m_ButtonImages["SNAPPING"][1]);
	m_Buttons["SNAPPING"]->SetDownState(m_ButtonImages["SNAPPING"][1]);
	m_Buttons["SNAPPING"]->SetDeactivatedState(m_ButtonImages["SNAPPING"][0]);
	m_Buttons["SNAPPING"]->SetDeactivatedStateHover(m_ButtonImages["SNAPPING"][0]);
	m_Buttons["SNAPPING"]->SetDeactivatedStateDown(m_ButtonImages["SNAPPING"][0]);

	m_Buttons["SNAPPING"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["SNAPPING"], _T("View the snapping options for the gizmo's."));

	// PLAY MODE BUTTON
	m_Buttons["PLAY"] = new Button(20,79,36,36);
	
	m_ButtonImages["PLAY"].push_back(Content->LoadImage(_T("../Content/Images/Editor/play_mode_normal.png")));
	m_ButtonImages["PLAY"].push_back(Content->LoadImage(_T("../Content/Images/Editor/play_mode_hover.png")));

	m_Buttons["PLAY"]->SetNormalState(m_ButtonImages["PLAY"][0]);
	m_Buttons["PLAY"]->SetHoverState(m_ButtonImages["PLAY"][1]);
	m_Buttons["PLAY"]->SetDownState(m_ButtonImages["PLAY"][1]);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["PLAY"], _T("Switch between game - editor - play mode."));

	// LOAD MODEL BUTTON
	m_Buttons["LOADMODEL"] = new Button(444,7,36,36, true);

	m_ButtonImages["LOADMODEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_off_normal.png")));
	m_ButtonImages["LOADMODEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_off_hover.png")));
	m_ButtonImages["LOADMODEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_on_normal.png")));
	m_ButtonImages["LOADMODEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_on_hover.png")));

	m_Buttons["LOADMODEL"]->SetDeactivatedState(m_ButtonImages["LOADMODEL"][0]);
	m_Buttons["LOADMODEL"]->SetDeactivatedStateHover(m_ButtonImages["LOADMODEL"][1]);
	m_Buttons["LOADMODEL"]->SetDeactivatedStateDown(m_ButtonImages["LOADMODEL"][1]);

	m_Buttons["LOADMODEL"]->SetNormalState(m_ButtonImages["LOADMODEL"][2]);
	m_Buttons["LOADMODEL"]->SetHoverState(m_ButtonImages["LOADMODEL"][3]);
	m_Buttons["LOADMODEL"]->SetDownState(m_ButtonImages["LOADMODEL"][3]);

	m_Buttons["LOADMODEL"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["LOADMODEL"], _T("Load a new levelobject from a file."));

	// LOAD LEVEL BUTTON
	m_Buttons["LOADLEVEL"] = new Button(480,7,36,36, true);

	m_ButtonImages["LOADLEVEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_off_normal.png")));
	m_ButtonImages["LOADLEVEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_off_hover.png")));
	m_ButtonImages["LOADLEVEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_on_normal.png")));
	m_ButtonImages["LOADLEVEL"].push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_on_hover.png")));

	m_Buttons["LOADLEVEL"]->SetDeactivatedState(m_ButtonImages["LOADLEVEL"][0]);
	m_Buttons["LOADLEVEL"]->SetDeactivatedStateHover(m_ButtonImages["LOADLEVEL"][1]);
	m_Buttons["LOADLEVEL"]->SetDeactivatedStateDown(m_ButtonImages["LOADLEVEL"][1]);

	m_Buttons["LOADLEVEL"]->SetNormalState(m_ButtonImages["LOADLEVEL"][2]);
	m_Buttons["LOADLEVEL"]->SetHoverState(m_ButtonImages["LOADLEVEL"][3]);
	m_Buttons["LOADLEVEL"]->SetDownState(m_ButtonImages["LOADLEVEL"][3]);

	m_Buttons["LOADLEVEL"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["LOADLEVEL"], _T("load or save a level to a file."));

	// SHOW GRID BUTTON
	m_Buttons["GRID"] = new Button(544,7,36,36,true);

	m_ButtonImages["GRID"].push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_on_normal.png")));
	m_ButtonImages["GRID"].push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_on_hover.png")));
	m_ButtonImages["GRID"].push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_off_normal.png")));
	m_ButtonImages["GRID"].push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_off_hover.png")));

	m_Buttons["GRID"]->SetNormalState(m_ButtonImages["GRID"][0]);
	m_Buttons["GRID"]->SetHoverState(m_ButtonImages["GRID"][1]);
	m_Buttons["GRID"]->SetDownState(m_ButtonImages["GRID"][1]);

	m_Buttons["GRID"]->SetDeactivatedState(m_ButtonImages["GRID"][2]);
	m_Buttons["GRID"]->SetDeactivatedStateHover(m_ButtonImages["GRID"][3]);
	m_Buttons["GRID"]->SetDeactivatedStateDown(m_ButtonImages["GRID"][3]);

	m_Buttons["GRID"]->SetState(Button::STATE_NORMAL);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["GRID"], _T("Turns the basegrid on/off."));

	// POST EFFECTS BUTTON
	m_Buttons["POSTFX"] = new Button(112,7,36,36,true);

	m_ButtonImages["POSTFX"].push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_on_normal.png")));
	m_ButtonImages["POSTFX"].push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_on_hover.png")));
	m_ButtonImages["POSTFX"].push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_off_normal.png")));
	m_ButtonImages["POSTFX"].push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_off_hover.png")));

	m_Buttons["POSTFX"]->SetNormalState(m_ButtonImages["POSTFX"][0]);
	m_Buttons["POSTFX"]->SetHoverState(m_ButtonImages["POSTFX"][1]);
	m_Buttons["POSTFX"]->SetDownState(m_ButtonImages["POSTFX"][1]);

	m_Buttons["POSTFX"]->SetDeactivatedState(m_ButtonImages["POSTFX"][2]);
	m_Buttons["POSTFX"]->SetDeactivatedStateHover(m_ButtonImages["POSTFX"][3]);
	m_Buttons["POSTFX"]->SetDeactivatedStateDown(m_ButtonImages["POSTFX"][3]);

	m_Buttons["POSTFX"]->SetState(Button::STATE_DEACTIVATED);

	m_pInfobar->AddButtonHoverMessage(m_Buttons["POSTFX"], _T("Turns on/off post processing effects."));
}

void Toolbar::Tick()
{
	// buttons
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		p.second->Tick();
	});

	// gizmo buttons
	vector<Button*> gizmos;
	gizmos.push_back(m_Buttons["MOVE"]);
	gizmos.push_back(m_Buttons["ROTATE"]);
	gizmos.push_back(m_Buttons["SCALE"]);

	m_GizmoMode = GIZMO_MODE_NONE;

	FlipButtons(gizmos);

	for (unsigned int i = 0; i < gizmos.size(); ++i)
	{
		if (gizmos[i]->IsActive())
			m_GizmoMode = static_cast<GIZMO_MODE>(i+1);
	}

	// load buttons
	vector<Button*> loaders;
	loaders.push_back(m_Buttons["LOADMODEL"]);
	loaders.push_back(m_Buttons["LOADLEVEL"]);

	m_LoadState = LOAD_STATE_NONE;

	FlipButtons(loaders);

	for (unsigned int i = 0; i < loaders.size(); ++i)
	{
		if (loaders[i]->IsActive())
			m_LoadState= static_cast<LOAD_STATE>(i+1);
	}

	// grid
	m_GridMode = m_Buttons["GRID"]->IsActive() ? GRID_MODE_ON : GRID_MODE_OFF;

	// snapping options
	m_SnappingOptions = m_Buttons["SNAPPING"]->IsActive() ? SNAPPING_OPTIONS_ACTIVE : SNAPPING_OPTIONS_NACTIVE;

	// lights
	m_LightMode = m_Buttons["LIGHT"]->IsActive() ? LIGHT_MODE_LIT : LIGHT_MODE_UNLIT;

	// post fx
	m_PostEffectsState = m_Buttons["POSTFX"]->IsActive() ? POST_EFFECTS_ON : POST_EFFECTS_OFF;

	// add lights
	if (m_Buttons["POINTLIGHT"]->Clicked())
		m_pLoader->AddPointLight();

	if (m_Buttons["SPOTLIGHT"]->Clicked())
		m_pLoader->AddSpotLight();
}

void Toolbar::Draw()
{
	// background
	BX2D->SetColor(70, 70, 70);
	BX2D->FillRect(0, 0, BX2D->GetWindowSize().width, 50);

	BX2D->SetColor(90, 90, 90);
	BX2D->DrawLine(0, 50, BX2D->GetWindowSize().width, 50);

	// buttons
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		if (p.first != "EDITOR" && p.first != "GAME" && p.first != "PLAY")
			p.second->Show();
	});

	// mode buttons
	ModeButtons();
}

void Toolbar::DeactivateGizmos()
{
	m_Buttons["MOVE"]->Deactivate();
	m_Buttons["ROTATE"]->Deactivate();
	m_Buttons["SCALE"]->Deactivate();
}

void Toolbar::DeactivateLoaders()
{
	m_Buttons["LOADLEVEL"]->Deactivate();
	m_Buttons["LOADMODEL"]->Deactivate();
}

void Toolbar::DeactivateEditorButtons()
{
	DeactivateGizmos();
	DeactivateLoaders();

	m_Buttons["SNAPPING"]->Deactivate();
}

void Toolbar::FlipButtons(vector<Button*>&  buttons)
{
	bool bClicked(false);

	for_each(buttons.begin(), buttons.end(), [&](Button* pButton)
	{
		if (pButton->Clicked())
			bClicked = true;
	});

	if (bClicked)
	{
		for (unsigned int i = 0; i < buttons.size(); ++i)
		{
			if (!buttons[i]->Clicked())
			{
				buttons[i]->Deactivate();
			}
		}
	}
}

void Toolbar::ModeButtons()
{
	if (m_EditorState == EDITOR_STATE_GAME)
	{
		m_Buttons["GAME"]->SetPosition(20, 7);
		m_Buttons["EDITOR"]->SetPosition(20, 43);
		m_Buttons["PLAY"]->SetPosition(20,79);

		if (CONTROLS->LeftMBUp() && m_bGameModeDown)
		{
			if (m_Buttons["GAME"]->Hover())
			{
				m_EditorState = EDITOR_STATE_GAME;
			}
			else if (m_Buttons["EDITOR"]->Hover())
			{
				m_EditorState = EDITOR_STATE_EDITOR;
			}
			else if (m_Buttons["PLAY"]->Hover())
			{
				m_EditorState = EDITOR_STATE_PLAY;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bGameModeDown)
		{
			m_Buttons["GAME"]->Show();
			m_Buttons["EDITOR"]->Show();
			m_Buttons["PLAY"]->Show();
		}
		else
		{
			m_Buttons["GAME"]->Show();

			m_bGameModeDown = false;
		}

		if (m_Buttons["GAME"]->Down())
		{
			m_bGameModeDown = true;
		}
	}
	else if (m_EditorState == EDITOR_STATE_EDITOR)
	{
		m_Buttons["EDITOR"]->SetPosition(20,7);
		m_Buttons["GAME"]->SetPosition(20,79);
		m_Buttons["PLAY"]->SetPosition(20,43);

		if (CONTROLS->LeftMBUp() && m_bEditorModeDown)
		{
			if (m_Buttons["EDITOR"]->Hover())
			{
				m_EditorState = EDITOR_STATE_EDITOR;
			}
			else if (m_Buttons["GAME"]->Hover())
			{
				m_EditorState = EDITOR_STATE_GAME;
			}
			else if (m_Buttons["PLAY"]->Hover())
			{
				m_EditorState = EDITOR_STATE_PLAY;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bEditorModeDown == true)
		{
			m_Buttons["GAME"]->Show();
			m_Buttons["EDITOR"]->Show();
			m_Buttons["PLAY"]->Show();
		}
		else
		{
			m_Buttons["EDITOR"]->Show();

			m_bEditorModeDown = false;
		}

		if (m_Buttons["EDITOR"]->Down())
		{
			m_bEditorModeDown = true;
		}
	}
	else if (m_EditorState == EDITOR_STATE_PLAY)
	{
		m_Buttons["EDITOR"]->SetPosition(20,43);
		m_Buttons["GAME"]->SetPosition(20,79);
		m_Buttons["PLAY"]->SetPosition(20,7);

		if (CONTROLS->LeftMBUp() && m_bPlayModeDown)
		{
			if (m_Buttons["EDITOR"]->Hover())
			{
				m_EditorState = EDITOR_STATE_EDITOR;
			}
			else if (m_Buttons["GAME"]->Hover())
			{
				m_EditorState = EDITOR_STATE_GAME;
			}
			else if (m_Buttons["PLAY"]->Hover())
			{
				m_EditorState = EDITOR_STATE_PLAY;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bPlayModeDown == true)
		{
			m_Buttons["GAME"]->Show();
			m_Buttons["EDITOR"]->Show();
			m_Buttons["PLAY"]->Show();
		}
		else
		{
			m_Buttons["PLAY"]->Show();

			m_bPlayModeDown = false;
		}

		if (m_Buttons["PLAY"]->Down())
		{
			m_bPlayModeDown = true;
		}
	}
}