#include "LoadLevelFromFile.h"
#include "ContentManager.h"
#include "SimpleObject.h"

// CONSTRUCTOR - DESTRUCTOR
LoadLevelFromFile::LoadLevelFromFile(Level* pLevel)
	:	m_bIsLoaded(false),
		m_LoadLevelPath(_T("")),
		m_SaveLevelPath(_T("")),
		m_pLoadButton(0),
		m_pSaveButton(0),
		m_pFont(0),
		m_WorkingDirectory(_T("")),
		m_pLevel(pLevel)
{
	m_LoadPathBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_normal.png")));
	m_LoadPathBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_hover.png")));

	m_LoadLevelBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_normal.png")));
	m_LoadLevelBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_hover.png")));

	m_SavelevelBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/save_normal.png")));
	m_SavelevelBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/save_hover.png")));

	m_pLoadButton = new Button(100, 260, 36, 36);
	m_pLoadButton->SetNormalState(m_LoadLevelBitmaps[0]);
	m_pLoadButton->SetHoverState(m_LoadLevelBitmaps[1]);
	m_pLoadButton->SetDownState(m_LoadLevelBitmaps[1]);

	m_pSaveButton = new Button(150, 260, 36, 36);
	m_pSaveButton->SetNormalState(m_SavelevelBitmaps[0]);
	m_pSaveButton->SetHoverState(m_SavelevelBitmaps[1]);
	m_pSaveButton->SetDownState(m_SavelevelBitmaps[1]);

	for (int i = 0; i < 2; ++i)
	{
		Button* pButton = new Button(150,122 + (i*80),15,15);
		pButton->SetNormalState(m_LoadPathBitmaps[0]);
		pButton->SetHoverState(m_LoadPathBitmaps[1]);
		pButton->SetDownState(m_LoadPathBitmaps[1]);

		m_Buttons.push_back(pButton);
	}

	TCHAR Buffer[256];
	GetCurrentDirectory(256, Buffer);

	m_WorkingDirectory = Buffer;

	m_pFont = Content->LoadTextFormat(_T("Verdana"),12, false,false);
}


LoadLevelFromFile::~LoadLevelFromFile()
{
	delete m_pLoadButton;
    delete m_pSaveButton;

	for (vector<Button*>::iterator it = m_Buttons.begin(); it != m_Buttons.end(); ++it)
		delete (*it);

	for (vector<TextBox*>::iterator it = m_TextBoxes.begin(); it != m_TextBoxes.end(); ++it)
		delete (*it);
}

// GENERAL
void LoadLevelFromFile::Tick()
{
	if (m_TextBoxes.size() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			TextBox* pTextBox = new TextBox();
			pTextBox->SetBounds(20, 120 + (i*80),120,20);

			m_TextBoxes.push_back(pTextBox);
		}
	}

	for (vector<Button*>::iterator it = m_Buttons.begin(); it != m_Buttons.end(); ++it)
	{
		(*it)->Tick();
	}

	m_pLoadButton->Tick();

	if (m_Buttons[0]->Clicked())
	{
		m_LoadLevelPath = GetPath(_T("Load Level"), L"Level Files\0*.level\0\0");

		m_TextBoxes[0]->SetText(m_LoadLevelPath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}
	if (m_Buttons[1]->Clicked())
	{
		m_SaveLevelPath = GetPath(_T("Save Level"), L"level Files\0*.level\0\0");
		
		m_TextBoxes[1]->SetText(m_SaveLevelPath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}

	if (m_pLoadButton->Clicked())
	{
		if (m_LoadLevelPath != _T(""))
		{
			string s(m_LoadLevelPath.begin(), m_LoadLevelPath.end());

			m_pLevel->Deserialize(s);

			m_bIsLoaded = true;
		}
	}

	if (m_pSaveButton->Clicked())
	{
		if (m_SaveLevelPath != _T(""))
		{
			string s(m_SaveLevelPath.begin(), m_SaveLevelPath.end());

			m_pLevel->Serialize(s);
		}
	}

	if (m_TextBoxes[0]->Entered())
	{
		m_LoadLevelPath = m_TextBoxes[0]->GetText();

		m_TextBoxes[0]->LoseFocus();
	}

	if (m_TextBoxes[1]->Entered())
	{
		m_SaveLevelPath = m_TextBoxes[1]->GetText();
		m_TextBoxes[1]->LoseFocus();
	}
}

void LoadLevelFromFile::Show()
{
	BX2D->SetColor(255, 255, 255);
	BX2D->SetFont(m_pFont);
	BX2D->DrawString(_T("Load/save level from a file") ,10,60);

	BX2D->DrawString(_T("Load path:") ,10,100);
	BX2D->DrawString(_T("Save path:") ,10,180);

	for (vector<Button*>::iterator it = m_Buttons.begin(); it != m_Buttons.end(); ++it)
	{
		(*it)->Show();
	}

	for (vector<TextBox*>::iterator it = m_TextBoxes.begin(); it != m_TextBoxes.end(); ++it)
	{
		(*it)->Show();			
	}

	m_pLoadButton->Show();
	m_pSaveButton->Show();
}

void LoadLevelFromFile::HideTextBoxes()
{
	for (vector<TextBox*>::iterator it = m_TextBoxes.begin(); it != m_TextBoxes.end(); ++it)
	{
		(*it)->Hide();
	}
}

tstring LoadLevelFromFile::GetPath(tstring title, LPWSTR filter)
{
	wchar_t filePath[256];

	OPENFILENAME opf;
	opf.hwndOwner = 0;
	opf.lpstrFilter = filter;
	opf.lpstrCustomFilter = 0;
	opf.nMaxCustFilter = 0L;
	opf.nFilterIndex = 1L;
	opf.lpstrFile = filePath;
	opf.lpstrFile[0] = '\0';
	opf.nMaxFile = 255;
	opf.lpstrFileTitle = 0;
	opf.nMaxFileTitle=50;
	opf.lpstrInitialDir = 0;
	opf.lpstrTitle = title.c_str();
	opf.nFileOffset = 0;
	opf.nFileExtension = 2;
	opf.lpstrDefExt = L"*.*";
	opf.lpfnHook = NULL;
	opf.lCustData = 0;
	opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	opf.lStructSize = sizeof(OPENFILENAMEW);

	GetOpenFileName(&opf);

	return opf.lpstrFile;
}

void LoadLevelFromFile::Clear()
{
	m_LoadLevelPath = m_SaveLevelPath = _T("");

	for (int i = 0; i < 2; ++i)
	{
		if (m_TextBoxes.size() > 0)
			m_TextBoxes[i]->SetText(_T(""));
	}
}

// GETTERS
