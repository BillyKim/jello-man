#pragma once

#include "Level.h"
#include "Blox2D.h"

class LoadLevelFromFile
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	LoadLevelFromFile();
	virtual ~LoadLevelFromFile();

	// GENERAL
	void Tick();
	void Show();
	void Clear();

	void HideTextBoxes();

	// GETTERS
	bool Load() const
	{ return m_pLoadButton->Clicked(); }
	bool Save() const
	{ return m_pSaveButton->Clicked(); }

	const tstring& GetLoadPath() const
	{ return m_LoadLevelPath; }
	const tstring& GetSavePath() const
	{ return m_SaveLevelPath; }

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

	
	/* Disable default assigment operator en copy constructor */
	LoadLevelFromFile(const LoadLevelFromFile&);
	LoadLevelFromFile& operator=(const LoadLevelFromFile&);
};

