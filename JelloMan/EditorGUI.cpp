#include "EditorGUI.h"


// CONSTRUCTOR - DESTRUCTOR
EditorGUI::EditorGUI()	:	m_pLightButton(0),
							m_pCameraBitmap(0),
							m_bUsingCamera(false),
							m_pMoveButton(0),
							m_pGameModeButton(0),
							m_pEditorModeButton(0),
							m_pPointlightButton(0),
							m_pSpotlightButton(0),
							m_GameEditorModeSelect(0),
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
							m_pRotateGizmo(0)
{
	
}

EditorGUI::~EditorGUI()
{
	delete m_pLightButton;
	for (vector<Bitmap*>::iterator it = m_pLightButtonBitmaps.begin(); it != m_pLightButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pMoveButton;
	for (vector<Bitmap*>::iterator it = m_pMoveButtonBitmaps.begin(); it != m_pMoveButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pGameModeButton;
	for (vector<Bitmap*>::iterator it = m_pGameModeButtonBitmaps.begin(); it != m_pGameModeButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pEditorModeButton;
	for (vector<Bitmap*>::iterator it = m_pEditorModeButtonBitmaps.begin(); it != m_pEditorModeButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pPointlightButton;
	for (vector<Bitmap*>::iterator it = m_pPointlightButtonBitmaps.begin(); it != m_pPointlightButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pSpotlightButton;
	for (vector<Bitmap*>::iterator it = m_pSpotlightButtonBitmaps.begin(); it != m_pSpotlightButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pColorPickerButton;
	for (vector<Bitmap*>::iterator it = m_pColorPickerButtonBitmaps.begin(); it != m_pColorPickerButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pApplyButton;
	for (vector<Bitmap*>::iterator it = m_pApplyButtonBitmaps.begin(); it != m_pApplyButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pRotateButton;
	for (vector<Bitmap*>::iterator it = m_pRotateButtonBitmaps.begin(); it != m_pRotateButtonBitmaps.end(); ++it)
		delete (*it);

	delete m_pCameraBitmap;
	delete m_pLightDebugger;
	delete m_pColorPicker;
	delete m_pMoveGizmo;
	delete m_pRotateGizmo;
}

// GENERAL
void EditorGUI::Initialize()
{
	// LIGHT BUTTON
	m_pLightButton = new Button(60,7,36,36,true);

	m_pLightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/lights_on_normal.png")));
	m_pLightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/lights_on_hover.png")));
	m_pLightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/lights_off_normal.png")));
	m_pLightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/lights_off_hover.png")));

	m_pLightButton->SetNormalState(m_pLightButtonBitmaps[0]);
	m_pLightButton->SetHoverState(m_pLightButtonBitmaps[1]);
	m_pLightButton->SetDownState(m_pLightButtonBitmaps[1]);
	m_pLightButton->SetDeactivatedState(m_pLightButtonBitmaps[2]);
	m_pLightButton->SetDeactivatedStateHover(m_pLightButtonBitmaps[3]);
	m_pLightButton->SetDeactivatedStateDown(m_pLightButtonBitmaps[3]);

	// MOVE BUTTON
	m_pMoveButton = new Button(120,7,36,36,true);

	m_pMoveButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/move_on_normal.png")));
	m_pMoveButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/move_on_hover.png")));
	m_pMoveButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/move_off_normal.png")));
	m_pMoveButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/move_off_hover.png")));

	m_pMoveButton->SetNormalState(m_pMoveButtonBitmaps[0]);
	m_pMoveButton->SetHoverState(m_pMoveButtonBitmaps[1]);
	m_pMoveButton->SetDownState(m_pMoveButtonBitmaps[1]);
	m_pMoveButton->SetDeactivatedState(m_pMoveButtonBitmaps[2]);
	m_pMoveButton->SetDeactivatedStateHover(m_pMoveButtonBitmaps[3]);
	m_pMoveButton->SetDeactivatedStateDown(m_pMoveButtonBitmaps[3]);

	m_pMoveButton->SetState(Button::STATE_DEACTIVATED);

	// GAME MODE BUTTON
	m_pGameModeButton = new Button(20,7,36,36);

	m_pGameModeButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/game_mode_normal.png")));
	m_pGameModeButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/game_mode_hover.png")));

	m_pGameModeButton->SetNormalState(m_pGameModeButtonBitmaps[0]);
	m_pGameModeButton->SetHoverState(m_pGameModeButtonBitmaps[1]);
	m_pGameModeButton->SetDownState(m_pGameModeButtonBitmaps[1]);

	// EDITOR MODE BUTTON
	m_pEditorModeButton = new Button(20,43,36,36);

	m_pEditorModeButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/editor_mode_normal.png")));
	m_pEditorModeButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/editor_mode_hover.png")));

	m_pEditorModeButton->SetNormalState(m_pEditorModeButtonBitmaps[0]);
	m_pEditorModeButton->SetHoverState(m_pEditorModeButtonBitmaps[1]);
	m_pEditorModeButton->SetDownState(m_pEditorModeButtonBitmaps[1]);

	// POINTLIGHT BUTTON
	m_pPointlightButton = new Button(216,7,36,36);

	m_pPointlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/pointlight_normal.png")));
	m_pPointlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/pointlight_hover.png")));

	m_pPointlightButton->SetNormalState(m_pPointlightButtonBitmaps[0]);
	m_pPointlightButton->SetHoverState(m_pPointlightButtonBitmaps[1]);
	m_pPointlightButton->SetDownState(m_pPointlightButtonBitmaps[1]);

	// SPOTLIGHT BUTTON
	m_pSpotlightButton = new Button(250,7,36,36);

	m_pSpotlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/spotlight_normal.png")));
	m_pSpotlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/spotlight_hover.png")));

	m_pSpotlightButton->SetNormalState(m_pSpotlightButtonBitmaps[0]);
	m_pSpotlightButton->SetHoverState(m_pSpotlightButtonBitmaps[1]);
	m_pSpotlightButton->SetDownState(m_pSpotlightButtonBitmaps[1]);

	// COLOR PICKER BUTTON
	m_pColorPickerButton = new Button(100,217,36,36,true);

	m_pColorPickerButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/colorpicker_normal.png")));
	m_pColorPickerButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/colorpicker_hover.png")));

	m_pColorPickerButton->SetNormalState(m_pColorPickerButtonBitmaps[0]);
	m_pColorPickerButton->SetHoverState(m_pColorPickerButtonBitmaps[1]);
	m_pColorPickerButton->SetDownState(m_pColorPickerButtonBitmaps[1]);
	m_pColorPickerButton->SetDeactivatedState(m_pColorPickerButtonBitmaps[0]);
	m_pColorPickerButton->SetDeactivatedStateHover(m_pColorPickerButtonBitmaps[1]);
	m_pColorPickerButton->SetDeactivatedStateDown(m_pColorPickerButtonBitmaps[1]);

	m_pColorPickerButton->SetState(Button::STATE_DEACTIVATED);

	// APPLY BUTTON
	m_pApplyButton = new Button((int)(210 + 360),(int)(60 + 220),36,36);

	m_pApplyButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/apply_normal.png")));
	m_pApplyButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/apply_hover.png")));

	m_pApplyButton->SetNormalState(m_pApplyButtonBitmaps[0]);
	m_pApplyButton->SetHoverState(m_pApplyButtonBitmaps[1]);
	m_pApplyButton->SetDownState(m_pApplyButtonBitmaps[1]);

	// ROTATE BUTTON
	m_pRotateButton = new Button(155,7,36,36,true);

	m_pRotateButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/rotate_on_normal.png")));
	m_pRotateButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/rotate_on_hover.png")));
	m_pRotateButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/rotate_off_normal.png")));
	m_pRotateButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/rotate_off_hover.png")));

	m_pRotateButton->SetNormalState(m_pRotateButtonBitmaps[0]);
	m_pRotateButton->SetHoverState(m_pRotateButtonBitmaps[1]);
	m_pRotateButton->SetDownState(m_pRotateButtonBitmaps[1]);
	m_pRotateButton->SetDeactivatedState(m_pRotateButtonBitmaps[2]);
	m_pRotateButton->SetDeactivatedStateHover(m_pRotateButtonBitmaps[3]);
	m_pRotateButton->SetDeactivatedStateDown(m_pRotateButtonBitmaps[3]);

	m_pRotateButton->SetState(Button::STATE_DEACTIVATED);

	// CAMERA
	m_pCameraBitmap = new Bitmap(_T("Content/Images/Editor/camera.png"));

	// LIGHT DEBUGGER
	m_pLightDebugger = new VisualLightDebugger();

	// COLOR PICKER
	m_pColorPicker = new ColorPicker();

	// MOVE GIZMO
	m_pMoveGizmo = new MoveGizmo();

	// ROTATE GIZMO
	m_pRotateGizmo = new RotateGizmo();
}
void EditorGUI::Draw()
{
	BLOX_2D->SetAntiAliasing(false);

	if (m_GameEditorModeSelect == 1)
	{
		m_pLightDebugger->Tick(m_pRenderContext);
		m_pLightDebugger->Draw();

		BLOX_2D->SetAntiAliasing(true);

		// MOVE GIZMO
		for (unsigned int i = 0; i < m_pLightDebugger->GetSpotLightsSelected().size(); ++i)
		{
			if (m_pLightDebugger->GetSpotLightsSelected()[i] == true)
			{
				if (m_bMoveable)
					m_pMoveGizmo->Show(m_pRenderContext->GetLightController()->GetSpotLights()[i].position,MoveGizmo::TYPE_SPOTLIGHT,i);
				else if (m_bRotateable)
					m_pRotateGizmo->Show(m_pRenderContext->GetLightController()->GetSpotLights()[i].position,RotateGizmo::TYPE_SPOTLIGHT,i);
			}
		}

		for (unsigned int i = 0; i < m_pLightDebugger->GetPointLightsSelected().size(); ++i)
		{
			if (m_pLightDebugger->GetPointLightsSelected()[i] == true)
			{
				if (m_bMoveable)
					m_pMoveGizmo->Show(m_pRenderContext->GetLightController()->GetPointLights()[i].position,MoveGizmo::TYPE_POINTLIGHT,i);
				else if (m_bRotateable)
					m_pRotateGizmo->Show(m_pRenderContext->GetLightController()->GetSpotLights()[i].position,RotateGizmo::TYPE_SPOTLIGHT,i);
			}
		}

		BLOX_2D->SetAntiAliasing(false);
	}

	// BACKGROUND
	BLOX_2D->SetColor(70, 70, 70);
	BLOX_2D->FillRect(0, 0, static_cast<int>(BLOX_2D->GetWindowSize().width), 50);

	BLOX_2D->SetColor(90, 90, 90);
	BLOX_2D->DrawLine(0, 50, static_cast<int>(BLOX_2D->GetWindowSize().width), 50);

	BLOX_2D->SetColor(70, 70, 70);
	BLOX_2D->FillRect(	0,
						static_cast<int>(BLOX_2D->GetWindowSize().height) - 20,
						static_cast<int>(BLOX_2D->GetWindowSize().width),
						20);
	
	BLOX_2D->SetColor(90, 90, 90);
	BLOX_2D->DrawLine(	0,
						static_cast<int>(BLOX_2D->GetWindowSize().height) - 20,
						static_cast<int>(BLOX_2D->GetWindowSize().width),
						static_cast<int>(BLOX_2D->GetWindowSize().height) - 20);

	if (m_GameEditorModeSelect == 1)
	{
		BLOX_2D->SetColor(70, 70, 70);
		BLOX_2D->FillRect(0, 50, 200, static_cast<int>(BLOX_2D->GetWindowSize().height) - 71);

		m_pLightDebugger->ShowLightInfo();
	}

	// BUTTONS
	m_pLightButton->Show();
	if (m_pLightButton->Hover() || m_pMoveButton->Down())
	{
		BLOX_2D->SetColor(255, 255, 255, 0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Switch between lit and unlit mode."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	m_pMoveButton->Show();
	if (m_pMoveButton->Hover() || m_pMoveButton->Down())
	{
		BLOX_2D->SetColor(255, 255, 255, 0.5f);
		BLOX_2D->SetFont(_T("Verdana"), false, false, 10);
		BLOX_2D->DrawString(_T("Move objects and lights present in the scene."), 20, static_cast<int>(BLOX_2D->GetWindowSize().height) - 16);
	}

	if (m_GameEditorModeSelect == 0)
	{
		m_pGameModeButton->SetPosition(20, 7);
		m_pEditorModeButton->SetPosition(20, 43);

		if (CONTROLS->LeftMBUp() && m_bGameModeDown)
		{
			if (m_pGameModeButton->Hover())
			{
				m_GameEditorModeSelect = 0;
			}
			else if (m_pEditorModeButton->Hover())
			{
				m_GameEditorModeSelect = 1;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bGameModeDown == true)
		{
			m_pGameModeButton->Show();
			m_pEditorModeButton->Show();
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
	else if (m_GameEditorModeSelect == 1)
	{
		m_pEditorModeButton->SetPosition(20,7);
		m_pGameModeButton->SetPosition(20,43);

		if (CONTROLS->LeftMBUp() && m_bEditorModeDown)
		{
			if (m_pEditorModeButton->Hover())
			{
				m_GameEditorModeSelect = 1;
			}
			else if (m_pGameModeButton->Hover())
			{
				m_GameEditorModeSelect = 0;
			}
		}

		if (CONTROLS->LeftMBDown() && m_bEditorModeDown == true)
		{
			m_pGameModeButton->Show();
			m_pEditorModeButton->Show();
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
	}

	if (m_pEditorModeButton->Hover() || m_pEditorModeButton->Down() || m_pGameModeButton->Hover() || m_pGameModeButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Switch between game and editor mode."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	m_pPointlightButton->Show();
	if (m_pPointlightButton->Hover() || m_pPointlightButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Add a pointlight to the scene."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	m_pSpotlightButton->Show();
	if (m_pSpotlightButton->Hover() || m_pSpotlightButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Add a spotlight to the scene."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	if (m_GameEditorModeSelect == 1 && m_pLightDebugger->GetNrLightsSelected() == 1)
		m_pColorPickerButton->Show();

	if (m_pColorPickerButton->Hover() || m_pColorPickerButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Change the color of the selected light."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	m_pRotateButton->Show();
	if (m_pRotateButton->Hover() || m_pRotateButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Move objects and lights present in the scene."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	// CAMERA
	if (m_bUsingCamera)
		BLOX_2D->DrawBitmap(m_pCameraBitmap,static_cast<int>(BLOX_2D->GetWindowSize().width-70),90,0.8f);

	BLOX_2D->SetAntiAliasing(true);

	/*BLOX_2D->SetColor(255,255,255,0.8f);
	BLOX_2D->SetFont(_T("Verdana"),false,false,10);

	tstringstream stream;
	stream << _T("Lights: ") << m_SelectedLights << _T(" / ") << m_TotalLights;
	BLOX_2D->DrawString(
		stream.str(),
		RectF(0.0f, 0.0f,
		BLOX_2D->GetWindowSize().width - 4,
		BLOX_2D->GetWindowSize().height - 4),
		Blox2D::HORIZONTAL_ALIGN_RIGHT,
		Blox2D::VERTICAL_ALIGN_BOTTOM);*/
}
void EditorGUI::Tick(const RenderContext* pRenderContext)
{
	// BUTTONS
	m_pLightButton->Tick();
	m_pMoveButton->Tick();
	m_pGameModeButton->Tick();
	m_pEditorModeButton->Tick();
	m_pPointlightButton->Tick();
	m_pSpotlightButton->Tick();
	m_pRotateButton->Tick();

	if (m_GameEditorModeSelect == 1 && m_pLightDebugger->GetNrLightsSelected() == 1)
		m_pColorPickerButton->Tick();

	m_pLightDebugger->Tick(pRenderContext);
	m_pMoveGizmo->Tick(pRenderContext);
	m_pRotateGizmo->Tick(pRenderContext);

	if (m_pPointlightButton->Clicked())
	{
		Vector3 look = pRenderContext->GetCamera()->GetLook();
		look.Normalize();

		PointLight pl;
		pl = PointLight();
		pl.position = (pRenderContext->GetCamera()->GetPosition() + look * 200);

		BYTE r = 180;
		BYTE g = 180;
		BYTE b = 200;

		pl.color = Color(r, g, b, 1);
		pl.multiplier = 1.0f;
		pl.AttenuationStart = 0;
		pl.AttenuationEnd = 200;
		pl.lightEnabled = true;

		pRenderContext->GetLightController()->AddLight(pl);

		m_pLightDebugger->DeselectAll();

		cout << "Added pointlight\n";
	}

	if (m_pSpotlightButton->Clicked())
	{
		Vector3 look = pRenderContext->GetCamera()->GetLook();
		look.Normalize();

		SpotLight sl;
		sl = SpotLight();
		sl.position = (pRenderContext->GetCamera()->GetPosition() + look * 200);

		BYTE r = 180;
		BYTE g = 180;
		BYTE b = 200;

		sl.color = Color(r, g, b, 1);
		sl.multiplier = 1.0f;
		sl.AttenuationStart = 0;
		sl.AttenuationEnd = 200;
		sl.power = 2;
		sl.direction = Vector3(0.0f,-1.0f,0.0f);
		sl.lightEnabled = true;
		sl.shadowsEnabled = false;
		sl.shadowMapSize = 256;

		pRenderContext->GetLightController()->AddLight(sl);

		m_pLightDebugger->DeselectAll();

		cout << "Added spotlight\n";
	}

	// CAMERA
	if (CONTROLS->RightMBDown())
		m_bUsingCamera = true;
	else
		m_bUsingCamera = false;

	// RENDERCONTEXT
	m_pRenderContext = pRenderContext;

	if (m_pMoveButton->IsActive())
	{
		m_bMoveable = true;
		m_bRotateable = false;
	}
	else
		m_bMoveable = false;

	if (m_pRotateButton->IsActive())
	{
		m_bMoveable = false;
		m_bRotateable = true;
	}
	else 
		m_bRotateable = false;

	if (m_pMoveButton->Clicked())
		m_pRotateButton->Deactivate();
	if (m_pRotateButton->Clicked())
		m_pMoveButton->Deactivate();

	if (m_pColorPickerButton->IsActive() && m_GameEditorModeSelect == 1)
	{
		if (m_pLightDebugger->GetNrLightsSelected() == 1)
		{
			m_pApplyButton->Tick();
		
			for (unsigned int i = 0; i < m_pLightDebugger->GetPointLightsSelected().size(); ++i)
			{
				if (m_pLightDebugger->GetPointLightsSelected()[i] == true)
				{
					m_pColorPicker->Show(pRenderContext->GetLightController()->GetPointLights()[i].color);
					
					if (m_pApplyButton->Clicked())
					{
						pRenderContext->GetLightController()->GetPointLights()[i].color.R = (float)(m_pColorPicker->GetCurrentColor().R / 255.0f);
						pRenderContext->GetLightController()->GetPointLights()[i].color.G = (float)(m_pColorPicker->GetCurrentColor().G / 255.0f);
						pRenderContext->GetLightController()->GetPointLights()[i].color.B = (float)(m_pColorPicker->GetCurrentColor().B / 255.0f);

						m_pColorPicker->PreviousColorSet(false);
					}
				}
			}

			for (unsigned int i = 0; i < m_pLightDebugger->GetSpotLightsSelected().size(); ++i)
			{
				if (m_pLightDebugger->GetSpotLightsSelected()[i] == true)
				{
					m_pColorPicker->Show(pRenderContext->GetLightController()->GetSpotLights()[i].color);
					
					if (m_pApplyButton->Clicked())
					{
						pRenderContext->GetLightController()->GetSpotLights()[i].color.R = (float)(m_pColorPicker->GetCurrentColor().R / 255.0f);
						pRenderContext->GetLightController()->GetSpotLights()[i].color.G = (float)(m_pColorPicker->GetCurrentColor().G / 255.0f);
						pRenderContext->GetLightController()->GetSpotLights()[i].color.B = (float)(m_pColorPicker->GetCurrentColor().B / 255.0f);

						m_pColorPicker->PreviousColorSet(false);
					}
				}
			}

			m_pApplyButton->Show();	
		}

		if (m_pColorPickerButton->Clicked())
			m_pColorPicker->PreviousColorSet(false);
	}
	else
	{
		m_pColorPicker->PreviousColorSet(false);
	}
}

// GETTERS
bool EditorGUI::EditorMode() const
{
	if (m_GameEditorModeSelect == 0)
		return false;
	else
		return true;
}