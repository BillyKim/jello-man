#pragma once

#include "ILevelObject.h"
#include "Blox2D.h"
#include "Level.h"
#include "RenderContext.h"
#include "PhysX.h"

class LoadModelFromFile
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	LoadModelFromFile(Level* pLevel, PhysX* pPhysXEngine);
	virtual ~LoadModelFromFile();

	// GENERAL
	void Tick(const RenderContext* pRenderContext);
	void Show();
	void Clear();

	void HideTextBoxes();

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

	Level* m_pLevel;

	const RenderContext* m_pRenderContext;

	PhysX* m_pPhysXEngine;
};

