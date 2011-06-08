#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include "RenderContext.h"
#include <vector>
#include "Light.h"

class LightInfo
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	LightInfo();
	virtual ~LightInfo();

	// GENERAL
	void Tick(const RenderContext* renderContext);
	void Draw();
	void ShowLightInfo();

	void HideTextBoxes();

	// GETTERS
	int GetNrLightsSelected();
	int GetTotalLightsInScene();

	// SETTERS
	void MovingLights(bool bMoving)
	{ m_bLightsMoving = bMoving; }

private:

	// DATAMEMBERS
	vector<Image*> m_pAddSubtractBitmaps;

	bool m_bClick;
	bool m_bTextBoxesSet;
	bool m_bLightsMoving;
	bool m_bDown;

	int m_TotalSelectedLights;
	int m_TotalLights;

	const RenderContext* m_pRenderContext;

	TextBox* m_pTextBoxX;
	TextBox* m_pTextBoxY;
	TextBox* m_pTextBoxZ;

	Button* m_pMultiplierAddButton;
	Button* m_pMultiplierSubtractButton;
	Button* m_pAttenuationAddButton;
	Button* m_pAttenuationSubtractButton;
	Button* m_pPowerAddButton;
	Button* m_pPowerSubtractButton;

	TextFormat* m_pLightInfoFont;
	TextFormat* m_pLightInfoFont2;

	LightType m_LightType;
	Light* m_pLight;
	Light* m_pPreviousLight;

	Vector3 m_PreviousPos;

	GameTimer m_ClickTimer;

	float m_dTime;

	// copy & assignment operator
	LightInfo(const LightInfo& second);
	LightInfo& operator=(const LightInfo& second);
};