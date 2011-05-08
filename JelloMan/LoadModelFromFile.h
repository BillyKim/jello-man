#pragma once

#include "LevelObject.h"
#include "Blox2D.h"

class LoadModelFromFile
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	LoadModelFromFile();
	virtual ~LoadModelFromFile();

	// GENERAL
	void Tick();
	void Show();
	void Clear();

	void HideTextBoxes();

	// GETTERS
	LevelObject* GetLevelObject()
	{ 
	  m_bLevelObjectExtracted = true;
	  return m_pLevelObject;
	}

	bool IsLoaded() const
	{ return m_bIsLoaded; }

	bool LevelObjectExtracted() const
	{ return m_bLevelObjectExtracted; }

private:

	tstring GetPath(tstring title, LPWSTR filter);

	// DATAMEMBERS
	bool m_bIsLoaded;
	bool m_bLevelObjectExtracted;

	tstring m_ModelPath;
	tstring m_PhysXModelPath;
	tstring m_NormalPath;
	tstring m_DiffusePath;
	tstring m_SpecPath;
	tstring m_GlossPath;

	LevelObject* m_pLevelObject;

	vector<TextBox*> m_TextBoxes;
	vector<Button*> m_Buttons;

	Button* m_pbtnUseNormalMap;
	Button* m_pbtnLoadModel;

	vector<Image*> m_LoadPathBitmaps;
	vector<Image*> m_LoadModelBitmaps;
	vector<Image*> m_UseNormalMapBitmaps;

	tstring m_WorkingDirectory;

	TextFormat* m_pFont;
};

