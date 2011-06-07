#include "Infobar.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR */
Infobar::Infobar() :	m_bShowMessage(false),
						m_pInfoFont(0),
						m_Message(_T(""))
{
	m_pInfoFont = Content->LoadTextFormat(_T("Verdana"),10,false,false);

	for (int i = 0; i < 4; ++i)
	{
		m_SceneInfo.push_back(0);
	}
}

Infobar::~Infobar()
{
}

/* GENERAL */
void Infobar::Draw()
{
	// background
	BX2D->SetColor(70, 70, 70);
	BX2D->FillRect(	0,
					BX2D->GetWindowSize().height - 20,
					BX2D->GetWindowSize().width,
					20	);

	BX2D->SetColor(90, 90, 90);
	BX2D->DrawLine(	0,
					BX2D->GetWindowSize().height - 20,
					BX2D->GetWindowSize().width,
					BX2D->GetWindowSize().height - 20	);

	// message
	tstring messageToDisplay(_T(""));

	if (m_bShowMessage == false)
	{
		for_each(m_ButtonMessages.begin(), m_ButtonMessages.end(), [&](pair<Button*, tstring> p)
		{
			if (p.first->Hover() || p.first->Down())
			{
				messageToDisplay = p.second;
			}
		});
	}
	else if (m_Message != _T(""))
	{
		messageToDisplay = m_Message;
	}

	if (messageToDisplay != _T(""))
	{
		BX2D->SetColor(255,255,255,0.8f);
		BX2D->SetFont(m_pInfoFont);

		m_pInfoFont->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);

		BX2D->DrawString(messageToDisplay, 20, BX2D->GetWindowSize().height - 16);
	}

	m_bShowMessage = false;

	// sceneinfo
	tstringstream streamInfo;

	streamInfo << _T("lights: ") << m_SceneInfo[1] << _T(" / ") << m_SceneInfo[0];
	streamInfo << _T("     ");
	streamInfo << _T("objects: ") << m_SceneInfo[3] << _T(" / ") << m_SceneInfo[2];

	BX2D->SetColor(255, 255, 255, 0.8f);
	BX2D->SetFont(m_pInfoFont);

	m_pInfoFont->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
	m_pInfoFont->SetVerticalAlignment(PARAGRAPH_ALIGNMENT_BOTTOM);

	BX2D->DrawString(streamInfo.str(), RectF(0, 0, BX2D->GetWindowSize().width - 5, BX2D->GetWindowSize().height - 4));
}

void Infobar::ShowMessage(const tstring& message)
{
	if (message != _T(""))
	{
		m_Message = message;

		m_bShowMessage = true;
	}
}

void Infobar::AddButtonHoverMessage(Button* pButton, const tstring& message)
{
	if (m_ButtonMessages.find(pButton) == m_ButtonMessages.end())
	{
		m_ButtonMessages[pButton] = message;
	}
	else
		ASSERT(false, "Button already has a hover message to display");
}

/* SETTERS */
void Infobar::SetSceneInfo(int nrLights, int nrSelectedLights, int nrObjects, int nrSelectedObjects)
{
	m_SceneInfo[0] = nrLights;
	m_SceneInfo[1] = nrSelectedLights;

	m_SceneInfo[2] = nrObjects;
	m_SceneInfo[3] = nrSelectedObjects;
}
