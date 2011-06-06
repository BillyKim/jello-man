#include "EditorGUI.h"
#include "LightDesc.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
EditorGUI::EditorGUI(PhysX* pPhysXEngine, ID3D10Device* pDXDevice, Level* pLevel)
	:	m_pLightButton(0),
		m_pCameraBitmap(0),
		m_bUsingCamera(false),
		m_pMoveButton(0),
		m_pGameModeButton(0),
		m_pEditorModeButton(0),
		m_pPointlightButton(0),
		m_pSpotlightButton(0),
		m_bGameModeDown(false),
		m_bEditorModeDown(false),
		m_pRenderContext(0),
		m_bMoveable(false),
		m_pColorPickerButton(0),
		m_pApplyButton(0),
		m_pRotateButton(0),
		m_pLightDebugger(0),
		m_pColorPicker(0),
		m_pMoveGizmo(0),
		m_pRotateGizmo(0),
		m_pPlayModeButton(0),
		m_Mode(MODE_EDITOR),
		m_bPlayModeDown(false),
		m_pLoadModelButton(0),
		m_pModelLoader(0),
		m_bNewModelLoaded(false),
		m_pShowGridButton(0),
		m_pPhysXEngine(pPhysXEngine),
		m_pDXDevice(pDXDevice),
        m_pObjectSelecter(new ObjectSelecter(pLevel, pPhysXEngine)),
		m_pLoadLevelButton(0),
		m_pLevel(pLevel),
		m_pLevelLoader(0),
		m_pPostFXButton(0),
		m_pScaleButton(0),
		m_bScaleable(false),
		m_pScaleGizmo(0),
		m_pSnapper(0),
		m_pSnappingButton(0),
		m_pObjectInfo(0)
{

}

EditorGUI::~EditorGUI()
{
	delete m_pLightButton;
	delete m_pMoveButton;
	delete m_pGameModeButton;
	delete m_pEditorModeButton;
	delete m_pPointlightButton;
	delete m_pSpotlightButton;
	delete m_pColorPickerButton;
	delete m_pApplyButton;
	delete m_pRotateButton;
	delete m_pPlayModeButton;
	delete m_pLoadModelButton;
	delete m_pShowGridButton;
	delete m_pLoadLevelButton;
	delete m_pPostFXButton;
	delete m_pScaleButton;
	delete m_pSnappingButton;

	delete m_pLightDebugger;
	delete m_pColorPicker;
	delete m_pMoveGizmo;
	delete m_pRotateGizmo;
	delete m_pModelLoader;
	delete m_pObjectSelecter;
	delete m_pLevelLoader;
	delete m_pScaleGizmo;
	delete m_pSnapper;
	delete m_pObjectInfo;
}

// GENERAL
void EditorGUI::Initialize()
{
	// LIGHT BUTTON
	m_pLightButton = new Button(76,7,36,36,true);

	m_pLightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_on_normal.png")));
	m_pLightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_on_hover.png")));
	m_pLightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_off_normal.png")));
	m_pLightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/lights_off_hover.png")));

	m_pLightButton->SetNormalState(m_pLightButtonBitmaps[0]);
	m_pLightButton->SetHoverState(m_pLightButtonBitmaps[1]);
	m_pLightButton->SetDownState(m_pLightButtonBitmaps[1]);
	m_pLightButton->SetDeactivatedState(m_pLightButtonBitmaps[2]);
	m_pLightButton->SetDeactivatedStateHover(m_pLightButtonBitmaps[3]);
	m_pLightButton->SetDeactivatedStateDown(m_pLightButtonBitmaps[3]);

	m_pLightButton->SetState(Button::STATE_DEACTIVATED);

	// MOVE BUTTON
	m_pMoveButton = new Button(168,7,36,36,true);

	m_pMoveButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/move_on_normal.png")));
	m_pMoveButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/move_on_hover.png")));
	m_pMoveButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/move_off_normal.png")));
	m_pMoveButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/move_off_hover.png")));

	m_pMoveButton->SetNormalState(m_pMoveButtonBitmaps[0]);
	m_pMoveButton->SetHoverState(m_pMoveButtonBitmaps[1]);
	m_pMoveButton->SetDownState(m_pMoveButtonBitmaps[1]);
	m_pMoveButton->SetDeactivatedState(m_pMoveButtonBitmaps[2]);
	m_pMoveButton->SetDeactivatedStateHover(m_pMoveButtonBitmaps[3]);
	m_pMoveButton->SetDeactivatedStateDown(m_pMoveButtonBitmaps[3]);

	m_pMoveButton->SetState(Button::STATE_DEACTIVATED);

	// GAME MODE BUTTON
	m_pGameModeButton = new Button(20,7,36,36);

	m_pGameModeButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/game_mode_normal.png")));
	m_pGameModeButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/game_mode_hover.png")));

	m_pGameModeButton->SetNormalState(m_pGameModeButtonBitmaps[0]);
	m_pGameModeButton->SetHoverState(m_pGameModeButtonBitmaps[1]);
	m_pGameModeButton->SetDownState(m_pGameModeButtonBitmaps[1]);

	// EDITOR MODE BUTTON
	m_pEditorModeButton = new Button(20,43,36,36);

	m_pEditorModeButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/editor_mode_normal.png")));
	m_pEditorModeButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/editor_mode_hover.png")));

	m_pEditorModeButton->SetNormalState(m_pEditorModeButtonBitmaps[0]);
	m_pEditorModeButton->SetHoverState(m_pEditorModeButtonBitmaps[1]);
	m_pEditorModeButton->SetDownState(m_pEditorModeButtonBitmaps[1]);

	// POINTLIGHT BUTTON
	m_pPointlightButton = new Button(352,7,36,36);

	m_pPointlightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/pointlight_normal.png")));
	m_pPointlightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/pointlight_hover.png")));

	m_pPointlightButton->SetNormalState(m_pPointlightButtonBitmaps[0]);
	m_pPointlightButton->SetHoverState(m_pPointlightButtonBitmaps[1]);
	m_pPointlightButton->SetDownState(m_pPointlightButtonBitmaps[1]);

	// SPOTLIGHT BUTTON
	m_pSpotlightButton = new Button(388,7,36,36);

	m_pSpotlightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/spotlight_normal.png")));
	m_pSpotlightButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/spotlight_hover.png")));

	m_pSpotlightButton->SetNormalState(m_pSpotlightButtonBitmaps[0]);
	m_pSpotlightButton->SetHoverState(m_pSpotlightButtonBitmaps[1]);
	m_pSpotlightButton->SetDownState(m_pSpotlightButtonBitmaps[1]);

	// COLOR PICKER BUTTON
	m_pColorPickerButton = new Button(100,217,36,36,true);

	m_pColorPickerButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/colorpicker_normal.png")));
	m_pColorPickerButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/colorpicker_hover.png")));

	m_pColorPickerButton->SetNormalState(m_pColorPickerButtonBitmaps[0]);
	m_pColorPickerButton->SetHoverState(m_pColorPickerButtonBitmaps[1]);
	m_pColorPickerButton->SetDownState(m_pColorPickerButtonBitmaps[1]);
	m_pColorPickerButton->SetDeactivatedState(m_pColorPickerButtonBitmaps[0]);
	m_pColorPickerButton->SetDeactivatedStateHover(m_pColorPickerButtonBitmaps[1]);
	m_pColorPickerButton->SetDeactivatedStateDown(m_pColorPickerButtonBitmaps[1]);

	m_pColorPickerButton->SetState(Button::STATE_DEACTIVATED);

	// APPLY BUTTON
	m_pApplyButton = new Button((int)(210 + 360),(int)(60 + 220),36,36);

	m_pApplyButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/apply_normal.png")));
	m_pApplyButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/apply_hover.png")));

	m_pApplyButton->SetNormalState(m_pApplyButtonBitmaps[0]);
	m_pApplyButton->SetHoverState(m_pApplyButtonBitmaps[1]);
	m_pApplyButton->SetDownState(m_pApplyButtonBitmaps[1]);

	// ROTATE BUTTON
	m_pRotateButton = new Button(204,7,36,36,true);

	m_pRotateButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_on_normal.png")));
	m_pRotateButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_on_hover.png")));
	m_pRotateButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_off_normal.png")));
	m_pRotateButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/rotate_off_hover.png")));

	m_pRotateButton->SetNormalState(m_pRotateButtonBitmaps[0]);
	m_pRotateButton->SetHoverState(m_pRotateButtonBitmaps[1]);
	m_pRotateButton->SetDownState(m_pRotateButtonBitmaps[1]);
	m_pRotateButton->SetDeactivatedState(m_pRotateButtonBitmaps[2]);
	m_pRotateButton->SetDeactivatedStateHover(m_pRotateButtonBitmaps[3]);
	m_pRotateButton->SetDeactivatedStateDown(m_pRotateButtonBitmaps[3]);

	m_pRotateButton->SetState(Button::STATE_DEACTIVATED);

	// SCALE BUTTON
	m_pScaleButton = new Button(240,7,36,36,true);

	m_pScaleButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_on_normal.png")));
	m_pScaleButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_on_hover.png")));
	m_pScaleButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_off_normal.png")));
	m_pScaleButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/scale_off_hover.png")));

	m_pScaleButton->SetNormalState(m_pScaleButtonBitmaps[0]);
	m_pScaleButton->SetHoverState(m_pScaleButtonBitmaps[1]);
	m_pScaleButton->SetDownState(m_pScaleButtonBitmaps[1]);
	m_pScaleButton->SetDeactivatedState(m_pScaleButtonBitmaps[2]);
	m_pScaleButton->SetDeactivatedStateHover(m_pScaleButtonBitmaps[3]);
	m_pScaleButton->SetDeactivatedStateDown(m_pScaleButtonBitmaps[3]);

	m_pScaleButton->SetState(Button::STATE_DEACTIVATED);

	// SNAPPING BUTTON
	m_pSnappingButton = new Button(296,7,36,36,true);

	m_pSnappingButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/snapping_normal.png")));
	m_pSnappingButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/snapping_hover.png")));

	m_pSnappingButton->SetNormalState(m_pSnappingButtonBitmaps[1]);
	m_pSnappingButton->SetHoverState(m_pSnappingButtonBitmaps[1]);
	m_pSnappingButton->SetDownState(m_pSnappingButtonBitmaps[1]);
	m_pSnappingButton->SetDeactivatedState(m_pSnappingButtonBitmaps[0]);
	m_pSnappingButton->SetDeactivatedStateHover(m_pSnappingButtonBitmaps[0]);
	m_pSnappingButton->SetDeactivatedStateDown(m_pSnappingButtonBitmaps[0]);

	m_pSnappingButton->SetState(Button::STATE_DEACTIVATED);

	// PLAY MODE BUTTON
	m_pPlayModeButton = new Button(20,79,36,36);
	
	m_pPlayModeButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/play_mode_normal.png")));
	m_pPlayModeButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/play_mode_hover.png")));

	m_pPlayModeButton->SetNormalState(m_pPlayModeButtonBitmaps[0]);
	m_pPlayModeButton->SetHoverState(m_pPlayModeButtonBitmaps[1]);
	m_pPlayModeButton->SetDownState(m_pPlayModeButtonBitmaps[1]);

	// LOAD MODEL BUTTON
	m_pLoadModelButton = new Button(444,7,36,36, true);

	m_pLoadModelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_off_normal.png")));
	m_pLoadModelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_off_hover.png")));
	m_pLoadModelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_on_normal.png")));
	m_pLoadModelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_model_on_hover.png")));

	m_pLoadModelButton->SetDeactivatedState(m_pLoadModelButtonBitmaps[0]);
	m_pLoadModelButton->SetDeactivatedStateHover(m_pLoadModelButtonBitmaps[1]);
	m_pLoadModelButton->SetDeactivatedStateDown(m_pLoadModelButtonBitmaps[1]);

	m_pLoadModelButton->SetNormalState(m_pLoadModelButtonBitmaps[2]);
	m_pLoadModelButton->SetHoverState(m_pLoadModelButtonBitmaps[3]);
	m_pLoadModelButton->SetDownState(m_pLoadModelButtonBitmaps[3]);

	m_pLoadModelButton->SetState(Button::STATE_DEACTIVATED);

	// LOAD LEVEL BUTTON
	m_pLoadLevelButton = new Button(480,7,36,36, true);

	m_pLoadLevelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_off_normal.png")));
	m_pLoadLevelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_off_hover.png")));
	m_pLoadLevelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_on_normal.png")));
	m_pLoadLevelButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_level_on_hover.png")));

	m_pLoadLevelButton->SetDeactivatedState(m_pLoadLevelButtonBitmaps[0]);
	m_pLoadLevelButton->SetDeactivatedStateHover(m_pLoadLevelButtonBitmaps[1]);
	m_pLoadLevelButton->SetDeactivatedStateDown(m_pLoadLevelButtonBitmaps[1]);

	m_pLoadLevelButton->SetNormalState(m_pLoadLevelButtonBitmaps[2]);
	m_pLoadLevelButton->SetHoverState(m_pLoadLevelButtonBitmaps[3]);
	m_pLoadLevelButton->SetDownState(m_pLoadLevelButtonBitmaps[3]);

	m_pLoadLevelButton->SetState(Button::STATE_DEACTIVATED);

	// SHOW GRID BUTTON
	m_pShowGridButton = new Button(544,7,36,36,true);

	m_pShowGridButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_on_normal.png")));
	m_pShowGridButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_on_hover.png")));
	m_pShowGridButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_off_normal.png")));
	m_pShowGridButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/grid_off_hover.png")));

	m_pShowGridButton->SetNormalState(m_pShowGridButtonBitmaps[0]);
	m_pShowGridButton->SetHoverState(m_pShowGridButtonBitmaps[1]);
	m_pShowGridButton->SetDownState(m_pShowGridButtonBitmaps[1]);

	m_pShowGridButton->SetDeactivatedState(m_pShowGridButtonBitmaps[2]);
	m_pShowGridButton->SetDeactivatedStateHover(m_pShowGridButtonBitmaps[3]);
	m_pShowGridButton->SetDeactivatedStateDown(m_pShowGridButtonBitmaps[3]);

	m_pShowGridButton->SetState(Button::STATE_NORMAL);

	// POST EFFECTS BUTTON
	m_pPostFXButton = new Button(112,7,36,36,true);

	m_pPostFXButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_on_normal.png")));
	m_pPostFXButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_on_hover.png")));
	m_pPostFXButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_off_normal.png")));
	m_pPostFXButtonBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/posteffects_off_hover.png")));

	m_pPostFXButton->SetNormalState(m_pPostFXButtonBitmaps[0]);
	m_pPostFXButton->SetHoverState(m_pPostFXButtonBitmaps[1]);
	m_pPostFXButton->SetDownState(m_pPostFXButtonBitmaps[1]);

	m_pPostFXButton->SetDeactivatedState(m_pPostFXButtonBitmaps[2]);
	m_pPostFXButton->SetDeactivatedStateHover(m_pPostFXButtonBitmaps[3]);
	m_pPostFXButton->SetDeactivatedStateDown(m_pPostFXButtonBitmaps[3]);

	m_pPostFXButton->SetState(Button::STATE_DEACTIVATED);

	// CAMERA
	m_pCameraBitmap = Content->LoadImage(_T("../Content/Images/Editor/camera.png"));

	// LIGHT DEBUGGER
	m_pLightDebugger = new VisualLightDebugger();

	m_pObjectInfo = new LevelObjectInfo(m_pLevel);

	// COLOR PICKER
	m_pColorPicker = new ColorPicker();

	// MOVE GIZMO
	m_pMoveGizmo = new MoveGizmo();

	// ROTATE GIZMO
	m_pRotateGizmo = new RotateGizmo();

	// SCALE GIZMO
	m_pScaleGizmo = new ScaleGizmo();

	m_pSnapper = new Snapper(m_pMoveGizmo, m_pRotateGizmo);

	m_pModelLoader = new LoadModelFromFile(m_pLevel, m_pPhysXEngine);
	m_pLevelLoader = new LoadLevelFromFile(m_pLevel);

	// FONT
	m_pInfoFont = Content->LoadTextFormat(_T("Verdana"),10,false,false);
}

void EditorGUI::Draw(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	BX2D->SetAntiAliasing(false);

	/*if (m_Mode == MODE_EDITOR || m_Mode == MODE_GAME)
	{
	for_each(m_pLevel->GetTriggers().cbegin(), m_pLevel->GetTriggers().cend(),[&](pair<tstring, Trigger*> trigger)
	{
	trigger.second->Draw(const_cast<RenderContext*>(pRenderContext));
	});
	}*/

	if (m_Mode == MODE_EDITOR)
	{
		DrawSelectedObjects();

		if (m_bMoveable)
			m_pLightDebugger->MovingLights(true);
		else
			m_pLightDebugger->MovingLights(false);

		m_pLightDebugger->Tick(m_pRenderContext);
		m_pObjectInfo->Tick();

		BX2D->SetAntiAliasing(true);

		// MOVE GIZMO
       
        if (m_bMoveable)
            m_pMoveGizmo->Draw(m_pRenderContext);
        else if (m_bRotateable)
            m_pRotateGizmo->Draw(m_pRenderContext);
		else if (m_bScaleable)
			m_pScaleGizmo->Draw(m_pRenderContext);

		BX2D->SetAntiAliasing(false);
	}
	else
	{
		m_pLightDebugger->HideTextBoxes();
		m_pObjectInfo->HideTextBoxes();
        m_pObjectSelecter->DeselectAll();
		m_pSnapper->HideTextBoxes();
	}

	// BACKGROUND
	BX2D->SetColor(70, 70, 70);
	BX2D->FillRect(0, 0, BX2D->GetWindowSize().width, 50);

	BX2D->SetColor(90, 90, 90);
	BX2D->DrawLine(0, 50, BX2D->GetWindowSize().width, 50);

	BX2D->SetColor(70, 70, 70);
	BX2D->FillRect(	0,	BX2D->GetWindowSize().height - 20,
						BX2D->GetWindowSize().width,
						20);
	
	BX2D->SetColor(90, 90, 90);
	BX2D->DrawLine(	0,	BX2D->GetWindowSize().height - 20,
						BX2D->GetWindowSize().width,
						BX2D->GetWindowSize().height - 20);

	if (m_Mode == MODE_EDITOR)
	{
		BX2D->SetColor(70, 70, 70);
		BX2D->FillRect(0, 50, 200, BX2D->GetWindowSize().height - 71);

		if (!m_pSnappingButton->IsActive())
		{
			m_pLightDebugger->Draw();
			m_pObjectInfo->Draw(m_pRenderContext);
		}
		else
		{
			m_pLightDebugger->HideTextBoxes();
			m_pObjectInfo->HideTextBoxes();
		}

		if (m_pLoadModelButton->IsActive() && m_pLightDebugger->GetNrLightsSelected() == 0 && m_pObjectInfo->GetNrObjectsSelected() == 0)
			m_pModelLoader->Show();
		else
		{
			m_pModelLoader->Clear();
			m_pLoadModelButton->Deactivate();
		}

		if (m_pLoadLevelButton->IsActive() && m_pLightDebugger->GetNrLightsSelected() == 0 && m_pObjectInfo->GetNrObjectsSelected() == 0)
			m_pLevelLoader->Show();
		else
		{
			m_pLevelLoader->Clear();
			m_pLoadLevelButton->Deactivate();
		}
	}
	else
	{
		m_pLoadModelButton->Deactivate();
		m_pLoadLevelButton->Deactivate();
		m_pModelLoader->Clear();
		m_pLevelLoader->Clear();
	}

	// BUTTONS
	m_pLightButton->Show();
	if (m_pLightButton->Hover() || m_pMoveButton->Down())
	{
		BX2D->SetColor(255, 255, 255, 0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Switch between lit and unlit mode."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pMoveButton->Show();
	if (m_pMoveButton->Hover() || m_pMoveButton->Down())
	{
		BX2D->SetColor(255, 255, 255, 0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Move objects and lights present in the scene."), 20, BX2D->GetWindowSize().height - 16);
	}

	if (m_Mode == MODE_GAME)
	{
		m_pGameModeButton->SetPosition(20, 7);
		m_pEditorModeButton->SetPosition(20, 43);
		m_pPlayModeButton->SetPosition(20,79);

		if (CONTROLS->LeftMBUp() && m_bGameModeDown)
		{
			if (m_pGameModeButton->Hover())
			{
				m_Mode = MODE_GAME;
			}
			else if (m_pEditorModeButton->Hover())
			{
				m_Mode = MODE_EDITOR;
			}
			else if (m_pPlayModeButton->Hover())
			{
				m_Mode = MODE_PLAY;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bGameModeDown == true)
		{
			m_pGameModeButton->Show();
			m_pEditorModeButton->Show();
			m_pPlayModeButton->Show();
		}
		else
		{
			m_pGameModeButton->Show();

			m_bGameModeDown = false;
		}

		if (m_pGameModeButton->Down())
		{
			m_bGameModeDown = true;
		}
	}
	else if (m_Mode == MODE_EDITOR)
	{
		m_pEditorModeButton->SetPosition(20,7);
		m_pGameModeButton->SetPosition(20,79);
		m_pPlayModeButton->SetPosition(20,43);

		if (CONTROLS->LeftMBUp() && m_bEditorModeDown)
		{
			if (m_pEditorModeButton->Hover())
			{
				m_Mode = MODE_EDITOR;
			}
			else if (m_pGameModeButton->Hover())
			{
				m_Mode = MODE_GAME;
			}
			else if (m_pPlayModeButton->Hover())
			{
				m_Mode = MODE_PLAY;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bEditorModeDown == true)
		{
			m_pGameModeButton->Show();
			m_pEditorModeButton->Show();
			m_pPlayModeButton->Show();
		}
		else
		{
			m_pEditorModeButton->Show();

			m_bEditorModeDown = false;
		}

		if (m_pEditorModeButton->Down())
		{
			m_bEditorModeDown = true;
		}

		if (m_pSnappingButton->IsActive())
			m_pSnapper->Draw();
		else
			m_pSnapper->HideTextBoxes();
	}
	else if (m_Mode == MODE_PLAY)
	{
		m_pEditorModeButton->SetPosition(20,43);
		m_pGameModeButton->SetPosition(20,79);
		m_pPlayModeButton->SetPosition(20,7);

		if (CONTROLS->LeftMBUp() && m_bPlayModeDown)
		{
			if (m_pEditorModeButton->Hover())
			{
				m_Mode = MODE_EDITOR;
			}
			else if (m_pGameModeButton->Hover())
			{
				m_Mode = MODE_GAME;
			}
			else if (m_pPlayModeButton->Hover())
			{
				m_Mode = MODE_PLAY;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bPlayModeDown == true)
		{
			m_pGameModeButton->Show();
			m_pEditorModeButton->Show();
			m_pPlayModeButton->Show();
		}
		else
		{
			m_pPlayModeButton->Show();

			m_bPlayModeDown = false;
		}

		if (m_pPlayModeButton->Down())
		{
			m_bPlayModeDown = true;
		}
	}

	if (m_pEditorModeButton->Hover() || m_pEditorModeButton->Down() || m_pGameModeButton->Hover() || m_pGameModeButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Switch between game and editor mode."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pPointlightButton->Show();
	if (m_pPointlightButton->Hover() || m_pPointlightButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Add a pointlight to the scene."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pSpotlightButton->Show();
	if (m_pSpotlightButton->Hover() || m_pSpotlightButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Add a spotlight to the scene."), 20, BX2D->GetWindowSize().height - 16);
	}

	if (m_Mode == MODE_EDITOR && m_pLightDebugger->GetNrLightsSelected() == 1 && !m_pSnappingButton->IsActive())
		m_pColorPickerButton->Show();

	if (m_pColorPickerButton->Hover() || m_pColorPickerButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Change the color of the selected light."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pRotateButton->Show();
	if (m_pRotateButton->Hover() || m_pRotateButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Rotate objects and lights present in the scene."), 20, BX2D->GetWindowSize().height - 16);
	}

	if (m_pPlayModeButton->Hover() || m_pPlayModeButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Play and interact with the objects in the scene."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pLoadModelButton->Show();
	if (m_pLoadModelButton->Hover() || m_pLoadModelButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Load new model from file and place it into the scene."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pLoadLevelButton->Show();
	if (m_pLoadLevelButton->Hover() || m_pLoadLevelButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Load/save a level from/to a file."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pShowGridButton->Show();
	if (m_pShowGridButton->Hover() || m_pShowGridButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Shows 3D ground grid."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pPostFXButton->Show();
	if (m_pPostFXButton->Hover() || m_pPostFXButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Turns on/off post processing effects."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pScaleButton->Show();
	if (m_pScaleButton->Hover() || m_pScaleButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Scale objects and lights present in the scene."), 20, BX2D->GetWindowSize().height - 16);
	}

	m_pSnappingButton->Show();
	if (m_pSnappingButton->Hover() || m_pSnappingButton->Down())
	{
		BX2D->SetColor(255,255,255,0.5f);
		BX2D->SetFont(m_pInfoFont);
		BX2D->DrawString(_T("Change the snapping options for the gizmo's."), 20, BX2D->GetWindowSize().height - 16);
	}

	// CAMERA
	if (m_bUsingCamera && m_Mode != MODE_PLAY)
		BX2D->DrawImage(m_pCameraBitmap, BX2D->GetWindowSize().width - 70, 90, true, 0.8f);

	BX2D->SetAntiAliasing(true);

	if (m_pColorPickerButton->IsActive() && m_Mode == MODE_EDITOR)
	{
		if (m_pLightDebugger->GetNrLightsSelected() == 1)
		{
			m_pApplyButton->Tick();
		
            //TODO
			for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
			{
				if (m_pRenderContext->GetLightController()->GetLights()[i]->IsSelected())
				{
					Color prevColor = m_pRenderContext->GetLightController()->GetLights()[i]->GetColor() * 255.0f;

					m_pColorPicker->SetPreviousColor(prevColor);

					if (m_pApplyButton->Clicked())
					{
						m_pRenderContext->GetLightController()->GetLights()[i]->SetColor(m_pColorPicker->GetCurrentColor() / 255.0f);

						m_pColorPicker->PreviousColorSet(false);

						m_pColorPicker->SetPreviousColor(m_pColorPicker->GetCurrentColor());
					}
				}
			}

			m_pColorPicker->Show();
			m_pApplyButton->Show();
		}

		if (m_pColorPickerButton->Clicked())
			m_pColorPicker->PreviousColorSet(false);
	}
	else
	{
		m_pColorPicker->PreviousColorSet(false);
	}

	// SCENE INFO
	tstringstream streamInfo;

	int objectsSelected(0);

	for_each(m_pLevel->GetLevelObjects().cbegin(), m_pLevel->GetLevelObjects().cend(), [&](ILevelObject* obj)
    {
		if (obj->IsSelected())
			++objectsSelected;
    });

	streamInfo << _T("lights: ") << m_pLightDebugger->GetNrLightsSelected() << _T(" / ") << m_pLightDebugger->GetTotalLightsInScene();
	streamInfo << _T("     ");
	streamInfo << _T("objects: ") << objectsSelected << _T(" / ") << m_pLevel->GetLevelObjects().size();

	BX2D->SetColor(255, 255, 255, 0.5f);
	BX2D->SetFont(m_pInfoFont);

	m_pInfoFont->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
	m_pInfoFont->SetVerticalAlignment(PARAGRAPH_ALIGNMENT_BOTTOM);

	BX2D->DrawString(	streamInfo.str(),
						RectF(0,0, BX2D->GetWindowSize().width - 5, BX2D->GetWindowSize().height - 4));
}

void EditorGUI::Tick()
{
	if (!m_pRenderContext)
		return;

	// BUTTONS
	m_pLightButton->Tick();
	m_pMoveButton->Tick();
	m_pGameModeButton->Tick();
	m_pEditorModeButton->Tick();
	m_pPointlightButton->Tick();
	m_pSpotlightButton->Tick();
	m_pRotateButton->Tick();
	m_pPlayModeButton->Tick();
	m_pLoadModelButton->Tick();
	m_pShowGridButton->Tick();
	m_pLoadLevelButton->Tick();
	m_pPostFXButton->Tick();
	m_pScaleButton->Tick();
	m_pSnappingButton->Tick();

	if (m_pLoadLevelButton->Clicked())
		m_pLoadModelButton->Deactivate();
	if (m_pLoadModelButton->Clicked())
		m_pLoadLevelButton->Deactivate();

	if (m_pSnappingButton->IsActive())
		m_pSnapper->Tick();

	if (m_Mode == MODE_EDITOR && m_pLightDebugger->GetNrLightsSelected() == 1)
		m_pColorPickerButton->Tick();

	if (!m_pColorPickerButton->IsActive() && m_Mode == MODE_EDITOR)
		m_pObjectSelecter->Tick(m_pRenderContext);

	m_pLightDebugger->Tick(m_pRenderContext);

	if (m_Mode == MODE_EDITOR)
	{
		if (CONTROLS->IsKeyDown(VK_LCONTROL) && CONTROLS->IsKeyDown('D'))
		{
			m_pMoveButton->Deactivate();
			m_pRotateButton->Deactivate();
		}

		if (m_pPointlightButton->Clicked())
		{
			Vector3 look = m_pRenderContext->GetCamera()->GetLook();
			look.Normalize();

			PointLightDesc pDesc;

			pDesc.position = (m_pRenderContext->GetCamera()->GetPosition() + look * 10);

			BYTE r = 180;
			BYTE g = 180;
			BYTE b = 200;
            BYTE a = 255;

			pDesc.color = Color(r, g, b, a);
			pDesc.multiplier = 1.0f;
			pDesc.attenuationStart = 0;
			pDesc.attenuationEnd = 5;
            
			PointLight* pl = new PointLight(pDesc);
			pl->InitEditor();
			m_pRenderContext->GetLightController()->AddLight(pl);

			m_pMoveButton->Deactivate();
			m_pRotateButton->Deactivate();

			cout << "Added pointlight\n";
		}

		if (m_pSpotlightButton->Clicked())
		{
			Vector3 look = m_pRenderContext->GetCamera()->GetLook();
			look.Normalize();

			SpotLight* sp2 = new SpotLight();
				sp2->SetPosition(m_pRenderContext->GetCamera()->GetPosition() + look * 10);
				sp2->SetColor(Color(0.9f, 0.9f, 0.9f, 1.0f));
				sp2->SetMulitplier(2.0f);
				sp2->SetAttenuationStart(1);
				sp2->SetAttenuationEnd(5);
				sp2->SetOpeningsAngle(ToRadians(90));
				sp2->Rotate(Vector3::Forward, PiOver2);
				sp2->SetShadowMap(m_pDXDevice, ShadowMapType1024x1024);
			
			m_pRenderContext->GetLightController()->AddLight(sp2);

			m_pMoveButton->Deactivate();
			m_pRotateButton->Deactivate();

			cout << "Added spotlight\n";
		}
	}

	if (m_pLoadModelButton->IsActive())
		m_pModelLoader->Tick(m_pRenderContext);
	else
		m_pModelLoader->HideTextBoxes();

	if (m_pLoadLevelButton->IsActive())
		m_pLevelLoader->Tick();
	else
		m_pLevelLoader->HideTextBoxes();

	// CAMERA
	if (CONTROLS->RightMBDown())
		m_bUsingCamera = true;
	else
		m_bUsingCamera = false;

	if (m_pMoveButton->IsActive())
	{
		m_bMoveable = true;
		m_bRotateable = false;
		m_bScaleable = false;
	}
	else
		m_bMoveable = false;

	if (m_pRotateButton->IsActive())
	{
		m_bMoveable = false;
		m_bRotateable = true;
		m_bScaleable = false;
	}
	else 
		m_bRotateable = false;

	if (m_pScaleButton->IsActive())
	{
		m_bMoveable = false;
		m_bRotateable = false;
		m_bScaleable = true;
	}
	else 
		m_bScaleable = false;

	if (m_pMoveButton->Clicked())
	{
		m_pRotateButton->Deactivate();
		m_pScaleButton->Deactivate();
	}
	else if (m_pRotateButton->Clicked())
	{
		m_pMoveButton->Deactivate();
		m_pScaleButton->Deactivate();
	}
	else if (m_pScaleButton->Clicked())
	{
		m_pMoveButton->Deactivate();
		m_pRotateButton->Deactivate();
	}

    if (m_bMoveable)
        m_pMoveGizmo->Tick(m_pObjectSelecter);
    else if (m_bRotateable)
        m_pRotateGizmo->Tick(m_pObjectSelecter);
	else if (m_bScaleable)
		m_pScaleGizmo->Tick(m_pObjectSelecter);
}

void EditorGUI::DrawSelectedObjects()
{
	for_each(m_pLevel->GetLevelObjects().cbegin(), m_pLevel->GetLevelObjects().cend(), [&](ILevelObject* pObj)
	{
		SimpleObject* pSObj = dynamic_cast<SimpleObject*>(pObj);

		if (pSObj && pSObj->IsSelected() && pSObj->IsUsedForInstancing())
		{
			Matrix matProj = m_pRenderContext->GetCamera()->GetProjection();
			Matrix matView = m_pRenderContext->GetCamera()->GetView();
			Matrix matWorld = Matrix::Identity;

			D3D10_VIEWPORT viewPort;
			viewPort.MinDepth = 0.0f;
			viewPort.MaxDepth = 1.0f;
			viewPort.TopLeftX = 0;
			viewPort.TopLeftY = 0;
			viewPort.Height = static_cast<UINT>(BX2D->GetWindowSize().height);
			viewPort.Width = static_cast<UINT>(BX2D->GetWindowSize().width);

			Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();
			Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - pSObj->GetPosition();
			//float l = length.Length() / 100;

			//Vector3 pos2D(Vector3::Project(pSObj->GetPosition(), &viewPort, matProj, matView, matWorld));

			if (vLook.Dot(length) < 0)
			{
				/*BX2D->SetColor(255, 255, 0, 0.5f);
				BX2D->FillEllipse(pos2D.X, pos2D.Y, 4 / l, 4 / l);*/

				NxBounds3 bounds;
				pSObj->GetActor()->getShapes()[0]->getWorldBounds(bounds);

				NxVec3 mid;
				mid.zero();
				bounds.getCenter(mid);
				NxVec3 dim;
				bounds.getDimensions(dim);

				Vector3 cube[8];

				cube[0] = Vector3(mid.x + (dim.x/2), mid.y + (dim.y/2), mid.z + (dim.z/2));
				cube[1] = Vector3(mid.x - (dim.x/2), mid.y + (dim.y/2), mid.z + (dim.z/2));
				cube[2] = Vector3(mid.x + (dim.x/2), mid.y - (dim.y/2), mid.z + (dim.z/2));
				cube[3] = Vector3(mid.x - (dim.x/2), mid.y - (dim.y/2), mid.z + (dim.z/2));
				cube[4] = Vector3(mid.x + (dim.x/2), mid.y + (dim.y/2), mid.z - (dim.z/2));
				cube[5] = Vector3(mid.x - (dim.x/2), mid.y + (dim.y/2), mid.z - (dim.z/2));
				cube[6] = Vector3(mid.x + (dim.x/2), mid.y - (dim.y/2), mid.z - (dim.z/2));
				cube[7] = Vector3(mid.x - (dim.x/2), mid.y - (dim.y/2), mid.z - (dim.z/2));

				Vector3 cube2D[8];

				for (int i = 0; i < 8; ++i)
				{
					cube2D[i] = Vector3::Project(cube[i], &viewPort, matProj, matView, matWorld);
				}

				Point2D pol1[4];
				pol1[0] = Point2F(cube2D[0].X, cube2D[0].Y);
				pol1[1] = Point2F(cube2D[1].X, cube2D[1].Y);
				pol1[2] = Point2F(cube2D[3].X, cube2D[3].Y);
				pol1[3] = Point2F(cube2D[2].X, cube2D[2].Y);

				Point2D pol2[4];
				pol2[0] = Point2F(cube2D[4].X, cube2D[4].Y);
				pol2[1] = Point2F(cube2D[5].X, cube2D[5].Y);
				pol2[2] = Point2F(cube2D[7].X, cube2D[7].Y);
				pol2[3] = Point2F(cube2D[6].X, cube2D[6].Y);

				Point2D pol3[4];
				pol3[0] = Point2F(cube2D[0].X, cube2D[0].Y);
				pol3[1] = Point2F(cube2D[2].X, cube2D[2].Y);
				pol3[2] = Point2F(cube2D[6].X, cube2D[6].Y);
				pol3[3] = Point2F(cube2D[4].X, cube2D[4].Y);

				Point2D pol4[4];
				pol4[0] = Point2F(cube2D[1].X, cube2D[1].Y);
				pol4[1] = Point2F(cube2D[3].X, cube2D[3].Y);
				pol4[2] = Point2F(cube2D[7].X, cube2D[7].Y);
				pol4[3] = Point2F(cube2D[5].X, cube2D[5].Y);

				Point2D pol5[4];
				pol5[0] = Point2F(cube2D[0].X, cube2D[0].Y);
				pol5[1] = Point2F(cube2D[1].X, cube2D[1].Y);
				pol5[2] = Point2F(cube2D[5].X, cube2D[5].Y);
				pol5[3] = Point2F(cube2D[4].X, cube2D[4].Y);

				Point2D pol6[4];
				pol6[0] = Point2F(cube2D[2].X, cube2D[2].Y);
				pol6[1] = Point2F(cube2D[3].X, cube2D[3].Y);
				pol6[2] = Point2F(cube2D[7].X, cube2D[7].Y);
				pol6[3] = Point2F(cube2D[6].X, cube2D[6].Y);

				BX2D->SetColor(255, 255, 0, 0.2f);
				BX2D->FillPolygon(pol1, 4);
				BX2D->FillPolygon(pol2, 4);
				BX2D->FillPolygon(pol3, 4);
				BX2D->FillPolygon(pol4, 4);
				BX2D->FillPolygon(pol5, 4);
				BX2D->FillPolygon(pol6, 4);
			}
		}
	});
}

// GETTERS
void EditorGUI::Clear()
{
    m_pObjectSelecter->DeselectAll();
}