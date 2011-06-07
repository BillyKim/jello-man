#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include <map>
#include <vector>

class Infobar
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Infobar();
	virtual ~Infobar();

	/* GENERAL */
	void Draw();
	void ShowMessage(const tstring& message);

	void AddButtonHoverMessage(Button* pButton, const tstring& message);

	/* SETTERS */
	void SetSceneInfo(int nrLights, int nrSelectedLights, int nrObjects, int nrSelectedObjects);

private:

	/* DATAMEMBERS */
	bool m_bShowMessage;

	tstring m_Message;

	map<Button*, tstring> m_ButtonMessages;

	TextFormat* m_pInfoFont;

	vector<int> m_SceneInfo;
};

