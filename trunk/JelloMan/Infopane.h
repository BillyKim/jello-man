#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"

class LoadModelFromFile;
class LoadLevelFromFile;
class Infobar;
class ColorPicker;
class Level;
class Toolbar;
class LightInfo;
class LevelObjectInfo;
class RenderContext;
class EditorLoader;
class Snapper;

class Infopane
{
public:

	enum INFO_STATE
	{
		INFO_STATE_NONE,
		INFO_STATE_LIGHTINFO,
		INFO_STATE_LEVELOBJECTINFO,
		INFO_STATE_LOADMODEL,
		INFO_STATE_LOADLEVEL,
		INFO_STATE_SNAPPINGOPTIONS
	};

	/* CONSTRUCTOR - DESTRUCTOR */
	Infopane(Infobar* pInfobar, Toolbar* pToolbar, Level* pLevel, EditorLoader* pLoader, Snapper* pSnapper);
	virtual ~Infopane();

	/* GENERAL */
	void LoadButtons();
	void Tick(const RenderContext* pRenderContext);
	void Draw();
	void Clear();

	/* SETTERS */
	void SetState(INFO_STATE state)
	{ m_State = state;}

	/* GETTERS */
	bool IsColorPickerActive()
	{ return m_Buttons["COLORPICKER"]->IsActive(); }

private:

	/* DATAMEMBERS */
	LoadModelFromFile* m_pLoadModelFromFile;
	LoadLevelFromFile* m_pLoadLevelFromFile;

	map<string, Button*> m_Buttons;
	map<string, vector<Image*> > m_ButtonImages;

	Infobar* m_pInfobar;
	Toolbar* m_pToolbar;

	ColorPicker* m_pColorPicker;

	Level* m_pLevel;

	LightInfo* m_pLightInfo;
	LevelObjectInfo* m_pLevelObjectInfo;

	EditorLoader* m_pLoader;

	Snapper* m_pSnapper;

	const RenderContext* m_pRenderContext;

	INFO_STATE m_State;

	bool m_bInit;
	bool m_bSetLightColor;
};

