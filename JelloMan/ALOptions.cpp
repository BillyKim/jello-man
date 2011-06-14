#include "ALOptions.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR */
ALOptions::ALOptions() : m_bUseAO(false)
{
	m_SSAOSettings.radius = 0.1f;
	m_SSAOSettings.intensity = 14.0f;
	m_SSAOSettings.scale = 2.0f;
	m_SSAOSettings.bias = 0.03f;
	m_SSAOSettings.minIterations = 8;
	m_SSAOSettings.maxIterations = 64;

	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_normal.png")));
	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_hover.png")));
	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/subtract_small_normal.png")));
	m_AddSubtractButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/subtract_small_hover.png")));

	m_UseAOButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/checkbox_empty_normal.png")));
	m_UseAOButtonImages.push_back(Content->LoadImage(_T("../Content/Images/Editor/checkbox_full_normal.png")));

	m_Buttons["USEAO"] = new Button(20,150,15,15, true);
	m_Buttons["USEAO"]->SetNormalState(m_UseAOButtonImages[1]);
	m_Buttons["USEAO"]->SetHoverState(m_UseAOButtonImages[1]);
	m_Buttons["USEAO"]->SetDownState(m_UseAOButtonImages[1]);
	m_Buttons["USEAO"]->SetDeactivatedState(m_UseAOButtonImages[0]);
	m_Buttons["USEAO"]->SetDeactivatedStateHover(m_UseAOButtonImages[0]);
	m_Buttons["USEAO"]->SetDeactivatedStateDown(m_UseAOButtonImages[0]);

	m_Buttons["USEAO"]->SetState(Button::STATE_DEACTIVATED);

	m_Buttons["RADIUS_ADD"] = new Button(40,230,15,15);
	m_Buttons["RADIUS_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["RADIUS_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["RADIUS_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["RADIUS_SUB"] = new Button(20,230,15,15);
	m_Buttons["RADIUS_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["RADIUS_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["RADIUS_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["INTENSITY_ADD"] = new Button(40,289,15,15);
	m_Buttons["INTENSITY_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["INTENSITY_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["INTENSITY_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["INTENSITY_SUB"] = new Button(20,289,15,15);
	m_Buttons["INTENSITY_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["INTENSITY_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["INTENSITY_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["SCALE_ADD"] = new Button(40,348,15,15);
	m_Buttons["SCALE_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["SCALE_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["SCALE_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["SCALE_SUB"] = new Button(20,348,15,15);
	m_Buttons["SCALE_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["SCALE_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["SCALE_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["BIAS_ADD"] = new Button(40,407,15,15);
	m_Buttons["BIAS_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["BIAS_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["BIAS_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["BIAS_SUB"] = new Button(20,407,15,15);
	m_Buttons["BIAS_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["BIAS_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["BIAS_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["MIN_IT_ADD"] = new Button(40,466,15,15);
	m_Buttons["MIN_IT_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["MIN_IT_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["MIN_IT_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["MIN_IT_SUB"] = new Button(20,466,15,15);
	m_Buttons["MIN_IT_SUB"]->SetNormalState(m_AddSubtractButtonImages[2]);
	m_Buttons["MIN_IT_SUB"]->SetHoverState(m_AddSubtractButtonImages[3]);
	m_Buttons["MIN_IT_SUB"]->SetDownState(m_AddSubtractButtonImages[3]);

	m_Buttons["MAX_IT_ADD"] = new Button(40,525,15,15);
	m_Buttons["MAX_IT_ADD"]->SetNormalState(m_AddSubtractButtonImages[0]);
	m_Buttons["MAX_IT_ADD"]->SetHoverState(m_AddSubtractButtonImages[1]);
	m_Buttons["MAX_IT_ADD"]->SetDownState(m_AddSubtractButtonImages[1]);

	m_Buttons["MAX_IT_SUB"] = new Button(20,525,15,15);
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
		m_SSAOSettings.radius += 0.02f;
	else if (m_Buttons["RADIUS_SUB"]->Clicked())
		m_SSAOSettings.radius -= 0.02f;

	// intensity
	if (m_Buttons["INTENSITY_ADD"]->Clicked())
		m_SSAOSettings.intensity += 0.5f;
	else if (m_Buttons["INTENSITY_SUB"]->Clicked())
		m_SSAOSettings.intensity -= 0.5f;

	// scale
	if (m_Buttons["SCALE_ADD"]->Clicked())
		m_SSAOSettings.scale += 0.2f;
	else if (m_Buttons["SCALE_SUB"]->Clicked())
		m_SSAOSettings.scale -= 0.2f;

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

	if (m_Buttons["USEAO"]->IsActive())
		m_bUseAO = true;
	else
		m_bUseAO = false;
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

	stream << _T("SSAO:\n\n\n\n");
	stream << _T("Use AO:\n\n\n\n\n\n");
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