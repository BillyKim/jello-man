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
							m_bClick(false)
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

	//SPOTLIGHT BUTTON
	m_pSpotlightButton = new Button(214,7,36,36);

	m_pSpotlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/spotlight_normal.png")));
	m_pSpotlightButtonBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/spotlight_hover.png")));

	m_pSpotlightButton->SetNormalState(m_pSpotlightButtonBitmaps[0]);
	m_pSpotlightButton->SetHoverState(m_pSpotlightButtonBitmaps[1]);
	m_pSpotlightButton->SetDownState(m_pSpotlightButtonBitmaps[1]);

	// CAMERA
	m_pCameraBitmap = new Bitmap(_T("Content/Images/Editor/camera.png"));
}
void EditorGUI::Draw()
{
	BLOX_2D->SetAntiAliasing(false);

	// BACKGROUND
	BLOX_2D->SetColor(70,70,70);
	BLOX_2D->FillRect(0,0,static_cast<int>(BLOX_2D->GetWindowSize().width),50);

	BLOX_2D->SetColor(90,90,90);
	BLOX_2D->DrawLine(0,50,static_cast<int>(BLOX_2D->GetWindowSize().width),50);

	BLOX_2D->SetColor(40,40,40);
	BLOX_2D->FillRect(0,static_cast<int>(BLOX_2D->GetWindowSize().height)-20,static_cast<int>(BLOX_2D->GetWindowSize().width),20);

	// BUTTONS
	m_pLightButton->Show();
	if (m_pLightButton->Hover() || m_pMoveButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Switch between lit and unlit mode."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	m_pMoveButton->Show();
	if (m_pMoveButton->Hover() || m_pMoveButton->Down())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Move objects and lights present in the scene."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	if (m_GameEditorModeSelect == 0)
	{
		m_pGameModeButton->SetPosition(20,7);
		m_pEditorModeButton->SetPosition(20,43);

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

	// CAMERA
	if (m_bUsingCamera)
		BLOX_2D->DrawBitmap(m_pCameraBitmap,static_cast<int>(BLOX_2D->GetWindowSize().width-70),90,0.8f);

	BLOX_2D->SetAntiAliasing(true);
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

	if (m_pPointlightButton->Clicked())
	{
		Vector3 look = pRenderContext->GetCamera()->GetLook();
		look.Normalize();

		PointLight pl;
		pl = PointLight();
		pl.position = (pRenderContext->GetCamera()->GetPosition() + look*200);

		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;

		pl.color = Vector4(r/255.0f, g/255.0f, b/255.0f, 1);
		pl.multiplier = 1.0f;
		pl.AttenuationStart = 0;
		pl.AttenuationEnd = 200;
		pRenderContext->GetLightController()->AddLight(pl);

		cout << "Added pointlight";
	}

	// CAMERA
	if (CONTROLS->RightMBDown())
		m_bUsingCamera = true;
	else
		m_bUsingCamera = false;

	// RENDERCONTEXT
	m_pRenderContext = pRenderContext;

	// VISUAL LIGHT DEBUGGER
	if (pRenderContext->GetLightController()->GetPointLights().size() > m_pHitRectLights.size())
	{
		m_pHitRectLights.push_back(new HitRegion(HitRegion::TYPE_ELLIPSE,0,0,20,20));
		m_LightsSelected.push_back(false);
		m_MPos.push_back(D3DXVECTOR3(0.0f,0.0f,0.0f));
	}

	if (m_GameEditorModeSelect == 1)
		VisualLightDebugger(pRenderContext->GetCamera());
	if (m_pMoveButton->IsActive())
		m_bMoveable = true;
	else 
		m_bMoveable = false;
}

void EditorGUI::VisualLightDebugger(const Camera* pCamera)
{
	// LIGHT DEBUG
	Matrix matProj = pCamera->GetProjection();
	Matrix matView = pCamera->GetView();
	
	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	Matrix ident;
	D3DXMatrixIdentity(&ident);

	int size = 10;

	// POINTLIGHTS
	for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetPointLights().size(); ++i)
	{
		D3DXVECTOR3 pos = m_pRenderContext->GetLightController()->GetPointLights()[i].position.ToD3DVector3();

		Vector3 length = pCamera->GetPosition() - pos;
		float l = length.Length();
		l *= 0.001f;

		// VIEWPORT PROJECTION
		D3DXVECTOR3 temp;
		D3DXVECTOR3 temp2;
		D3DXVECTOR3 temp3;
		D3DXVECTOR3 temp4;

		D3DXVECTOR3 temp5;
		D3DXVECTOR3 temp6;
		D3DXVECTOR3 temp7;

		D3DXVECTOR3 temp8;
		D3DXVECTOR3 temp9;
		D3DXVECTOR3 temp10;
	
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

		D3DXVec3Project(&temp,&pos,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp2,&posLineX,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp3,&posLineY,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp4,&posLineZ,&viewP,&matProj,&matView,&ident);

		D3DXVec3Project(&temp5,&posLineX2,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp6,&posLineY2,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp7,&posLineZ2,&viewP,&matProj,&matView,&ident);

		D3DXVec3Project(&temp8,&posLineXY,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp9,&posLineYZ,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp10,&posLineZX,&viewP,&matProj,&matView,&ident);

		ColorF col(m_pRenderContext->GetLightController()->GetPointLights()[i].color.R,m_pRenderContext->GetLightController()->GetPointLights()[i].color.G,m_pRenderContext->GetLightController()->GetPointLights()[i].color.B,0.4f/l);

		Vector3 lo = pCamera->GetLook();

		if (lo.Dot(length) < 0)
		{
			// HITRECTS
			m_pHitRectLights[i]->SetSize(static_cast<int>(size/l),static_cast<int>(size/l));
			m_pHitRectLights[i]->SetPosition(static_cast<int>(temp.x),static_cast<int>(temp.y));

			BLOX_2D->SetColor(0,0,0,0.4f/l);
			BLOX_2D->SetFont(_T("Arial"),true,false,(size/2)/(l/2));

			// DRAW
			if (m_pHitRectLights[i]->HitTest(CONTROLS->GetMousePos()) || m_LightsSelected[i] == true)
			{
				BLOX_2D->SetColor(255,255,255,0.4f/l);
				BLOX_2D->FillEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l));
				BLOX_2D->SetColor(0,0,0,0.4f/l);
				BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l),3.0f/l);

				BLOX_2D->DrawString(_T("P"),
				RectF(temp.x-((size/l)/2),temp.y-((size/l)/2),temp.x+(size/l)-((size/l)/2),temp.y+(size/l)-((size/l)/2)),
				Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);

				BLOX_2D->SetColor(255,255,255,0.4f/l);
				BLOX_2D->SetFont(_T("Arial"),true,false,(size/4)/(l/2));

				tstringstream s;
				s << _T("PointLight ") << i;
				BLOX_2D->DrawString(s.str(),
				RectF(temp.x-(((size-40)/l)/2),temp.y-(((size+40)/l)/2),temp.x+(size/l)-(((size+40)/l)/2),temp.y+(size/l)-(((size+40)/l)/2)),
				Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);

				BLOX_2D->SetColor(255,255,255);
				BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd/l),static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd/l),2);

				if (m_bMoveable)
				{
					// X
					HitRegion* hitX = new HitRegion(HitRegion::TYPE_ELLIPSE,static_cast<int>(temp2.x),static_cast<int>(temp2.y),size,size);
					BLOX_2D->SetColor(255,0,0);
					BLOX_2D->DrawLine(static_cast<int>(temp5.x),static_cast<int>(temp5.y),static_cast<int>(temp2.x),static_cast<int>(temp2.y),2.0f);
					BLOX_2D->FillEllipse(static_cast<int>(temp2.x),static_cast<int>(temp2.y),static_cast<int>(size/2),static_cast<int>(size/2));
					BLOX_2D->SetColor(255,0,0);
					BLOX_2D->SetFont(_T("Verdana"),false,false,14);
					BLOX_2D->DrawString(_T("X"),static_cast<int>(temp2.x-5),static_cast<int>(temp2.y) - 25);

					D2D1_POINT_2F r[5];
					r[0].x = temp8.x;
					r[0].y = temp8.y;
					r[1].x = temp6.x;
					r[1].y = temp6.y;
					r[2].x = temp.x;
					r[2].y = temp.y;
					r[3].x = temp5.x;
					r[3].y = temp5.y;
					r[4].x = temp8.x;
					r[4].y = temp8.y;

					HitRegion* hitXY = new HitRegion(HitRegion::TYPE_POLYGON,&r[0],5);
						
					if (hitXY->HitTest(CONTROLS->GetMousePos()))
						BLOX_2D->SetColor(100,100,255,0.4f);
					else
						BLOX_2D->SetColor(50,50,255,0.4f);

					BLOX_2D->FillPolygon(r,4);

					BLOX_2D->SetColor(100,100,255,0.8f);
					BLOX_2D->DrawLine(r[0],r[1]);
					BLOX_2D->DrawLine(r[1],r[2]);
					BLOX_2D->DrawLine(r[2],r[3]);
					BLOX_2D->DrawLine(r[3],r[0]);

					if (hitX->HitTest(CONTROLS->GetMousePos()))
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp2.x),static_cast<int>(temp2.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						if (CONTROLS->LeftMBDown() && !m_bLockY && !m_bLockZ)
						{
							m_bLockX = true;
						}
					}

					if (!CONTROLS->LeftMBDown())
					{
						m_bLockX = false;
						m_bLockY = false;
						m_bLockZ = false;
					}

					D3DXVECTOR3 testt(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,temp2.z);
					D3DXVECTOR3 ttempp;
					D3DXVec3Unproject(&ttempp,&testt,&viewP,&matProj,&matView,&ident);
					ttempp.x -= l*100;
					D3DXVECTOR3 ttempp2;
					D3DXVec3Project(&ttempp2,&ttempp,&viewP,&matProj,&matView,&ident);
					D3DXVECTOR3 ttempp3;
					D3DXVec3Unproject(&ttempp3,&ttempp2,&viewP,&matProj,&matView,&ident);

					if (m_bLockX)
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp2.x),static_cast<int>(temp2.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						float difX = m_MPos[i].x - ttempp3.x;
						m_pRenderContext->GetLightController()->GetPointLights()[i].position.X -= difX;

						m_MPos[i].x = ttempp3.x;
					}
					else
					{
						m_MPos[i].x = ttempp3.x;
					}
				}

				if (m_bMoveable)
				{
					// Y
					HitRegion* hitY = new HitRegion(HitRegion::TYPE_ELLIPSE,static_cast<int>(temp3.x),static_cast<int>(temp3.y),size,size);
					BLOX_2D->SetColor(0,255,0);
					BLOX_2D->DrawLine(static_cast<int>(temp6.x),static_cast<int>(temp6.y),static_cast<int>(temp3.x),static_cast<int>(temp3.y),2.0f);
					BLOX_2D->FillEllipse(static_cast<int>(temp3.x),static_cast<int>(temp3.y),static_cast<int>(size/2),static_cast<int>(size/2));
					BLOX_2D->SetColor(0,255,0);
					BLOX_2D->SetFont(_T("Verdana"),false,false,14);
					BLOX_2D->DrawString(_T("Y"),static_cast<int>(temp3.x-5),static_cast<int>(temp3.y) - 25);
					BLOX_2D->SetColor(50,50,255,0.4f);
					D2D1_POINT_2F r[4];
					r[0].x = temp6.x;
					r[0].y = temp6.y;
					r[1].x = temp9.x;
					r[1].y = temp9.y;
					r[2].x = temp7.x;
					r[2].y = temp7.y;
					r[3].x = temp.x;
					r[3].y = temp.y;

					BLOX_2D->FillPolygon(r,4);

					BLOX_2D->SetColor(100,100,255,0.8f);
					BLOX_2D->DrawLine(r[0],r[1]);
					BLOX_2D->DrawLine(r[1],r[2]);
					BLOX_2D->DrawLine(r[2],r[3]);
					BLOX_2D->DrawLine(r[3],r[0]);

					if (hitY->HitTest(CONTROLS->GetMousePos()))
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp3.x),static_cast<int>(temp3.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						if (CONTROLS->LeftMBDown() && !m_bLockX && !m_bLockZ)
						{
							m_bLockY = true;
						}
					}

					D3DXVECTOR3 testt(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,temp3.z);
					D3DXVECTOR3 ttempp;
					D3DXVec3Unproject(&ttempp,&testt,&viewP,&matProj,&matView,&ident);
					ttempp.y -= l*100;
					D3DXVECTOR3 ttempp2;
					D3DXVec3Project(&ttempp2,&ttempp,&viewP,&matProj,&matView,&ident);
					D3DXVECTOR3 ttempp3;
					D3DXVec3Unproject(&ttempp3,&ttempp2,&viewP,&matProj,&matView,&ident);

					if (m_bLockY)
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp3.x),static_cast<int>(temp3.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);
					
						float difY = m_MPos[i].y - ttempp3.y;
						m_pRenderContext->GetLightController()->GetPointLights()[i].position.Y -= difY;

						m_MPos[i].y = ttempp3.y;
					}
					else
					{
						m_MPos[i].y = ttempp3.y;
					}
				}
				if (m_bMoveable)
				{
					// Z
					HitRegion* hitZ = new HitRegion(HitRegion::TYPE_ELLIPSE,static_cast<int>(temp4.x),static_cast<int>(temp4.y),size,size);
					BLOX_2D->SetColor(255,255,0);
					BLOX_2D->DrawLine(static_cast<int>(temp7.x),static_cast<int>(temp7.y),static_cast<int>(temp4.x),static_cast<int>(temp4.y),2.0f);
					BLOX_2D->FillEllipse(static_cast<int>(temp4.x),static_cast<int>(temp4.y),static_cast<int>(size/2),static_cast<int>(size/2));
					BLOX_2D->SetColor(255,255,0);
					BLOX_2D->SetFont(_T("Verdana"),false,false,14);
					BLOX_2D->DrawString(_T("Z"),static_cast<int>(temp4.x-5),static_cast<int>(temp4.y) - 25);
					BLOX_2D->SetColor(50,50,255,0.4f);
					D2D1_POINT_2F r[4];
					r[0].x = temp10.x;
					r[0].y = temp10.y;
					r[1].x = temp5.x;
					r[1].y = temp5.y;
					r[2].x = temp.x;
					r[2].y = temp.y;
					r[3].x = temp7.x;
					r[3].y = temp7.y;

					BLOX_2D->FillPolygon(r,4);

					BLOX_2D->SetColor(100,100,255,0.8f);
					BLOX_2D->DrawLine(r[0],r[1]);
					BLOX_2D->DrawLine(r[1],r[2]);
					BLOX_2D->DrawLine(r[2],r[3]);
					BLOX_2D->DrawLine(r[3],r[0]);

					if (hitZ->HitTest(CONTROLS->GetMousePos()))
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp4.x),static_cast<int>(temp4.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						if (CONTROLS->LeftMBDown() && !m_bLockX && !m_bLockY)
						{
							m_bLockZ = true;
						}
					}

					D3DXVECTOR3 testt(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,temp4.z);
					D3DXVECTOR3 ttempp;
					D3DXVec3Unproject(&ttempp,&testt,&viewP,&matProj,&matView,&ident);
					ttempp.z -= l*100;
					D3DXVECTOR3 ttempp2;
					D3DXVec3Project(&ttempp2,&ttempp,&viewP,&matProj,&matView,&ident);
					D3DXVECTOR3 ttempp3;
					D3DXVec3Unproject(&ttempp3,&ttempp2,&viewP,&matProj,&matView,&ident);

					if (m_bLockZ)
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp4.x),static_cast<int>(temp4.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						float difZ = m_MPos[i].z - ttempp3.z;
						m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z -= difZ;

						m_MPos[i].z = ttempp3.z;
					}
					else
					{
						m_MPos[i].z = ttempp3.z;
					}
				}
			}
			else
			{
				BLOX_2D->SetColor(col);
				BLOX_2D->FillEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>((size/l)));
				BLOX_2D->SetColor(255,255,255,0.4f/l);
				BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l),2.0f);

				BLOX_2D->DrawString(_T("P"),
				RectF(temp.x-((size/l)/2),temp.y-((size/l)/2),temp.x+(size/l)-((size/l)/2),temp.y+(size/l)-((size/l)/2)),
				Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);
			}

		
		}
		else
		{
			// HITRECTS
			m_pHitRectLights[i]->SetSize(1,1);
			m_pHitRectLights[i]->SetPosition(-10,-10);
		}

		if (CONTROLS->LeftMBDown())
		{
			if (!m_bClick)
			{
				for (unsigned int i = 0; i < m_pHitRectLights.size(); ++i)
				{
					if (CONTROLS->IsKeyDown(VK_LCONTROL))
					{
						if (m_pHitRectLights[i]->HitTest(CONTROLS->GetMousePos()))
						{
							m_LightsSelected[i] = !m_LightsSelected[i];
						}
					}
					else
					{
						if (m_pHitRectLights[i]->HitTest(CONTROLS->GetMousePos()))
						{
							m_LightsSelected[i] = !m_LightsSelected[i];
					
							for (unsigned int i2 = 0; i2 < m_pHitRectLights.size(); ++i2)
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

		if (CONTROLS->IsKeyDown(VK_LCONTROL) && CONTROLS->IsKeyDown('D'))
		{
			for (unsigned int i2 = 0; i2 < m_pHitRectLights.size(); ++i2)
			{
				m_LightsSelected[i2] = false;
			}
		}

		// MOVE
		Vector3 look = pCamera->GetLook();
		look.Normalize();
		Vector3 right = pCamera->GetRight();
		right.Normalize();

		if (m_LightsSelected[i] == true && m_bMoveable)
		{
			if (CONTROLS->IsKeyDown(VK_NUMPAD8))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X += look.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z += look.Z*5;
			}
			else if (CONTROLS->IsKeyDown(VK_NUMPAD2))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X -= look.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z -= look.Z*5;
			}
			if (CONTROLS->IsKeyDown(VK_NUMPAD6))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X += right.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z += right.Z*5;
			}
			else if (CONTROLS->IsKeyDown(VK_NUMPAD4))
			{
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.X -= right.X*5;
				m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z -= right.Z*5;
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

	BLOX_2D->SetColor(100,100,100);
	BLOX_2D->FillRect(0,50,150,static_cast<int>(BLOX_2D->GetWindowSize().height));
	BLOX_2D->SetColor(60,60,60);
	BLOX_2D->DrawLine(149,50,149,static_cast<int>(BLOX_2D->GetWindowSize().height),2.0f);

	BLOX_2D->SetColor(255,255,255);
	// DEBUG INFO
	BLOX_2D->SetFont(_T("Verdana"),false,false,10);
	tstringstream stream;
	stream << _T("Nr. lights in scene: ") << m_pRenderContext->GetLightController()->GetPointLights().size() << _T("\n");
	
	int b = 0;
	for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
	{
		if (m_LightsSelected[i] == true)
			++b;
	}

	stream << _T("Nr. lights selected: ") << b << _T("\n-------------------------\n\n");

	for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
	{
		if (m_LightsSelected[i] == true)
		{
			stream << _T("PointLight ") << i << _T(":\n   ");
			tstringstream strm;
			strm << m_pRenderContext->GetLightController()->GetPointLights()[i].position.X;
			tstring t = strm.str();
			stream << _T("X: ") << t.substr(0, t.find(_T(".")) + 4) << _T("\n   ");
			tstringstream strm1;
			strm1 << m_pRenderContext->GetLightController()->GetPointLights()[i].position.Y;
			t = strm1.str();
			stream << _T("Y: ") << t.substr(0, t.find(_T(".")) + 4) << _T("\n   ");
			tstringstream strm2;
			strm2 << m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z;
			t = strm2.str();
			stream << _T("Z: ") << t.substr(0, t.find(_T(".")) + 4) << _T("\n   ");
			stream << _T("R:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.R*255) << _T(" ");
			stream << _T("G:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.G*255) << _T(" ");
			stream << _T("B:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.B*255) << _T("\n   ");
			stream << _T("Multiplier: ") << m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier << _T("\n   ");
			stream << _T("Attenuation end: ") << m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd << _T("\n\n");
		}
	}

	BLOX_2D->DrawString(stream.str(),2,60);
}

// GETTERS
bool EditorGUI::EditorMode() const
{
	if (m_GameEditorModeSelect == 0)
		return false;
	else
		return true;
}