#include "ALOptions.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR */
ALOptions::ALOptions()
{
	m_SSAOSettings.radius = 0.5f;
	m_SSAOSettings.intensity = 8.0f;
	m_SSAOSettings.scale = 2.0f;
	m_SSAOSettings.bias = 0.03f;
	m_SSAOSettings.minIterations = 8;
	m_SSAOSettings.maxIterations = 16;

	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_normal.png")));
	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_hover.png")));
	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/subtract_small_normal.png")));
	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/subtract_small_hover.png")));

	m_Buttons["RADIUS_ADD"] = new Button(40,200,15,15);
	m_Buttons["RADIUS_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["RADIUS_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["RADIUS_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["RADIUS_SUB"] = new Button(20,200,15,15);
	m_Buttons["RADIUS_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["RADIUS_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["RADIUS_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["INTENSITY_ADD"] = new Button(40,259,15,15);
	m_Buttons["INTENSITY_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["INTENSITY_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["INTENSITY_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["INTENSITY_SUB"] = new Button(20,259,15,15);
	m_Buttons["INTENSITY_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["INTENSITY_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["INTENSITY_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["SCALE_ADD"] = new Button(40,318,15,15);
	m_Buttons["SCALE_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["SCALE_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["SCALE_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["SCALE_SUB"] = new Button(20,318,15,15);
	m_Buttons["SCALE_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["SCALE_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["SCALE_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["BIAS_ADD"] = new Button(40,377,15,15);
	m_Buttons["BIAS_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["BIAS_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["BIAS_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["BIAS_SUB"] = new Button(20,377,15,15);
	m_Buttons["BIAS_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["BIAS_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["BIAS_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["MIN_IT_ADD"] = new Button(40,436,15,15);
	m_Buttons["MIN_IT_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["MIN_IT_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["MIN_IT_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["MIN_IT_SUB"] = new Button(20,436,15,15);
	m_Buttons["MIN_IT_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["MIN_IT_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["MIN_IT_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["MAX_IT_ADD"] = new Button(40,495,15,15);
	m_Buttons["MAX_IT_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["MAX_IT_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["MAX_IT_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["MAX_IT_SUB"] = new Button(20,495,15,15);
	m_Buttons["MAX_IT_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["MAX_IT_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["MAX_IT_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_pFont = Content->LoadTextFormat(_T("Verdana"),12, false,false);
}

ALOptions::~ALOptions()
{
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		delete p.second;
	});
}

/* GENERAL */
void ALOptions::Tick()
{
	// buttons
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		p.second->Tick();
	});

	// radius
	if (m_Buttons["RADIUS_ADD"]->Clicked())
		m_SSAOSettings.radius += 0.1f;
	else if (m_Buttons["RADIUS_SUB"]->Clicked())
		m_SSAOSettings.radius -= 0.1f;

	// intensity
	if (m_Buttons["INTENSITY_ADD"]->Clicked())
		m_SSAOSettings.intensity += 0.5f;
	else if (m_Buttons["INTENSITY_SUB"]->Clicked())
		m_SSAOSettings.intensity -= 0.5f;

	// scale
	if (m_Buttons["SCALE_ADD"]->Clicked())
		m_SSAOSettings.scale += 0.25f;
	else if (m_Buttons["SCALE_SUB"]->Clicked())
		m_SSAOSettings.scale -= 0.25f;

	// bias
	if (m_Buttons["BIAS_ADD"]->Clicked())
		m_SSAOSettings.bias += 0.01f;
	else if (m_Buttons["BIAS_SUB"]->Clicked())
		m_SSAOSettings.bias -= 0.01f;

	// minIterations
	if (m_Buttons["MIN_IT_ADD"]->Clicked())
		m_SSAOSettings.minIterations += 2;
	else if (m_Buttons["MIN_IT_SUB"]->Clicked())
		m_SSAOSettings.minIterations -= 2;

	// maxIterations
	if (m_Buttons["MAX_IT_ADD"]->Clicked())
		m_SSAOSettings.maxIterations += 2;
	else if (m_Buttons["MAX_IT_SUB"]->Clicked())
		m_SSAOSettings.maxIterations -= 2;
}

void ALOptions::Draw()
{
	// buttons
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		p.second->Show();
	});

	tstringstream stream;
	stream.precision(3);

	stream << _T("SSAO:\n\n\n\n\n\n\n\n");
	stream << _T("Radius: ") << m_SSAOSettings.radius << _T("\n\n\n\n");
	stream << _T("Intensity: ") << m_SSAOSettings.intensity << _T("\n\n\n\n");
	stream << _T("Scale: ") << m_SSAOSettings.scale << _T("\n\n\n\n");
	stream << _T("Bias: ") << m_SSAOSettings.bias << _T("\n\n\n\n");
	stream << _T("Min Iterations: ") << m_SSAOSettings.minIterations << _T("\n\n\n\n");
	stream << _T("Max Iterations: ") << m_SSAOSettings.maxIterations << _T("\n\n\n\n");

	BX2D->SetColor(255,255,255);
	BX2D->SetFont(m_pFont);

	BX2D->DrawString(stream.str(), 10, 60);
}