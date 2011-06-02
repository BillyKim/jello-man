#include "Snapper.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR */
Snapper::Snapper(MoveGizmo* pMoveGizmo, RotateGizmo* pRotateGizmo)	:
	m_pMoveGizmo(pMoveGizmo),
	m_pRotateGizmo(pRotateGizmo),
	m_LinearSnapSize(0.1f),
	m_AngularSnapSize(1.0f),
	m_bSnapToGrid(false),
	m_pLinearSnapTextBox(0),
	m_pAngularSnapTextBox(0),
	m_pUseLinearSnappingButton(0),
	m_pUseAngularSnappingButton(0),
	m_pGridSnappingButton(0),
	m_pFont(0),
	m_bTextBoxesSet(false)
{
	m_pFont = Content->LoadTextFormat(_T("Verdana"), 12, false, false);

	m_UseSnappingButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/checkbox_empty_normal.png")));
	m_UseSnappingButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/checkbox_full_normal.png")));

	m_pUseLinearSnappingButton = new Button(160, 101, 15, 15, true);
	m_pUseLinearSnappingButton->SetNormalState(m_UseSnappingButtonImages[1]);
	m_pUseLinearSnappingButton->SetHoverState(m_UseSnappingButtonImages[1]);
	m_pUseLinearSnappingButton->SetDownState(m_UseSnappingButtonImages[1]);
	m_pUseLinearSnappingButton->SetDeactivatedState(m_UseSnappingButtonImages[0]);
	m_pUseLinearSnappingButton->SetDeactivatedStateHover(m_UseSnappingButtonImages[0]);
	m_pUseLinearSnappingButton->SetDeactivatedStateDown(m_UseSnappingButtonImages[0]);
	m_pUseLinearSnappingButton->SetState(Button::STATE_DEACTIVATED);

	m_pUseAngularSnappingButton = new Button(160, 161, 15, 15, true);
	m_pUseAngularSnappingButton->SetNormalState(m_UseSnappingButtonImages[1]);
	m_pUseAngularSnappingButton->SetHoverState(m_UseSnappingButtonImages[1]);
	m_pUseAngularSnappingButton->SetDownState(m_UseSnappingButtonImages[1]);
	m_pUseAngularSnappingButton->SetDeactivatedState(m_UseSnappingButtonImages[0]);
	m_pUseAngularSnappingButton->SetDeactivatedStateHover(m_UseSnappingButtonImages[0]);
	m_pUseAngularSnappingButton->SetDeactivatedStateDown(m_UseSnappingButtonImages[0]);
	m_pUseAngularSnappingButton->SetState(Button::STATE_DEACTIVATED);

	m_pGridSnappingButton = new Button(160, 221, 15, 15, true);
	m_pGridSnappingButton->SetNormalState(m_UseSnappingButtonImages[1]);
	m_pGridSnappingButton->SetHoverState(m_UseSnappingButtonImages[1]);
	m_pGridSnappingButton->SetDownState(m_UseSnappingButtonImages[1]);
	m_pGridSnappingButton->SetDeactivatedState(m_UseSnappingButtonImages[0]);
	m_pGridSnappingButton->SetDeactivatedStateHover(m_UseSnappingButtonImages[0]);
	m_pGridSnappingButton->SetDeactivatedStateDown(m_UseSnappingButtonImages[0]);
	m_pGridSnappingButton->SetState(Button::STATE_DEACTIVATED);
}

Snapper::~Snapper()
{
	m_pMoveGizmo = 0;
	m_pRotateGizmo = 0;
	
	delete m_pLinearSnapTextBox;
	delete m_pAngularSnapTextBox;

	delete m_pUseLinearSnappingButton;
	delete m_pUseAngularSnappingButton;
	delete m_pGridSnappingButton;
}

/* GENERAL */
void Snapper::Tick()
{
	if (!m_pLinearSnapTextBox)
	{
		m_pLinearSnapTextBox = new TextBox();
		m_pLinearSnapTextBox->SetBounds(20, 120, 120, 20);
		m_pLinearSnapTextBox->SetEnabled(false);
	}

	if (!m_pAngularSnapTextBox)
	{
		m_pAngularSnapTextBox = new TextBox();
		m_pAngularSnapTextBox->SetBounds(20, 180, 120, 20);
		m_pAngularSnapTextBox->SetEnabled(false);
	}

	if (!m_bTextBoxesSet)
	{
		tstringstream stream;
		stream << m_LinearSnapSize;

		m_pLinearSnapTextBox->SetText(stream.str());

		stream.str(_T(""));

		stream << m_AngularSnapSize;

		m_pAngularSnapTextBox->SetText(stream.str());

		m_bTextBoxesSet = true;
	}

	m_pUseLinearSnappingButton->Tick();
	m_pUseAngularSnappingButton->Tick();
	m_pGridSnappingButton->Tick();

	if (m_pUseLinearSnappingButton->Clicked())
	{
		if (m_pUseLinearSnappingButton->IsActive())
		{
			m_pMoveGizmo->UseSnapping(true);
			m_pLinearSnapTextBox->SetEnabled(true);
		}
		else
		{
			m_pMoveGizmo->UseSnapping(false);
			m_pLinearSnapTextBox->SetEnabled(false);
		}
	}

	if (m_pUseAngularSnappingButton->Clicked())
	{
		if (m_pUseAngularSnappingButton->IsActive())
		{
			m_pRotateGizmo->UseSnapping(true);
			m_pAngularSnapTextBox->SetEnabled(true);
		}
		else
		{
			m_pRotateGizmo->UseSnapping(false);
			m_pAngularSnapTextBox->SetEnabled(false);
		}
	}

	if (m_pGridSnappingButton->Clicked())
	{
		if (m_pGridSnappingButton->IsActive())
			m_pMoveGizmo->SnapToGrid(true);
		else
			m_pMoveGizmo->SnapToGrid(false);
	}

	if (m_pLinearSnapTextBox->Entered())
	{
		float f;

		tstring s1 = m_pLinearSnapTextBox->GetText();

		string s2(s1.begin(), s1.end());

		const char* s = s2.c_str();

		f = static_cast<float>(atof(s));

		if (!(f == 0 && s2 != "0") && f > 0.001)
		{
			m_LinearSnapSize = f;
			m_pMoveGizmo->SetSnapSize(f);
		}
		else
		{
			m_LinearSnapSize = 0.002;
			m_pMoveGizmo->SetSnapSize(0.002);
			m_bTextBoxesSet = false;
		}

		m_pLinearSnapTextBox->LoseFocus();
	}

	if (m_pAngularSnapTextBox->Entered())
	{
		float f;

		tstring s1 = m_pAngularSnapTextBox->GetText();

		string s2(s1.begin(), s1.end());

		const char* s = s2.c_str();

		f = static_cast<float>(atof(s));

		if (!(f == 0 && s2 != "0") && f > 0.01)
		{
			m_AngularSnapSize = f;
			m_pMoveGizmo->SetSnapSize(f);
		}
		else
			m_bTextBoxesSet = false;

		m_pAngularSnapTextBox->LoseFocus();
	}
}

void Snapper::Draw()
{
	BX2D->SetColor(255, 255, 255);
	BX2D->SetFont(m_pFont);
	BX2D->DrawString(_T("Snapping options") , 10, 60);

	BX2D->DrawString(_T("Use linear snapping"), 10, 100);
	BX2D->DrawString(_T("Use angular snapping"), 10, 160);
	BX2D->DrawString(_T("Snap to grid (linear)"), 10, 220);

	m_pLinearSnapTextBox->Show();
	m_pAngularSnapTextBox->Show();

	m_pUseLinearSnappingButton->Show();
	m_pUseAngularSnappingButton->Show();
	m_pGridSnappingButton->Show();
}

void Snapper::HideTextBoxes()
{
	if (m_pLinearSnapTextBox)
		m_pLinearSnapTextBox->Hide();

	if (m_pAngularSnapTextBox)
		m_pAngularSnapTextBox->Hide();
}