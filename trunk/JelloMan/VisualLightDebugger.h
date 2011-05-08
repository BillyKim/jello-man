#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include "RenderContext.h"
#include <vector>

class VisualLightDebugger
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	VisualLightDebugger();
	virtual ~VisualLightDebugger();

	// GENERAL
	void Tick(const RenderContext* renderContext);
	void CheckControls();
	void Draw();
	void ShowLightInfo();

	void DeselectAllLights(int excluding = -1);

	void HideTextBoxes();
	void MovingLights(bool bMoving)
	{ m_bLightsMoving = bMoving; }

	// GETTERS
	int GetNrLightsSelected();
	int GetTotalLightsInScene();

private:

	// DATAMEMBERS
	vector<Image*> m_pPointLightBitmaps;
	vector<Image*> m_pSpotLightBitmaps;
	vector<Image*> m_pAddSubtractBitmaps;

	bool m_bClick;
	bool m_bTextBoxesSet;
	bool m_bLightsMoving;

	int m_TotalSelectedLights;
	int m_TotalLights;
	int m_PreviousSelectedLight;

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
};