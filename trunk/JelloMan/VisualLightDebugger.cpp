#include "VisualLightDebugger.h"
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ContentManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CONSTRUCTOR - DESTRUCTOR
VisualLightDebugger::VisualLightDebugger()	:	m_bClick(false),
												m_TotalSelectedLights(0),
												m_TotalLights(0),
												m_pRenderContext(0),
												m_pTextBoxX(0),
												m_pTextBoxY(0),
												m_pTextBoxZ(0),
												m_pMultiplierAddButton(0),
												m_pMultiplierSubtractButton(0),
												m_pAttenuationAddButton(0),
												m_pAttenuationSubtractButton(0),
												m_pPowerAddButton(0),
												m_pPowerSubtractButton(0),
												m_bTextBoxesSet(false),
												m_pLightInfoFont(0),
												m_pLightInfoFont2(0),
												m_pLight(0),
												m_pPreviousLight(0),
												m_bDown(false)
{
	// ADD - SUBTRACT BUTTON
	m_pAddSubtractBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_normal.png")));
	m_pAddSubtractBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/add_small_hover.png")));
	m_pAddSubtractBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/subtract_small_normal.png")));
	m_pAddSubtractBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/subtract_small_hover.png")));

	// multiplier
	m_pMultiplierAddButton = new Button(40,300,15,15);
	m_pMultiplierAddButton->SetNormalState(m_pAddSubtractBitmaps[0]);
	m_pMultiplierAddButton->SetHoverState(m_pAddSubtractBitmaps[1]);
	m_pMultiplierAddButton->SetDownState(m_pAddSubtractBitmaps[1]);

	m_pMultiplierSubtractButton = new Button(20,300,15,15);
	m_pMultiplierSubtractButton->SetNormalState(m_pAddSubtractBitmaps[2]);
	m_pMultiplierSubtractButton->SetHoverState(m_pAddSubtractBitmaps[3]);
	m_pMultiplierSubtractButton->SetDownState(m_pAddSubtractBitmaps[3]);

	// attenuation
	m_pAttenuationAddButton = new Button(40,359,15,15);
	m_pAttenuationAddButton->SetNormalState(m_pAddSubtractBitmaps[0]);
	m_pAttenuationAddButton->SetHoverState(m_pAddSubtractBitmaps[1]);
	m_pAttenuationAddButton->SetDownState(m_pAddSubtractBitmaps[1]);

	m_pAttenuationSubtractButton = new Button(20,359,15,15);
	m_pAttenuationSubtractButton->SetNormalState(m_pAddSubtractBitmaps[2]);
	m_pAttenuationSubtractButton->SetHoverState(m_pAddSubtractBitmaps[3]);
	m_pAttenuationSubtractButton->SetDownState(m_pAddSubtractBitmaps[3]);

	// power
	m_pPowerAddButton = new Button(40,418,15,15);
	m_pPowerAddButton->SetNormalState(m_pAddSubtractBitmaps[0]);
	m_pPowerAddButton->SetHoverState(m_pAddSubtractBitmaps[1]);
	m_pPowerAddButton->SetDownState(m_pAddSubtractBitmaps[1]);

	m_pPowerSubtractButton = new Button(20,418,15,15);
	m_pPowerSubtractButton->SetNormalState(m_pAddSubtractBitmaps[2]);
	m_pPowerSubtractButton->SetHoverState(m_pAddSubtractBitmaps[3]);
	m_pPowerSubtractButton->SetDownState(m_pAddSubtractBitmaps[3]);

	//// TEXTBOX
	//m_pTextBoxX = new TextBox();
	//m_pTextBoxX->SetBounds(30,85,80,20);

	//m_pTextBoxY = new TextBox();
	//m_pTextBoxY->SetBounds(30,115,80,20);

	//m_pTextBoxZ = new TextBox();
	//m_pTextBoxZ->SetBounds(30,145,80,20);

	// FONT
	m_pLightInfoFont = Content->LoadTextFormat(_T("Verdana"),10, false,false);
	m_pLightInfoFont2 = Content->LoadTextFormat(_T("Verdana"),12, false,false);

	m_ClickTimer.Reset();
}


VisualLightDebugger::~VisualLightDebugger()
{
	m_pRenderContext = 0;

	delete m_pTextBoxX;
	delete m_pTextBoxY;
	delete m_pTextBoxZ;

	delete m_pMultiplierAddButton;
	delete m_pMultiplierSubtractButton;
	delete m_pAttenuationAddButton;
	delete m_pAttenuationSubtractButton;
	delete m_pPowerAddButton;
	delete m_pPowerSubtractButton;
}

// GENERAL
void VisualLightDebugger::Tick(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	if (!m_pTextBoxX)
	{
		m_pTextBoxX = new TextBox();
		m_pTextBoxX->SetBounds(30,85,80,20);
	}

	if (!m_pTextBoxY)
	{
		m_pTextBoxY = new TextBox();
		m_pTextBoxY->SetBounds(30,115,80,20);
	}

	if (!m_pTextBoxZ)
	{
		m_pTextBoxZ = new TextBox();
		m_pTextBoxZ->SetBounds(30,145,80,20);
	}

	if (GetNrLightsSelected() == 1)
	{
		if (CONTROLS->LeftMBDown())
		{
			m_ClickTimer.Tick();

			m_dTime += m_ClickTimer.GetDeltaTime();
		}
		else
		{
			m_ClickTimer.Reset();

			m_dTime = 0;
			m_bDown = false;
		}

		float clickTime = m_ClickTimer.GetGameTime();

		for_each(m_pRenderContext->GetLightController()->GetLights().cbegin(), m_pRenderContext->GetLightController()->GetLights().cend(), [&](Light* pLight)
		{
			if (pLight->IsSelected())
			{
				m_LightType = pLight->GetType();

				m_pLight = pLight;
			}
		});

		m_pMultiplierAddButton->Tick();
		m_pMultiplierSubtractButton->Tick();
		m_pAttenuationAddButton->Tick();
		m_pAttenuationSubtractButton->Tick();

		if (m_LightType == LightType_Spot)
		{
			m_pPowerAddButton->Tick();
			m_pPowerSubtractButton->Tick();
		}

		if (m_PreviousPos != m_pLight->GetPosition())
			m_bTextBoxesSet = false;

		// multiplier
		float multiplier = m_pLight->GetMulitplier();

		if (!m_bDown)
		{
			if (m_pMultiplierAddButton->Down() && m_dTime > 0.1f || m_pMultiplierAddButton->Clicked())
			{
				multiplier += 0.1f;

				m_dTime = 0;
			}
			else if (m_pMultiplierSubtractButton->Down() && m_dTime > 0.1f || m_pMultiplierSubtractButton->Clicked())
			{
				multiplier -= 0.1f;

				m_dTime = 0;
			}
		}

		if (multiplier < 0.1f)
		{
			multiplier = 0.0f;
		}

		m_pLight->SetMulitplier(multiplier);

		// attenuation
		int attenuationEnd = static_cast<int>(m_pLight->GetAttenuationEnd());

		if (!m_bDown)
		{
			if (m_pAttenuationAddButton->Down() && m_dTime > 0.1f || m_pAttenuationAddButton->Clicked())
			{
				attenuationEnd += 1;

				m_dTime = 0;
			}

			if (m_pAttenuationSubtractButton->Down() && m_dTime > 0.1f || m_pAttenuationSubtractButton->Clicked())
			{
				attenuationEnd -= 1;

				m_dTime = 0;
			}
		}

		if (attenuationEnd < 1)
		{
			attenuationEnd = 0;
		}

		if (m_LightType == LightType_Spot)
		{
			SpotLight* pSLight = dynamic_cast<SpotLight*>(m_pLight);

			if (!m_bDown)
			{
				if (m_pPowerAddButton->Down() && m_dTime > 0.1f || m_pPowerAddButton->Clicked())
				{
					pSLight->AddOpeningsAngle(0.01f); //radians
					if (pSLight->GetOpeningsAngle() >= Pi)
					{
						pSLight->SetOpeningsAngle(Pi - 0.01f);
					}

					m_dTime = 0;
				}

				if (m_pPowerSubtractButton->Down() && m_dTime > 0.1f || m_pPowerSubtractButton->Clicked())
				{
					pSLight->AddOpeningsAngle(-0.01f);
					if (pSLight->GetOpeningsAngle() < 0.01f)
					{
						pSLight->SetOpeningsAngle(0.01f);
					}

					m_dTime = 0;
				}
			}
		}

		m_pLight->SetAttenuationEnd(static_cast<float>(attenuationEnd));

		m_pPreviousLight = m_pLight;

		if (m_pTextBoxX->Entered())
		{
			float f;

			tstring s1 = m_pTextBoxX->GetText();

			string s2(s1.begin(), s1.end());

			const char* s = s2.c_str();

			f = static_cast<float>(atof(s));

			if (!(f == 0 && s2 != "0"))
			{
				Vector3 pos = m_pLight->GetPosition();
				pos.X = f;

				m_pLight->SetPosition(pos);
			}

			m_pTextBoxX->LoseFocus();
		}

		if (m_pTextBoxY->Entered())
		{
			float f;

			tstring s1 = m_pTextBoxY->GetText();

			string s2(s1.begin(), s1.end());

			const char* s = s2.c_str();

			f = static_cast<float>(atof(s));

			if (!(f == 0 && s2 != "0"))
			{
				Vector3 pos = m_pLight->GetPosition();
				pos.Y = f;

				m_pLight->SetPosition(pos);
			}

			m_pTextBoxY->LoseFocus();
		}

		if (m_pTextBoxZ->Entered())
		{
			float f;

			tstring s1 = m_pTextBoxZ->GetText();

			string s2(s1.begin(), s1.end());

			const char* s = s2.c_str();

			f = static_cast<float>(atof(s));

			if (!(f == 0 && s2 != "0"))
			{
				Vector3 pos = m_pLight->GetPosition();
				pos.Z = f;

				m_pLight->SetPosition(pos);
			}

			m_pTextBoxZ->LoseFocus();
		}

		m_PreviousPos = m_pLight->GetPosition();

		if (clickTime < 0.5)
			m_bDown = true;
		else
			m_bDown = false;
	}
}

void VisualLightDebugger::Draw()
{
	if (GetNrLightsSelected() == 1)
	{
		//if (m_pLight)
			ShowLightInfo();
	}
	else if (GetNrLightsSelected() > 1)
	{
		tstringstream stream;
		stream.precision(8);

		stream << _T("Selected:\n\n");

		for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
		{
			if (m_pRenderContext->GetLightController()->GetLights()[i]->IsSelected())
			{
				stream << _T("Light ") << i << _T(",\n");
			}
		}

		BX2D->SetColor(255, 255, 255);
		BX2D->SetFont(m_pLightInfoFont2);
		BX2D->DrawString(stream.str(),10,60);

		HideTextBoxes();
	}
	else
		HideTextBoxes();
}

void VisualLightDebugger::ShowLightInfo()
{
	tstringstream stream;
	stream.precision(8);

	if (m_LightType == LightType_Spot)
		stream << _T("Spotlight selected:\n\n");
	else
		stream << _T("Pointlight selected:\n\n");

	tstringstream strm;
	strm << m_pLight->GetPosition().X;

	tstring t = strm.str();
	stream << _T("X: ")  << _T("\n\n");
				
	if (!m_bTextBoxesSet || m_pPreviousLight != m_pLight)
	{
		tstringstream strmX;
		strmX << t.substr(0, t.find(_T(".")) + 4);
		m_pTextBoxX->SetText(strmX.str());
	}

	strm.str(_T(""));
	strm << m_pLight->GetPosition().Y;
	t = strm.str();
	stream << _T("Y: ") << _T("\n\n");

	if (!m_bTextBoxesSet || m_pPreviousLight != m_pLight)
	{
		tstringstream strmY;
		strmY << t.substr(0, t.find(_T(".")) + 4);
		m_pTextBoxY->SetText(strmY.str());
	}

	strm.str(_T(""));
	strm << m_pLight->GetPosition().Z;
	t = strm.str();
	stream << _T("Z: ") << _T("\n\n\n");

	if (!m_bTextBoxesSet || m_pPreviousLight != m_pLight)
	{
		tstringstream strmZ;
		strmZ << t.substr(0, t.find(_T(".")) + 4);
		m_pTextBoxZ->SetText(strmZ.str());
	}

	m_bTextBoxesSet = true;

	stream << _T("R:") << static_cast<int>(m_pLight->GetColor().R * 255) << _T(" ");
	stream << _T("G:") << static_cast<int>(m_pLight->GetColor().G * 255) << _T(" ");
	stream << _T("B:") << static_cast<int>(m_pLight->GetColor().B * 255) << _T("\n\n\n\n\n\n");

	stream << _T("Multiplier: ") << m_pLight->GetMulitplier() << _T("\n\n\n\n");
	stream << _T("Attenuation end: ") << m_pLight->GetAttenuationEnd() << _T("\n\n\n\n");

    if (m_LightType == LightType_Spot)
	{
		SpotLight* pSLight = dynamic_cast<SpotLight*>(m_pLight);

		float angle = ToDegrees(pSLight->GetOpeningsAngle());
		//if (angle < 0.1f) angle = 0.0f; //would be weird for the debug -- should never be sub 0

		stream << _T("Angle: ") << angle << _T("\n\n\n\n");

		stream << _T("ShadowMaps enabled: ") << pSLight->HasShadowMap() << _T("\n");
		stream << _T("ShadowMaps size: ") << pSLight->GetShadowMapType();
	}

	BX2D->SetColor(	static_cast<int>(m_pLight->GetColor().R * 255),
					static_cast<int>(m_pLight->GetColor().G * 255),
					static_cast<int>(m_pLight->GetColor().B * 255)	);

	BX2D->FillRect(20, 215, 60, 40);
		
	m_pTextBoxX->Show();
	m_pTextBoxY->Show();
	m_pTextBoxZ->Show();
		
	m_pMultiplierAddButton->Show();
	m_pMultiplierSubtractButton->Show();
	m_pAttenuationAddButton->Show();
	m_pAttenuationSubtractButton->Show();

	BX2D->SetColor(40,40,40);
	BX2D->DrawLine(10,180,190,180);
	BX2D->DrawLine(10,270,190,270);
	BX2D->DrawLine(10,330,190,330);
	BX2D->DrawLine(10,387,190,387);

	if (m_LightType == LightType_Spot)
	{
		m_pPowerAddButton->Show();
		m_pPowerSubtractButton->Show();

		BX2D->DrawLine(10,445,190,445);
	}

	BX2D->SetColor(255, 255, 255);
	BX2D->SetFont(m_pLightInfoFont2);
	BX2D->DrawString(stream.str(),10,60);
}

// GETTERS
int VisualLightDebugger::GetNrLightsSelected()
{
	int b = 0;

	if (m_pRenderContext)
	{
		for_each(m_pRenderContext->GetLightController()->GetLights().cbegin(), m_pRenderContext->GetLightController()->GetLights().cend(), [&](Light* pLight)
		{
			if (pLight->IsSelected())
				++b;
		});
	}

	return b;
}

int VisualLightDebugger::GetTotalLightsInScene()
{
	if (m_pRenderContext)
		return m_pRenderContext->GetLightController()->GetLights().size();
	else
		return 0;
}

void VisualLightDebugger::HideTextBoxes()
{
	m_pTextBoxX->Hide();
	m_pTextBoxY->Hide();
	m_pTextBoxZ->Hide();

	m_bTextBoxesSet = false;
}