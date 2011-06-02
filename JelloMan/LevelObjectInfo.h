#pragma once

#include "D3DUtil.h"
#include "RenderContext.h"
#include "ILevelObject.h"
#include "Level.h"

class LevelObjectInfo
{
public:

	/* CONSTRUCTOR - DESTRUCTOR*/
	LevelObjectInfo(Level* pLevel);
	virtual ~LevelObjectInfo();

	/* GENERAL */
	void Tick();
	void Draw(const RenderContext* pRenderContext);
	void ShowModelInfo();

	void HideTextBoxes();

	/* GETTERS */
	int GetNrObjectsSelected();

private:

	/* DATAMEMBERS */
	bool m_bTextBoxesSet;

	const RenderContext* m_pRenderContext;

	Level* m_pLevel;

	TextBox* m_pTextBoxX;
	TextBox* m_pTextBoxY;
	TextBox* m_pTextBoxZ;

	TextFormat* m_pObjectInfoFont;
	TextFormat* m_pObjectInfoFont2;

	ILevelObject* m_pLevelObject;
	ILevelObject* m_pPreviousLevelObject;

	Vector3 m_PreviousPos;
};

