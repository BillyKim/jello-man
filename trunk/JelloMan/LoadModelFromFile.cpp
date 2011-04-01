#include "LoadModelFromFile.h"

// CONSTRUCTOR - DESTRUCTOR
LoadModelFromFile::LoadModelFromFile()	:	m_bIsLoaded(false),
											m_bLevelObjectExtracted(false)
{
}


LoadModelFromFile::~LoadModelFromFile()
{
}

// GENERAL
void LoadModelFromFile::LoadNewModel()
{
	tstring modelPath = _T("");
	tstring physXModelPath = _T("");
	tstring diffusePath = _T("");
	tstring specPath = _T("");
	tstring glossPath = _T("");

	if (modelPath == _T(""))
	{
		wchar_t filePath[256];

		OPENFILENAME opf;
		opf.hwndOwner = 0;
		opf.lpstrFilter = L"*.binobj|*.binobj";
		opf.lpstrCustomFilter = 0;
		opf.nMaxCustFilter = 0L;
		opf.nFilterIndex = 1L;
		opf.lpstrFile = filePath;
		opf.lpstrFile[0] = '\0';
		opf.nMaxFile = 255;
		opf.lpstrFileTitle = 0;
		opf.nMaxFileTitle=50;
		opf.lpstrInitialDir = 0;
		opf.lpstrTitle = L"Open Model";
		opf.nFileOffset = 0;
		opf.nFileExtension = 2;
		opf.lpstrDefExt = L"*.*";
		opf.lpfnHook = NULL;
		opf.lCustData = 0;
		opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		opf.lStructSize = sizeof(OPENFILENAMEW);

		GetOpenFileName(&opf);

		modelPath = opf.lpstrFile;
	}

	if (physXModelPath == _T(""))
	{
		wchar_t filePath[256];

		OPENFILENAME opf;
		opf.hwndOwner = 0;
		opf.lpstrFilter = L"*.nxconcave|*.nxconcave";
		opf.lpstrCustomFilter = 0;
		opf.nMaxCustFilter = 0L;
		opf.nFilterIndex = 1L;
		opf.lpstrFile = filePath;
		opf.lpstrFile[0] = '\0';
		opf.nMaxFile = 255;
		opf.lpstrFileTitle = 0;
		opf.nMaxFileTitle=50;
		opf.lpstrInitialDir = 0;
		opf.lpstrTitle = L"Open PhysX Model";
		opf.nFileOffset = 0;
		opf.nFileExtension = 2;
		opf.lpstrDefExt = L"*.*";
		opf.lpfnHook = NULL;
		opf.lCustData = 0;
		opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		opf.lStructSize = sizeof(OPENFILENAMEW);

		GetOpenFileNameW(&opf);

		physXModelPath = opf.lpstrFile;
	}

	if (diffusePath == _T(""))
	{
		wchar_t filePath[256];

		OPENFILENAME opf;
		opf.hwndOwner = 0;
		opf.lpstrFilter = L"*.png|*.png";
		opf.lpstrCustomFilter = 0;
		opf.nMaxCustFilter = 0L;
		opf.nFilterIndex = 1L;
		opf.lpstrFile = filePath;
		opf.lpstrFile[0] = '\0';
		opf.nMaxFile = 255;
		opf.lpstrFileTitle = 0;
		opf.nMaxFileTitle=50;
		opf.lpstrInitialDir = 0;
		opf.lpstrTitle = L"Open Diffuse Map";
		opf.nFileOffset = 0;
		opf.nFileExtension = 2;
		opf.lpstrDefExt = L"*.*";
		opf.lpfnHook = NULL;
		opf.lCustData = 0;
		opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		opf.lStructSize = sizeof(OPENFILENAMEW);

		GetOpenFileNameW(&opf);

		diffusePath = opf.lpstrFile;
	}

	if (specPath == _T(""))
	{
		wchar_t filePath[256];

		OPENFILENAME opf;
		opf.hwndOwner = 0;
		opf.lpstrFilter = L"*.png|*.png";
		opf.lpstrCustomFilter = 0;
		opf.nMaxCustFilter = 0L;
		opf.nFilterIndex = 1L;
		opf.lpstrFile = filePath;
		opf.lpstrFile[0] = '\0';
		opf.nMaxFile = 255;
		opf.lpstrFileTitle = 0;
		opf.nMaxFileTitle=50;
		opf.lpstrInitialDir = 0;
		opf.lpstrTitle = L"Open Specular Map";
		opf.nFileOffset = 0;
		opf.nFileExtension = 2;
		opf.lpstrDefExt = L"*.*";
		opf.lpfnHook = NULL;
		opf.lCustData = 0;
		opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		opf.lStructSize = sizeof(OPENFILENAMEW);

		GetOpenFileNameW(&opf);

		specPath = opf.lpstrFile;
	}

	if (glossPath == _T(""))
	{
		wchar_t filePath[256];

		OPENFILENAME opf;
		opf.hwndOwner = 0;
		opf.lpstrFilter = L"*.png|*.png";
		opf.lpstrCustomFilter = 0;
		opf.nMaxCustFilter = 0L;
		opf.nFilterIndex = 1L;
		opf.lpstrFile = filePath;
		opf.lpstrFile[0] = '\0';
		opf.nMaxFile = 255;
		opf.lpstrFileTitle = 0;
		opf.nMaxFileTitle=50;
		opf.lpstrInitialDir = 0;
		opf.lpstrTitle = L"Open Glossiness Map";
		opf.nFileExtension = 2;
		opf.lpstrDefExt = L"*.*";
		opf.lpfnHook = NULL;
		opf.lCustData = 0;
		opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		opf.lStructSize = sizeof(OPENFILENAMEW);

		GetOpenFileNameW(&opf);

		glossPath = opf.lpstrFile;
	}

	m_pLevelObject = new LevelObject();

	m_pLevelObject->SetModelPath(modelPath);
	m_pLevelObject->SetPhysXModelPath(physXModelPath);
	m_pLevelObject->SetDiffusePath(diffusePath);
	m_pLevelObject->SetSpecPath(specPath);
	m_pLevelObject->SetGlossPath(glossPath);

	m_bIsLoaded = true;
}

// GETTERS
//
//BOOL DoOpenDialog(HWND hwnd, wchar_t *szDataPath)
//{
//   OPENFILENAME ofn;
//   ZeroMemory(&ofn, sizeof(ofn));
//   ZeroMemory(szDataPath, MAX_PATH);
//   ofn.lStructSize = sizeof(ofn);
//   ofn.hwndOwner = hwnd;
//   ofn.lpstrFilter = "pCrypt Files (*.pc)\0*.pc\0All Files (*.*)\0*.*\0\0";
//   ofn.lpstrFile = szDataPath;
//   ofn.nMaxFile = MAX_PATH;
//   ofn.Flags = OFN_HIDEREADONLY;
//   ofn.lpstrTitle = "Open Encryption";
//   return GetOpenFileName(&ofn);
//}