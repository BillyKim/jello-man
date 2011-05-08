#include "LoadModelFromFile.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
LoadModelFromFile::LoadModelFromFile()	:	m_bIsLoaded(false),
											m_bLevelObjectExtracted(false),
											m_pLevelObject(0),
											m_pbtnUseNormalMap(0),
											m_pbtnLoadModel(0),
											m_ModelPath(_T("")),
											m_PhysXModelPath(_T("")),
											m_NormalPath(_T("")),
											m_DiffusePath(_T("")),
											m_SpecPath(_T("")),
											m_GlossPath(_T("")),
											m_WorkingDirectory(_T(""))
{
	m_LoadPathBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_normal.png")));
	m_LoadPathBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_hover.png")));

	m_LoadModelBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_normal.png")));
	m_LoadModelBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/load_hover.png")));

	m_pbtnLoadModel = new Button(150, 600, 36, 36);
	m_pbtnLoadModel->SetNormalState(m_LoadModelBitmaps[0]);
	m_pbtnLoadModel->SetHoverState(m_LoadModelBitmaps[1]);
	m_pbtnLoadModel->SetDownState(m_LoadModelBitmaps[1]);

	for (int i = 0; i < 6; ++i)
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


LoadModelFromFile::~LoadModelFromFile()
{
	delete m_pbtnUseNormalMap;

	for (vector<Button*>::iterator it = m_Buttons.begin(); it != m_Buttons.end(); ++it)
		delete (*it);

	for (vector<TextBox*>::iterator it = m_TextBoxes.begin(); it != m_TextBoxes.end(); ++it)
		delete (*it);
}

// GENERAL
void LoadModelFromFile::Tick()
{
	if (m_TextBoxes.size() == 0)
	{
		for (int i = 0; i < 6; ++i)
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

	m_pbtnLoadModel->Tick();

	if (m_Buttons[0]->Clicked())
	{
		m_ModelPath = GetPath(_T("Load Model"), L"OBJ Files\0*.binobj;*.obj*\0\0");

		m_TextBoxes[0]->SetText(m_ModelPath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}
	if (m_Buttons[1]->Clicked())
	{
		m_PhysXModelPath = GetPath(_T("Load PhysX Model"), L"NX Files\0*.nxconvex;*.nxconcave*\0\0");
		
		m_TextBoxes[1]->SetText(m_PhysXModelPath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}
	if (m_Buttons[2]->Clicked())
	{
		m_NormalPath = GetPath(_T("Load Normal Map"), L"Image Files\0*.png\0\0");

		m_TextBoxes[2]->SetText(m_NormalPath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}
	if (m_Buttons[3]->Clicked())
	{
		m_DiffusePath = GetPath(_T("Load Diffuse map"), L"Image Files\0*.png\0\0");

		m_TextBoxes[3]->SetText(m_DiffusePath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}
	if (m_Buttons[4]->Clicked())
	{
		m_SpecPath = GetPath(_T("Load Specular Map"), L"Image Files\0*.png\0\0");

		m_TextBoxes[4]->SetText(m_SpecPath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}
	if (m_Buttons[5]->Clicked())
	{
		m_GlossPath = GetPath(_T("Load Glossiness Map"), L"Image Files\0*.png\0\0");

		m_TextBoxes[5]->SetText(m_GlossPath);

		SetCurrentDirectory(m_WorkingDirectory.c_str());
	}

	if (m_pbtnLoadModel->Clicked())
	{
		if (m_ModelPath != _T("") && m_PhysXModelPath != _T(""))
		{
			m_pLevelObject = new LevelObject();

			if (m_NormalPath != _T(""))
			{
				m_pLevelObject->UseNormalMap(true);
				m_pLevelObject->SetNormalPath(m_NormalPath);
			}
			else
				m_pLevelObject->UseNormalMap(false);

			if (m_DiffusePath == _T(""))
				m_DiffusePath = _T("../Content/Textures/checker_1024.png");

			if (m_SpecPath == _T(""))
				m_SpecPath = _T("../Content/Textures/checker_1024.png");

			if (m_GlossPath == _T(""))
				m_GlossPath = _T("../Content/Textures/checker_1024.png");

			m_pLevelObject->UseSimplifiedPhysXMesh(false);

			m_pLevelObject->SetModelPath(m_ModelPath);
			m_pLevelObject->SetPhysXModelPath(m_PhysXModelPath);
			
			m_pLevelObject->SetDiffusePath(m_DiffusePath);
			m_pLevelObject->SetSpecPath(m_SpecPath);
			m_pLevelObject->SetGlossPath(m_GlossPath);

			m_pLevelObject->SetRigid(true);
			m_pLevelObject->SetMass(1000);

			m_bIsLoaded = true;
		}
	}

	if (m_TextBoxes[0]->Entered())
	{
		m_ModelPath = m_TextBoxes[0]->GetText();

		m_TextBoxes[0]->LoseFocus();
	}

	if (m_TextBoxes[1]->Entered())
	{
		m_PhysXModelPath = m_TextBoxes[1]->GetText();
		m_TextBoxes[1]->LoseFocus();
	}

	if (m_TextBoxes[2]->Entered())
	{
		m_NormalPath = m_TextBoxes[2]->GetText();
		m_TextBoxes[2]->LoseFocus();
	}

	if (m_TextBoxes[3]->Entered())
	{
		m_DiffusePath = m_TextBoxes[3]->GetText();
		m_TextBoxes[3]->LoseFocus();
	}

	if (m_TextBoxes[4]->Entered())
	{
		m_SpecPath = m_TextBoxes[4]->GetText();
		m_TextBoxes[4]->LoseFocus();
	}

	if (m_TextBoxes[5]->Entered())
	{
		m_GlossPath = m_TextBoxes[5]->GetText();
		m_TextBoxes[5]->LoseFocus();
	}
}

void LoadModelFromFile::Show()
{
	BX2D->SetColor(255, 255, 255);
	BX2D->SetFont(m_pFont);
	BX2D->DrawString(_T("Load new model from file") ,10,60);

	BX2D->DrawString(_T("Model path:") ,10,100);
	BX2D->DrawString(_T("PhysX Model path:") ,10,180);
	BX2D->DrawString(_T("Normal Map path:") ,10,260);
	BX2D->DrawString(_T("Diffuse Map path:") ,10,340);
	BX2D->DrawString(_T("Specular Map path:") ,10,420);
	BX2D->DrawString(_T("Glossiness Map path:") ,10,500);

	for (vector<Button*>::iterator it = m_Buttons.begin(); it != m_Buttons.end(); ++it)
	{
		(*it)->Show();
	}

	for (vector<TextBox*>::iterator it = m_TextBoxes.begin(); it != m_TextBoxes.end(); ++it)
	{
		(*it)->Show();			
	}

	m_pbtnLoadModel->Show();
}

void LoadModelFromFile::HideTextBoxes()
{
	for (vector<TextBox*>::iterator it = m_TextBoxes.begin(); it != m_TextBoxes.end(); ++it)
	{
		(*it)->Hide();
	}
}

tstring LoadModelFromFile::GetPath(tstring title, LPWSTR filter)
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

void LoadModelFromFile::Clear()
{
	m_GlossPath = m_SpecPath = m_DiffusePath = m_NormalPath = m_PhysXModelPath = m_ModelPath = _T("");

	m_pLevelObject = 0;

	m_bIsLoaded = false;

	m_bLevelObjectExtracted = false;

	for (int i = 0; i < 6; ++i)
	{
		if (m_TextBoxes.size() > 0)
			m_TextBoxes[i]->SetText(_T(""));
	}
}

// GETTERS
