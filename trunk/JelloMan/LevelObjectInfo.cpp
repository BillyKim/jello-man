#include "LevelObjectInfo.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR*/
LevelObjectInfo::LevelObjectInfo(Level* pLevel) :	m_pLevel(pLevel),
													m_pLevelObject(0),
													m_pTextBoxX(0),
													m_pTextBoxY(0),
													m_pTextBoxZ(0),
													m_pObjectInfoFont(0),
													m_pObjectInfoFont2(0),
													m_pPreviousLevelObject(0),
													m_PreviousPos(Vector3(0,0,0)),
													m_bTextBoxesSet(false)
{
	// FONT
	m_pObjectInfoFont = Content->LoadTextFormat(_T("Verdana"),10, false,false);
	m_pObjectInfoFont2 = Content->LoadTextFormat(_T("Verdana"),12, false,false);
}

LevelObjectInfo::~LevelObjectInfo()
{
	m_pRenderContext = 0;
	m_pLevel = 0;
	m_pObjectInfoFont = 0;
	m_pObjectInfoFont2 = 0;
	m_pLevelObject = 0;
	m_pPreviousLevelObject = 0; 

	delete m_pTextBoxX;
	delete m_pTextBoxY;
	delete m_pTextBoxZ;
}

/* GENERAL */
void LevelObjectInfo::Tick()
{
	if (!m_pTextBoxX)
	{
		m_pTextBoxX = new TextBox();
		m_pTextBoxX->SetBounds(30,85,80,20);
	}

	if (!m_pTextBoxY)
	{
		m_pTextBoxY = new TextBox();
		m_pTextBoxY->SetBounds(30,115,80,20);
	}

	if (!m_pTextBoxZ)
	{
		m_pTextBoxZ = new TextBox();
		m_pTextBoxZ->SetBounds(30,145,80,20);
	}

	if (GetNrObjectsSelected() == 1)
	{
		for_each(m_pLevel->GetLevelObjects().cbegin(), m_pLevel->GetLevelObjects().cend(), [&](ILevelObject* pObj)
		{
			if (pObj->IsSelected())
				m_pLevelObject = pObj;
		});

		if (m_pTextBoxX->Entered())
		{
			float f;

			tstring s1 = m_pTextBoxX->GetText();

			string s2(s1.begin(), s1.end());

			const char* s = s2.c_str();

			f = static_cast<float>(atof(s));

			if (!(f == 0 && s2 != "0"))
			{
				Vector3 pos = m_pLevelObject->GetPosition();
				pos.X = f;

				m_pLevelObject->SetPosition(pos);
			}

			m_pTextBoxX->LoseFocus();
		}

		if (m_pTextBoxY->Entered())
		{
			float f;

			tstring s1 = m_pTextBoxY->GetText();

			string s2(s1.begin(), s1.end());

			const char* s = s2.c_str();

			f = static_cast<float>(atof(s));

			if (!(f == 0 && s2 != "0"))
			{
				Vector3 pos = m_pLevelObject->GetPosition();
				pos.Y = f;

				m_pLevelObject->SetPosition(pos);
			}

			m_pTextBoxY->LoseFocus();
		}

		if (m_pTextBoxZ->Entered())
		{
			float f;

			tstring s1 = m_pTextBoxZ->GetText();

			string s2(s1.begin(), s1.end());

			const char* s = s2.c_str();

			f = static_cast<float>(atof(s));

			if (!(f == 0 && s2 != "0"))
			{
				Vector3 pos = m_pLevelObject->GetPosition();
				pos.Z = f;

				m_pLevelObject->SetPosition(pos);
			}

			m_pTextBoxZ->LoseFocus();
		}

		if (m_PreviousPos != m_pLevelObject->GetPosition())
			m_bTextBoxesSet = false;

		m_PreviousPos = m_pLevelObject->GetPosition();
		m_pPreviousLevelObject = m_pLevelObject;
	}
}

void LevelObjectInfo::Draw(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	if (GetNrObjectsSelected() == 1)
	{
		ShowModelInfo();
	}
	else if (GetNrObjectsSelected() > 1)
	{
		tstringstream stream;
		stream.precision(8);

		stream << _T("Selected:\n\n");

		for (unsigned int i = 0; i < m_pLevel->GetLevelObjects().size(); ++i)
		{
			if (m_pLevel->GetLevelObjects()[i]->IsSelected())
			{
				stream << _T("Object ") << i << _T(",\n");
			}
		}

		BX2D->SetColor(255, 255, 255);
		BX2D->SetFont(m_pObjectInfoFont2);
		BX2D->DrawString(stream.str(),10,60);

		HideTextBoxes();
	}
	else
		HideTextBoxes();
}

void LevelObjectInfo::ShowModelInfo()
{
	tstringstream stream;
	stream.precision(8);

	stream << _T("LevelObject selected:\n\n");

	// X
	tstringstream strm;
	strm << m_pLevelObject->GetPosition().X;

	tstring t = strm.str();
	stream << _T("X: ")  << _T("\n\n");

	if (!m_bTextBoxesSet || m_pPreviousLevelObject != m_pLevelObject)
	{
		tstringstream strmX;
		strmX << t.substr(0, t.find(_T(".")) + 4);
		m_pTextBoxX->SetText(strmX.str());
	}

	strm.str(_T(""));
	strm << m_pLevelObject->GetPosition().Y;
	t = strm.str();
	stream << _T("Y: ") << _T("\n\n");

	if (!m_bTextBoxesSet || m_pPreviousLevelObject != m_pLevelObject)
	{
		tstringstream strmY;
		strmY << t.substr(0, t.find(_T(".")) + 4);
		m_pTextBoxY->SetText(strmY.str());
	}

	strm.str(_T(""));
	strm << m_pLevelObject->GetPosition().Z;
	t = strm.str();
	stream << _T("Z: ") << _T("\n\n\n");

	if (!m_bTextBoxesSet || m_pPreviousLevelObject!= m_pLevelObject)
	{
		tstringstream strmZ;
		strmZ << t.substr(0, t.find(_T(".")) + 4);
		m_pTextBoxZ->SetText(strmZ.str());
	}

	m_bTextBoxesSet = true;

	m_pTextBoxX->Show();
	m_pTextBoxY->Show();
	m_pTextBoxZ->Show();

	BX2D->SetColor(40,40,40);
	BX2D->DrawLine(10,180,190,180);
	/*BX2D->DrawLine(10,270,190,270);
	BX2D->DrawLine(10,330,190,330);
	BX2D->DrawLine(10,387,190,387);*/

	BX2D->SetColor(255, 255, 255);
	BX2D->SetFont(m_pObjectInfoFont2);
	BX2D->DrawString(stream.str(),10,60);
}

void LevelObjectInfo::HideTextBoxes()
{
	m_pTextBoxX->Hide();
	m_pTextBoxY->Hide();
	m_pTextBoxZ->Hide();

	m_bTextBoxesSet = false;
}

/* GETTERS */
int LevelObjectInfo::GetNrObjectsSelected()
{
	int b = 0;

	for_each(m_pLevel->GetLevelObjects().cbegin(), m_pLevel->GetLevelObjects().cend(), [&](ILevelObject* pObj)
	{
		if (pObj->IsSelected())
			++b;
	});

	return b;
}
