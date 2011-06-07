#pragma once

#include "ILevelObject.h"
#include "Blox2D.h"
#include <vector>

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
	bool Load() const
	{ return m_pbtnLoadModel->Clicked(); }

	vector<tstring> GetPaths() const;

private:

	tstring GetPath(const tstring& title, LPWSTR filter);
	tstring StripPath(const tstring& path);

	// DATAMEMBERS
	tstring m_ModelPath;
	tstring m_PhysXModelPath;
	tstring m_NormalPath;
	tstring m_DiffusePath;
	tstring m_SpecPath;
	tstring m_GlossPath;

	vector<TextBox*> m_TextBoxes;
	vector<Button*> m_Buttons;

	Button* m_pbtnUseNormalMap;
	Button* m_pbtnLoadModel;

	vector<Image*> m_LoadPathBitmaps;
	vector<Image*> m_LoadModelBitmaps;
	vector<Image*> m_UseNormalMapBitmaps;

	tstring m_WorkingDirectory;

	TextFormat* m_pFont;

	const RenderContext* m_pRenderContext;
};

