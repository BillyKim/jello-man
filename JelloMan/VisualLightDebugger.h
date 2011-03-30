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
	void DeselectAll();
	void ShowLightInfo();

	// GETTERS
	int GetNrLightsSelected();

	vector<bool>& GetSpotLightsSelected()
	{ return m_SLightsSelected; }

	vector<bool>& GetPointLightsSelected()
	{ return m_PLightsSelected; }

private:

	void DeselectAllPointLights();
	void DeselectAllSpotLights();

	// DATAMEMBERS
	vector<Bitmap*> m_pPointLightBitmaps;
	vector<Bitmap*> m_pSpotLightBitmaps;
	vector<Bitmap*> m_pAddSubtractBitmaps;

	bool m_bClick;

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

	// VISUAL POINTLIGHT DEBUGGER
	vector<HitRegion*> m_pPLightHitRects;
	vector<bool> m_PLightsSelected;

	// VISUAL SPOTLIGHT DEBUGGER
	vector<HitRegion*> m_pSLightHitRects;
	vector<bool> m_SLightsSelected;
};