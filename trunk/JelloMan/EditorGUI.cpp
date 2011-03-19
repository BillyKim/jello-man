#include "EditorGUI.h"


// CONSTRUCTOR - DESTRUCTOR
EditorGUI::EditorGUI()	:	m_pLightButton(0),
							m_pCameraBitmap(0),
							m_bUsingCamera(false),
							m_pMoveButton(0)
{
}

EditorGUI::~EditorGUI()
{
	delete m_pLightButton;
	for (int i = 0; i < m_pLightButtonBitmaps.size(); ++i)
		delete m_pLightButtonBitmaps[i];

	delete m_pMoveButton;
	for (int i = 0; i < m_pMoveButtonBitmaps.size(); ++i)
		delete m_pMoveButtonBitmaps[i];

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

	m_pLightButton->SetState(Button::STATE_DEACTIVATED);

	// MOVE BUTTON
	m_pMoveButton = new Button(20,7,36,36,true);

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

	// CAMERA
	m_pCameraBitmap = new Bitmap(_T("Content/Images/Editor/camera.png"));
}
void EditorGUI::Draw()
{
	BLOX_2D->SetAntiAliasing(false);

	// BACKGROUND
	BLOX_2D->SetColor(50,50,50);
	BLOX_2D->FillRect(0,0,static_cast<int>(BLOX_2D->GetWindowSize().width),50);

	BLOX_2D->SetColor(90,90,90);
	BLOX_2D->DrawLine(0,50,static_cast<int>(BLOX_2D->GetWindowSize().width),50);

	BLOX_2D->SetColor(40,40,40);
	BLOX_2D->FillRect(0,static_cast<int>(BLOX_2D->GetWindowSize().height)-20,static_cast<int>(BLOX_2D->GetWindowSize().width),20);

	// BUTTONS
	m_pLightButton->Show();
	if (m_pLightButton->Hover())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Turn on/off visual light debugger: let's you manipulate lights in the scene."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	m_pMoveButton->Show();
	if (m_pMoveButton->Hover())
	{
		BLOX_2D->SetColor(255,255,255,0.5f);
		BLOX_2D->SetFont(_T("Verdana"),false,false,10);
		BLOX_2D->DrawString(_T("Move objects and lights present in the scene."),20,static_cast<int>(BLOX_2D->GetWindowSize().height)-16);
	}

	// CAMERA
	if (m_bUsingCamera)
		BLOX_2D->DrawBitmap(m_pCameraBitmap,static_cast<int>(BLOX_2D->GetWindowSize().width-70),90,0.8f);

	BLOX_2D->SetAntiAliasing(true);
}
void EditorGUI::Tick()
{
	// BUTTONS
	m_pLightButton->Tick();
	m_pMoveButton->Tick();

	// CAMERA
	if (CONTROLS->RightMBDown())
		m_bUsingCamera = true;
	else
		m_bUsingCamera = false;
}