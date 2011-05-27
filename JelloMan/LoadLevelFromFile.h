#pragma once

#include "Level.h"
#include "Blox2D.h"

class LoadLevelFromFile
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	LoadLevelFromFile(Level* pLevel);
	virtual ~LoadLevelFromFile();

	// GENERAL
	void Tick();
	void Show();
	void Clear();

	void HideTextBoxes();

private:

	tstring GetPath(tstring title, LPWSTR filter, bool bSave = false);

	// DATAMEMBERS
	tstring m_LoadLevelPath;
	tstring m_SaveLevelPath;

	vector<TextBox*> m_TextBoxes;
	vector<Button*> m_Buttons;

	Button* m_pLoadButton;
	Button* m_pSaveButton;

	vector<Image*> m_LoadPathBitmaps;
	vector<Image*> m_LoadLevelBitmaps;
	vector<Image*> m_SavelevelBitmaps;

	tstring m_WorkingDirectory;

	TextFormat* m_pFont;

	Level* m_pLevel;
};

