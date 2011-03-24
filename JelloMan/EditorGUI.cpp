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
							m_bLockX(false),
							m_bLockY(false),
							m_bLockZ(false),
							m_bMoveable(false),
							m_bClick(false),
							m_pColorPickerButton(0),
							m_bPreviousColorSet(false),
							m_pApplyButton(0),
							m_TotalLights(0),
							m_SelectedLights(0)
{
	m_PreviousColor = Vector3(0.0f,0.0f,0.0f);
	m_CurrentColor = Vector3(0.0f,0.0f,0.0f);
	m_ColorPickerPos = Point2F(160,60.0f);
	m_HueColor = Vector3(255.0f,0.0f,0.0f);
	m_ColorPickerSelectPos = Point2F(0.0f,0.0f);
	m_Hue = 0;
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

	for (vector<Bitmap*>::iterator it = m_pPointLightBitmaps.begin(); it != m_pPointLightBitmaps.end(); ++it)
		delete (*it);

	delete m_pCameraBitmap;
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
	m_pPointlightButton = new Button(180,7,36,36);

	m_pPointlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/pointlight_normal.png")));
	m_pPointlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/pointlight_hover.png")));

	m_pPointlightButton->SetNormalState(m_pPointlightButtonBitmaps[0]);
	m_pPointlightButton->SetHoverState(m_pPointlightButtonBitmaps[1]);
	m_pPointlightButton->SetDownState(m_pPointlightButtonBitmaps[1]);

	// SPOTLIGHT BUTTON
	m_pSpotlightButton = new Button(214,7,36,36);

	m_pSpotlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/spotlight_normal.png")));
	m_pSpotlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/spotlight_hover.png")));

	m_pSpotlightButton->SetNormalState(m_pSpotlightButtonBitmaps[0]);
	m_pSpotlightButton->SetHoverState(m_pSpotlightButtonBitmaps[1]);
	m_pSpotlightButton->SetDownState(m_pSpotlightButtonBitmaps[1]);

	// COLOR PICKER BUTTON
	m_pColorPickerButton = new Button(274,7,36,36,true);

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
	m_pApplyButton = new Button((int)(m_ColorPickerPos.x + 360),(int)(m_ColorPickerPos.y+220),36,36);

	m_pApplyButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/apply_normal.png")));
	m_pApplyButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/apply_hover.png")));

	m_pApplyButton->SetNormalState(m_pApplyButtonBitmaps[0]);
	m_pApplyButton->SetHoverState(m_pApplyButtonBitmaps[1]);
	m_pApplyButton->SetDownState(m_pApplyButtonBitmaps[1]);

	// POINT LIGHT
	m_pPointLightBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/plight_normal.png")));
	m_pPointLightBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/plight_hover.png")));

	// CAMERA
	m_pCameraBitmap = new Bitmap(_T("Content/Images/Editor/camera.png"));
}
void EditorGUI::Draw()
{
	if (m_GameEditorModeSelect == 1)
		VisualLightDebugger(m_pRenderContext->GetCamera());

	BLOX_2D->SetAntiAliasing(false);

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

	m_pColorPickerButton->Show();
	if (m_pColorPickerButton->Hover() || m_pColorPickerButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Change the color of the selected light."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	// CAMERA
	if (m_bUsingCamera)
		BLOX_2D->DrawBitmap(m_pCameraBitmap,static_cast<int>(BLOX_2D->GetWindowSize().width-70),90,0.8f);

	BLOX_2D->SetAntiAliasing(true);

	BLOX_2D->SetColor(255,255,255,0.8f);
	BLOX_2D->SetFont(_T("Verdana"),false,false,10);

	tstringstream stream;
	stream << _T("Lights: ") << m_SelectedLights << _T(" / ") << m_TotalLights;
	BLOX_2D->DrawString(
		stream.str(),
		RectF(0.0f, 0.0f,
		BLOX_2D->GetWindowSize().width - 4,
		BLOX_2D->GetWindowSize().height - 4),
		Blox2D::HORIZONTAL_ALIGN_RIGHT,
		Blox2D::VERTICAL_ALIGN_BOTTOM);
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
	m_pColorPickerButton->Tick();	

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
		pRenderContext->GetLightController()->AddLight(pl);

		cout << "Added pointlight\n";
	}

	// CAMERA
	if (CONTROLS->RightMBDown())
		m_bUsingCamera = true;
	else
		m_bUsingCamera = false;

	// RENDERCONTEXT
	m_pRenderContext = pRenderContext;

	// VISUAL LIGHT DEBUGGER
	if (pRenderContext->GetLightController()->GetPointLights().size() > m_LightsSelected.size())
	{
		m_pLightHitRects.push_back(new HitRegion(HitRegion::TYPE_ELLIPSE, -10 ,-10, 10, 10));
		m_LightsSelected.push_back(false);
		m_MPos.push_back(D3DXVECTOR3(0.0f,0.0f,0.0f));
	}

	if (m_pMoveButton->IsActive())
		m_bMoveable = true;
	else 
		m_bMoveable = false;

	if (m_pColorPickerButton->IsActive() && m_GameEditorModeSelect == 1)
	{
		int b = 0;
		for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
		{
			if (m_LightsSelected[i] == true)
				++b;
		}

		if (b == 1)
		{
			m_pApplyButton->Tick();
		
			for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
			{
				if (m_LightsSelected[i] == true)
				{
					ColorPicker(&pRenderContext->GetLightController()->GetPointLights()[i]);
					
					if (m_pApplyButton->Clicked())
					{
						pRenderContext->GetLightController()->GetPointLights()[i].color.R = (float)(m_CurrentColor.X / 255.0f);
						pRenderContext->GetLightController()->GetPointLights()[i].color.G = (float)(m_CurrentColor.Y / 255.0f);
						pRenderContext->GetLightController()->GetPointLights()[i].color.B = (float)(m_CurrentColor.Z / 255.0f);

						m_bPreviousColorSet = false;
					}
				}
			}

			m_pApplyButton->Show();	
		}

		if (m_pColorPickerButton->Clicked())
			m_bPreviousColorSet = false;
	}
	else
	{
		m_bPreviousColorSet = false;
	}
}

void EditorGUI::VisualLightDebugger(const Camera* pCamera)
{
	// MATRIX
	D3DXMATRIX matProj = pCamera->GetProjection();
	D3DXMATRIX matView = pCamera->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	Vector3 vLook = pCamera->GetLook();
	Vector3 vRight = pCamera->GetRight();

	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	int size = 10;

	// POINTLIGHTS
	for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetPointLights().size(); ++i)
	{
		D3DXVECTOR3 pos = m_pRenderContext->GetLightController()->GetPointLights()[i].position.ToD3DVector3();
	
		Vector3 length = pCamera->GetPosition() - Vector3(pos);
		float l = length.Length();
		l *= 0.001f;

		// VIEWPORT PROJECTION
		D3DXVECTOR3 pos2D;
		D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

		ColorF col(
			m_pRenderContext->GetLightController()->GetPointLights()[i].color.R
			,m_pRenderContext->GetLightController()->GetPointLights()[i].color.G,
			m_pRenderContext->GetLightController()->GetPointLights()[i].color.B,
			0.4f / l);

		// HITRECT
		SafeDelete(m_pLightHitRects[i]);
		m_pLightHitRects[i] = new HitRegion(	
			HitRegion::TYPE_ELLIPSE,
			static_cast<float>(pos2D.x),
			static_cast<float>(pos2D.y), 
			static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (16 * l)),
			static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (16 * l)));

		if (vLook.Dot(length) < 0)
		{
			BLOX_2D->SetColor(0, 0, 0, 0.4f / l);
			BLOX_2D->SetFont(_T("Arial"), true, false, (size / 2) / (l / 2));

			// DRAW
			if (m_pLightHitRects[i]->HitTest(CONTROLS->GetMousePos()) || m_LightsSelected[i] == true)
			{
				/*BLOX_2D->SetColor(255, 255, 255, 0.4f / l);
				BLOX_2D->FillEllipse(
					static_cast<int>(pos2D.x),
					static_cast<int>(pos2D.y),
					static_cast<int>(size/l),
					static_cast<int>(size/l));

				BLOX_2D->SetColor(0,0,0,0.4f/l);
				BLOX_2D->DrawEllipse(
					static_cast<int>(pos2D.x),
					static_cast<int>(pos2D.y),
					static_cast<int>(size / l),
					static_cast<int>(size / l),
					3.0f / l);*/

				BLOX_2D->DrawBitmap(m_pPointLightBitmaps[1],
									static_cast<int>(pos2D.x - ((m_pPointLightBitmaps[0]->GetSize().width / (8 * l)) / 2)),
									static_cast<int>(pos2D.y - (m_pPointLightBitmaps[0]->GetSize().height / (8 * l)) / 2),
									1.0f / l,
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (8 * l)),
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (8 * l)));

				// MOVE GIZMO
				if (m_bMoveable && m_LightsSelected[i] == true)
					MoveGizmo(&m_pRenderContext->GetLightController()->GetPointLights()[i],i);
			}
			else
			{
				/*BLOX_2D->SetColor(col);
				BLOX_2D->FillEllipse(
					static_cast<int>(pos2D.x),
					static_cast<int>(pos2D.y),
					static_cast<int>((size) / (l)),
					static_cast<int>(((size) / (l))));

				BLOX_2D->SetColor(255, 255, 255, 0.4f / l);
				BLOX_2D->DrawEllipse(
					static_cast<int>(pos2D.x),
					static_cast<int>(pos2D.y),
					static_cast<int>(size / l),
					static_cast<int>(size / l),
					1/l);*/

				BLOX_2D->DrawBitmap(m_pPointLightBitmaps[0],
									static_cast<int>(pos2D.x - ((m_pPointLightBitmaps[0]->GetSize().width / (8 * l)) / 2)),
									static_cast<int>(pos2D.y - (m_pPointLightBitmaps[0]->GetSize().height / (8 * l)) / 2),
									1.0f / l,
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (8 * l)),
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (8 * l)));
			}
		}

		if (CONTROLS->IsKeyDown(VK_LCONTROL) && CONTROLS->IsKeyDown('D'))
		{
			for (unsigned int i2 = 0; i2 < m_LightsSelected.size(); ++i2)
			{
				m_LightsSelected[i2] = false;
			}
		}

		// MOVE
		vLook.Normalize();
		vRight.Normalize();

		if (m_LightsSelected[i] == true && m_bMoveable)
		{
			if (CONTROLS->IsKeyDown(VK_NUMPAD8))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X += vLook.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z += vLook.Z*5;
			}
			else if (CONTROLS->IsKeyDown(VK_NUMPAD2))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X -= vLook.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z -= vLook.Z*5;
			}
			if (CONTROLS->IsKeyDown(VK_NUMPAD6))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X += vRight.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z += vRight.Z*5;
			}
			else if (CONTROLS->IsKeyDown(VK_NUMPAD4))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X -= vRight.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z -= vRight.Z*5;
			}
			if (CONTROLS->IsKeyDown(VK_NUMPAD9))
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Y += 2;
			else if (CONTROLS->IsKeyDown(VK_NUMPAD7))
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Y -= 2;
		}
		
		if (m_LightsSelected[i] == true)
		{
			if (CONTROLS->IsKeyDown(VK_ADD))
				m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier += 0.1f;

			else if (CONTROLS->IsKeyDown(VK_SUBTRACT))
				m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier -= 0.1f;

			if (CONTROLS->IsKeyDown(VK_NUMPAD3))				
				m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd += 20;

			else if (CONTROLS->IsKeyDown(VK_NUMPAD1))
				if (m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd > 0)
				{
					m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd -= 20;
				}
		}
	}

	// CONTROLS
	if (CONTROLS->LeftMBDown())
	{
		if (!m_bClick)
		{
			for (unsigned int i = 0; i < m_pLightHitRects.size(); ++i)
			{
				if (CONTROLS->IsKeyDown(VK_LCONTROL))
				{
					if (m_pLightHitRects[i]->HitTest(CONTROLS->GetMousePos()))
						m_LightsSelected[i] = !m_LightsSelected[i];
				}
				else
				{
					if (m_pLightHitRects[i]->HitTest(CONTROLS->GetMousePos()))
					{
						m_LightsSelected[i] = !m_LightsSelected[i];
						m_bPreviousColorSet = false;
					
						for (unsigned int i2 = 0; i2 < m_LightsSelected.size(); ++i2)
						{
							if (i != i2)
								m_LightsSelected[i2] = false;
						}
					}
				}
			}

		m_bClick = true;

		}
	}
	else
		m_bClick = false;

	BLOX_2D->SetColor(100, 100, 100);
	BLOX_2D->FillRect(0, 50, 150, static_cast<int>(BLOX_2D->GetWindowSize().height)-70);

	BLOX_2D->SetColor(60, 60, 60);
	BLOX_2D->DrawLine(149, 50, 149, static_cast<int>(BLOX_2D->GetWindowSize().height)-20, 2.0f);

	// DEBUG INFO
	BLOX_2D->SetColor(255, 255, 255);
	BLOX_2D->SetFont(_T("Verdana"),false,false,10);

	int b = 0;
	for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
	{
		if (m_LightsSelected[i] == true)
			++b;
	}

	m_SelectedLights = b;
	m_TotalLights = m_pRenderContext->GetLightController()->GetPointLights().size();

	tstringstream stream2;

	for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
	{
		if (m_LightsSelected[i] == true)
		{
			stream2 << _T("PointLight ") << i << _T(":\n   ");
			tstringstream strm;
			strm << m_pRenderContext->GetLightController()->GetPointLights()[i].position.X;
			tstring t = strm.str();
			stream2 << _T("X: ") << t.substr(0, t.find(_T(".")) + 4) << _T("\n   ");

			tstringstream strm1;
			strm1 << m_pRenderContext->GetLightController()->GetPointLights()[i].position.Y;
			t = strm1.str();
			stream2 << _T("Y: ") << t.substr(0, t.find(_T(".")) + 4) << _T("\n   ");

			tstringstream strm2;
			strm2 << m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z;
			t = strm2.str();
			stream2 << _T("Z: ") << t.substr(0, t.find(_T(".")) + 4) << _T("\n   ");

			stream2 << _T("R:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.R * 255) << _T(" ");
			stream2 << _T("G:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.G * 255) << _T(" ");
			stream2 << _T("B:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.B * 255) << _T("\n   ");

			stream2 << _T("Multiplier: ") << m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier << _T("\n   ");
			stream2 << _T("Attenuation end: ") << m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd << _T("\n\n");
		}
	}

	BLOX_2D->DrawString(stream2.str(),2,60);
}

void EditorGUI::ColorPicker(PointLight* pointLight)
{
	if (!m_bPreviousColorSet)
	{
		m_PreviousColor.X = (pointLight->color.R * 255);
		m_PreviousColor.Y = (pointLight->color.G * 255);
		m_PreviousColor.Z = (pointLight->color.B * 255);

		m_CurrentColor = m_PreviousColor;

		float *h,*s, *v;
		h = new float(0.0f);
		s = new float(0.0f);
		v = new float(0.0f);

		RGBtoHSV(pointLight->color.R, pointLight->color.G, pointLight->color.B, h, s, v);

		m_HueColor.X = GetHue((int)((255 - ((*h / 360) * 255)))).X;
		m_HueColor.Y = GetHue((int)((255 - ((*h / 360) * 255)))).Y;
		m_HueColor.Z = GetHue((int)((255 - ((*h / 360) * 255)))).Z;

		m_Hue = (int)(*h);

		m_ColorPickerSelectPos = Point2F(m_ColorPickerPos.x + (*s * 255), m_ColorPickerPos.y + (255 - (*v * 255)));

		delete h;
		delete s;
		delete v;

		m_bPreviousColorSet = true;
	}

	// DRAW
	BLOX_2D->SetAntiAliasing(false);

	BLOX_2D->SetColor(43, 43, 43);
	BLOX_2D->FillRect((int)(m_ColorPickerPos.x - 5), (int)(m_ColorPickerPos.y - 5), 405, 265);
	BLOX_2D->SetColor(120, 120, 120);
	BLOX_2D->DrawRect((int)(m_ColorPickerPos.x - 5), (int)(m_ColorPickerPos.y - 5), 405, 265);

	HitRegion hHitRect(
		HitRegion::TYPE_RECTANGLE,
		(int)(m_ColorPickerPos.x + 280),
		(int)(m_ColorPickerPos.y + 1),
		20,
		255);

	if (hHitRect.HitTest(CONTROLS->GetMousePos()) && CONTROLS->LeftMBDown())
	{
		m_HueColor.X = GetHue((int)(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)).X;
		m_HueColor.Y = GetHue((int)(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)).Y;
		m_HueColor.Z = GetHue((int)(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)).Z;

		m_Hue = (int)(((255-(CONTROLS->GetMousePos().y - m_ColorPickerPos.y))/255.0f)*360.0f);

		Vector3 rgb = HsvToRgb(m_Hue,(double)((m_ColorPickerSelectPos.x - m_ColorPickerPos.x)/255.0f), (double)((255-(m_ColorPickerSelectPos.y - m_ColorPickerPos.y))/255.0f));

		m_CurrentColor = rgb;
	}

	BLOX_2D->SetColor((int)m_HueColor.X, (int)m_HueColor.Y, (int)m_HueColor.Z);
	BLOX_2D->FillRect((int)m_ColorPickerPos.x,(int)m_ColorPickerPos.y,255,255);

	HitRegion cHitRect(
		HitRegion::TYPE_RECTANGLE,
		(int)m_ColorPickerPos.x,
		(int)m_ColorPickerPos.y,
		255,
		255);

	if (cHitRect.HitTest(CONTROLS->GetMousePos()) && CONTROLS->LeftMBDown())
	{
		Vector3 rgb = HsvToRgb(m_Hue,(double)((CONTROLS->GetMousePos().x - m_ColorPickerPos.x)/255.0f), (double)((255-(CONTROLS->GetMousePos().y - m_ColorPickerPos.y))/255.0f));

		m_CurrentColor = rgb;

		m_ColorPickerSelectPos = Point2F((CONTROLS->GetMousePos().x - m_ColorPickerPos.x)+m_ColorPickerPos.x,(CONTROLS->GetMousePos().y - m_ColorPickerPos.y)+m_ColorPickerPos.y);
	}

	for (int i = 0; i < 255; ++i)
	{
		BLOX_2D->SetColor(255,255,255,(255-i)/255.0f);

		BLOX_2D->DrawLine((int)(m_ColorPickerPos.x+1+i),(int)m_ColorPickerPos.y,(int)(m_ColorPickerPos.x+1+i),(int)(m_ColorPickerPos.y+255));
	}

	for (int i = 0; i < 255; ++i)
	{
		BLOX_2D->SetColor(0,0,0,i/255.0f);

		BLOX_2D->DrawLine((int)m_ColorPickerPos.x,(int)(m_ColorPickerPos.y+i+1),(int)(m_ColorPickerPos.x+255),(int)(m_ColorPickerPos.y+i+1));
	}

	for (int i = 0; i < 255; ++i)
	{
		BLOX_2D->SetColor((int)GetHue(i).X,(int)GetHue(i).Y,(int)GetHue(i).Z);

		BLOX_2D->DrawLine((int)(m_ColorPickerPos.x+280),(int)(m_ColorPickerPos.y+i+1),(int)(m_ColorPickerPos.x+300),(int)(m_ColorPickerPos.y+i+1));
	}

	BLOX_2D->SetColor((int)m_PreviousColor.X, (int)m_PreviousColor.Y, (int)m_PreviousColor.Z);
	BLOX_2D->FillRect((int)(m_ColorPickerPos.x+310),(int)(m_ColorPickerPos.y),80,40);

	BLOX_2D->SetColor((int)m_CurrentColor.X, (int)m_CurrentColor.Y, (int)m_CurrentColor.Z);
	BLOX_2D->FillRect((int)(m_ColorPickerPos.x+310),(int)(m_ColorPickerPos.y+45),80,40);

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->DrawEllipse((int)m_ColorPickerSelectPos.x,(int)m_ColorPickerSelectPos.y,5,5);
	BLOX_2D->SetColor(0,0,0);
	BLOX_2D->DrawEllipse((int)m_ColorPickerSelectPos.x,(int)m_ColorPickerSelectPos.y,6,6);

	D2D1_POINT_2F r[4];
	r[0].x = m_ColorPickerPos.x+273;
	r[0].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1-5);
	r[1].x = m_ColorPickerPos.x+278;
	r[1].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1);
	r[2].x = m_ColorPickerPos.x+273;
	r[2].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1+5);
	r[3].x = m_ColorPickerPos.x+273;
	r[3].y = ((255-((m_Hue/360.0f)*255))+m_ColorPickerPos.y-1-5);

	BLOX_2D->SetColor(255,255,255);
	BLOX_2D->FillPolygon(r,4);

	tstringstream strm;
	strm << _T("R: ") << (int)m_CurrentColor.X << _T("\n");
	strm << _T("G: ") << (int)m_CurrentColor.Y << _T("\n");
	strm << _T("B: ") << (int)m_CurrentColor.Z << _T("\n");
	
	BLOX_2D->SetFont(_T("Verdana"),false,false,10);
	BLOX_2D->DrawString(strm.str(),(int)(m_ColorPickerPos.x+310),(int)(m_ColorPickerPos.y+90));

	BLOX_2D->SetAntiAliasing(true);
}

Vector3 EditorGUI::GetHue(int i)
{
	if (i < 255/6)
		return Vector3(255.0f,0.0f,i*6.0f);
	else if (i < 255/3)
		return Vector3(255.0f-((i-(255.0f/6.0f))*6.0f),0.0f,255.0f);
	else if (i < 255/2)
		return Vector3(0.0f,((i-(255.0f/3.0f))*6.0f),255.0f);
	else if (i < ((255*2)/3))
		return Vector3(0.0f,255.0f,255.0f-((i-(255.0f/2.0f))*6.0f));
	else if (i < ((255*5)/6))
		return Vector3(((i-((255.0f*2.0f)/3.0f))*6.0f),255.0f,0.0f);
	else
		return Vector3(255.0f,255.0f-((i-((255.0f*5.0f)/6.0f))*6.0f),0.0f);
}

Vector3 EditorGUI::HsvToRgb(double h, double S, double V)
{
  // ######################################################################
  // T. Nathan Mundhenk
  // mundhenk@usc.edu
  // C/C++ Macro HSV to RGB

  double H = h;
  while (H < 0) { H += 360; };
  while (H >= 360) { H -= 360; };
  double R, G, B;
  if (V <= 0)
    { R = G = B = 0; }
  else if (S <= 0)
  {
    R = G = B = V;
  }
  else
  {
    double hf = H / 60.0;
    int i = (int)hf;
    double f = hf - i;
    double pv = V * (1 - S);
    double qv = V * (1 - S * f);
    double tv = V * (1 - S * (1 - f));
    switch (i)
    {
      // Red is the dominant color
      case 0:
        R = V;
        G = tv;
        B = pv;
        break;
      // Green is the dominant color
      case 1:
        R = qv;
        G = V;
        B = pv;
        break;
      case 2:
        R = pv;
        G = V;
        B = tv;
        break;
      // Blue is the dominant color
      case 3:
        R = pv;
        G = qv;
        B = V;
        break;
      case 4:
        R = tv;
        G = pv;
        B = V;
        break;
      // Red is the dominant color
      case 5:
        R = V;
        G = pv;
        B = qv;
        break;
      // Just in case we overshoot on our math by a little, we put these here. Since its a switch it won't slow us down at all to put these here.
      case 6:
        R = V;
        G = tv;
        B = pv;
        break;
      case -1:
        R = V;
        G = pv;
        B = qv;
        break;
      // The color is not defined, we should throw an error.
      default:
        //LFATAL("i Value error in Pixel conversion, Value is %d", i);
        R = G = B = V; // Just pretend its black/white
        break;
    }
  }

  Vector3 RGB;
  RGB.X = (float)(R * 255.0f);
  RGB.Y = (float)(G * 255.0f);
  RGB.Z = (float)(B * 255.0f);

  return RGB;
}

void EditorGUI::RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;

	max = r;
	if (g > r)
		max = g;
	if (b > g)
		max = b;

	min = r;
	if (g < r)
		min = g;
	if (b < g)
		min = b;

	*v = max;				// v

	delta = max - min;

	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan

	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;
}

void EditorGUI::MoveGizmo(PointLight* pointLight, int id)
{
	// MATRIX
	D3DXMATRIX matProj = m_pRenderContext->GetCamera()->GetProjection();
	D3DXMATRIX matView = m_pRenderContext->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	// VIEWPORT
	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	// POSITION
	D3DXVECTOR3 pos = pointLight->position.ToD3DVector3();

	Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - Vector3(pos);
	float l = length.Length();
	l *= 0.001f;

	// VIEWPORT PROJECTION
	D3DXVECTOR3 pos2D;
	D3DXVECTOR3 posLineX_2D;
	D3DXVECTOR3 posLineY_2D;
	D3DXVECTOR3 posLineZ_2D;

	D3DXVECTOR3 posLineX2_2D;
	D3DXVECTOR3 posLineY2_2D;
	D3DXVECTOR3 posLineZ2_2D;

	D3DXVECTOR3 posLineXY_2D;
	D3DXVECTOR3 posLineYZ_2D;
	D3DXVECTOR3 posLineZX_2D;
	
	D3DXVECTOR3 posLineX = pos;
	posLineX.x += l*100;
	D3DXVECTOR3 posLineY = pos;
	posLineY.y += l*100;
	D3DXVECTOR3 posLineZ = pos;
	posLineZ.z += l*100;

	D3DXVECTOR3 posLineX2 = pos;
	posLineX2.x += l*50;
	D3DXVECTOR3 posLineY2 = pos;
	posLineY2.y += l*50;
	D3DXVECTOR3 posLineZ2 = pos;
	posLineZ2.z += l*50;

	D3DXVECTOR3 posLineXY = pos;
	posLineXY.x += l*50;
	posLineXY.y += l*50;
	D3DXVECTOR3 posLineYZ = pos;
	posLineYZ.y += l*50;
	posLineYZ.z += l*50;
	D3DXVECTOR3 posLineZX = pos;
	posLineZX.z += l*50;
	posLineZX.x += l*50;

	D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

	D3DXVec3Project(&posLineX_2D, &posLineX, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineY_2D, &posLineY, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineZ_2D, &posLineZ, &viewP, &matProj, &matView, &matIdent);

	D3DXVec3Project(&posLineX2_2D, &posLineX2, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineY2_2D, &posLineY2, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineZ2_2D, &posLineZ2, &viewP, &matProj, &matView, &matIdent);

	D3DXVec3Project(&posLineXY_2D, &posLineXY, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineYZ_2D, &posLineYZ, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineZX_2D, &posLineZX, &viewP, &matProj, &matView, &matIdent);

	int size = 10;

	// X
	HitRegion hitRectX(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineX_2D.x),
		static_cast<int>(posLineX_2D.y),
		size,size);

	BLOX_2D->SetColor(255, 0, 0);
	BLOX_2D->DrawLine(
		static_cast<int>(posLineX2_2D.x),
		static_cast<int>(posLineX2_2D.y),
		static_cast<int>(posLineX_2D.x),
		static_cast<int>(posLineX_2D.y),
		2.0f);

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineX_2D.x),
		static_cast<int>(posLineX_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));
	
	BLOX_2D->SetColor(255, 0, 0);
	BLOX_2D->SetFont(_T("Verdana"), false, false, 14);
	BLOX_2D->DrawString(_T("X"),
		static_cast<int>(posLineX_2D.x-5),
		static_cast<int>(posLineX_2D.y) - 25);

	// XY
	D2D1_POINT_2F r[5];
	r[0].x = posLineXY_2D.x;
	r[0].y = posLineXY_2D.y;
	r[1].x = posLineY2_2D.x;
	r[1].y = posLineY2_2D.y;
	r[2].x = pos2D.x;
	r[2].y = pos2D.y;
	r[3].x = posLineX2_2D.x;
	r[3].y = posLineX2_2D.y;
	r[4].x = posLineXY_2D.x;
	r[4].y = posLineXY_2D.y;

	HitRegion hitRectXY(HitRegion::TYPE_POLYGON, &r[0], 5);
						
	if (hitRectXY.HitTest(CONTROLS->GetMousePos()))
		BLOX_2D->SetColor(100, 100, 255, 0.4f);
	else
		BLOX_2D->SetColor(50, 50, 255, 0.4f);

	BLOX_2D->FillPolygon(r, 4);

	BLOX_2D->SetColor(100, 100, 255, 0.8f);
	BLOX_2D->DrawLine(r[0], r[1]);
	BLOX_2D->DrawLine(r[1], r[2]);
	BLOX_2D->DrawLine(r[2], r[3]);
	BLOX_2D->DrawLine(r[3], r[0]);

	if (hitRectX.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255, 255, 255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineX_2D.x),
			static_cast<int>(posLineX_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);

		if (CONTROLS->LeftMBDown() && !m_bLockY && !m_bLockZ)
			m_bLockX = true;
	}

	if (!CONTROLS->LeftMBDown())
		m_bLockX = m_bLockY = m_bLockZ = false;

	if (m_bLockX)
	{
		D3DXVECTOR3 mousePosPlusZ(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineX_2D.z);
		D3DXVECTOR3 mousePosPlusZ_3D;
		D3DXVec3Unproject(&mousePosPlusZ_3D, &mousePosPlusZ, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZ_3D.x -= l*100;

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineX_2D.x),
			static_cast<int>(posLineX_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);

		float diff = m_MPos[id].x - mousePosPlusZ_3D.x;

		pointLight->position.X -= diff;

		m_MPos[id] = mousePosPlusZ_3D;
	}

	// Y
	HitRegion hitRectY(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineY_2D.x),
		static_cast<int>(posLineY_2D.y),
		size,
		size);

	BLOX_2D->SetColor(0, 255, 0);
	BLOX_2D->DrawLine(static_cast<int>(posLineY2_2D.x),
		static_cast<int>(posLineY2_2D.y),
		static_cast<int>(posLineY_2D.x),
		static_cast<int>(posLineY_2D.y),
		2.0f);

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineY_2D.x),
		static_cast<int>(posLineY_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	BLOX_2D->SetColor(0, 255, 0);
	BLOX_2D->SetFont(_T("Verdana"), false, false, 14);
	BLOX_2D->DrawString(_T("Y"),
		static_cast<int>(posLineY_2D.x-5),
		static_cast<int>(posLineY_2D.y) - 25);
	
	BLOX_2D->SetColor(50, 50, 255, 0.4f);
	//D2D1_POINT_2F r[4];
	r[0].x = posLineY2_2D.x;
	r[0].y = posLineY2_2D.y;
	r[1].x = posLineYZ_2D.x;
	r[1].y = posLineYZ_2D.y;
	r[2].x = posLineZ2_2D.x;
	r[2].y = posLineZ2_2D.y;
	r[3].x = pos2D.x;
	r[3].y = pos2D.y;

	BLOX_2D->FillPolygon(r,4);

	BLOX_2D->SetColor(100, 100, 255, 0.8f);
	BLOX_2D->DrawLine(r[0],r[1]);
	BLOX_2D->DrawLine(r[1],r[2]);
	BLOX_2D->DrawLine(r[2],r[3]);
	BLOX_2D->DrawLine(r[3],r[0]);

	if (hitRectY.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineY_2D.x),
			static_cast<int>(posLineY_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);

		if (CONTROLS->LeftMBDown() && !m_bLockX && !m_bLockZ)
		{
			m_bLockY = true;
		}
	}

	if (m_bLockY)
	{
		D3DXVECTOR3 mousePosPlusZ(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineY_2D.z);
		D3DXVECTOR3 mousePosPlusZ_3D;
		D3DXVec3Unproject(&mousePosPlusZ_3D, &mousePosPlusZ, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZ_3D.y -= l*100;

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineY_2D.x),
			static_cast<int>(posLineY_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);
					
		float diff = m_MPos[id].y - mousePosPlusZ_3D.y;

		pointLight->position.Y -= diff;

		m_MPos[id] = mousePosPlusZ_3D;
	}
	
	// Z
	HitRegion hitRectZ(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineZ_2D.x),
		static_cast<int>(posLineZ_2D.y),
		size,
		size);

	BLOX_2D->SetColor(255, 255, 0);
	BLOX_2D->DrawLine(
		static_cast<int>(posLineZ2_2D.x),
		static_cast<int>(posLineZ2_2D.y),
		static_cast<int>(posLineZ_2D.x),
		static_cast<int>(posLineZ_2D.y),
		2.0f);

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineZ_2D.x),
		static_cast<int>(posLineZ_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	BLOX_2D->SetColor(255, 255, 0);
	BLOX_2D->SetFont(_T("Verdana"), false, false, 14);
	BLOX_2D->DrawString(_T("Z"),
		static_cast<int>(posLineZ_2D.x-5),
		static_cast<int>(posLineZ_2D.y) - 25);

	BLOX_2D->SetColor(50, 50, 255, 0.4f);
	//D2D1_POINT_2F r[4];
	r[0].x = posLineZX_2D.x;
	r[0].y = posLineZX_2D.y;
	r[1].x = posLineX2_2D.x;
	r[1].y = posLineX2_2D.y;
	r[2].x = pos2D.x;
	r[2].y = pos2D.y;
	r[3].x = posLineZ2_2D.x;
	r[3].y = posLineZ2_2D.y;

	BLOX_2D->FillPolygon(r,4);

	BLOX_2D->SetColor(100, 100, 255, 0.8f);
	BLOX_2D->DrawLine(r[0],r[1]);
	BLOX_2D->DrawLine(r[1],r[2]);
	BLOX_2D->DrawLine(r[2],r[3]);
	BLOX_2D->DrawLine(r[3],r[0]);

	if (hitRectZ.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineZ_2D.x),
			static_cast<int>(posLineZ_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);

		if (CONTROLS->LeftMBDown() && !m_bLockX && !m_bLockY)
		{
			m_bLockZ = true;
		}
	}

	if (m_bLockZ)
	{
		D3DXVECTOR3 mousePosPlusZ(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineZ_2D.z);
		D3DXVECTOR3 mousePosPlusZ_3D;
		D3DXVec3Unproject(&mousePosPlusZ_3D, &mousePosPlusZ, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZ_3D.z -= l*100;

		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineZ_2D.x),
			static_cast<int>(posLineZ_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);

		float diff = m_MPos[id].z - mousePosPlusZ_3D.z;

		pointLight->position.Z -= diff;

		m_MPos[id] = mousePosPlusZ_3D;
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