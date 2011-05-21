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
												m_PreviousSelectedLight(9999),
												m_bLightsMoving(false),
												m_pLightInfoFont(0),
												m_pLightInfoFont2(0)
{
	
	// POINT LIGHT
	m_pPointLightBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/plight_normal.png")));
	m_pPointLightBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/plight_hover.png")));

	// SPOT LIGHT
	m_pSpotLightBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/slight_normal.png")));
	m_pSpotLightBitmaps.push_back(Content->LoadImage(_T("../Content/Images/Editor/slight_hover.png")));

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
}

void VisualLightDebugger::Draw()
{
	/*for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
	{
		BX2D->SetAntiAliasing(true);
		m_pRenderContext->GetLightController()->GetLights()[i]->Draw(m_pRenderContext);
		BX2D->SetAntiAliasing(false);
	}*/
}

void VisualLightDebugger::DeselectAllLights(int excluding)
{
	//if (excluding < 0)
	//{
	//	for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
	//	{
	//		m_pRenderContext->GetLightController()->GetLights()[i]->Deselect();
	//	}
	//}
	//else
	//{
	//	for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
	//	{
	//		if (i != excluding)
	//			m_pRenderContext->GetLightController()->GetLights()[i]->Deselect();
	//	}
	//}
}

void VisualLightDebugger::ShowLightInfo()
{
	tstringstream stream;
	stream.precision(8);

	if (GetNrLightsSelected() == 1)
	{
        LightType type = LightType_Point;
		int selectedLight = 0;

		for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
		{
			if (m_pRenderContext->GetLightController()->GetLights()[i]->IsSelected())
			{
				type = m_pRenderContext->GetLightController()->GetLights()[i]->GetType();
				selectedLight = i;
			}
		}

		m_pMultiplierAddButton->Tick();
		m_pMultiplierSubtractButton->Tick();
		m_pAttenuationAddButton->Tick();
		m_pAttenuationSubtractButton->Tick();

		if (type == LightType_Spot)
		{
			m_pPowerAddButton->Tick();
			m_pPowerSubtractButton->Tick();
		}

		Light* pLight = m_pRenderContext->GetLightController()->GetLights()[selectedLight];

		stream << _T("Light ") << selectedLight << _T(":\n\n");
		tstringstream strm;
		strm << pLight->GetPosition().X;
		tstring t = strm.str();
		stream << _T("X: ")  << _T("\n\n");
				
		if (!m_bTextBoxesSet || m_PreviousSelectedLight != selectedLight || m_bLightsMoving)
		{
			tstringstream strmX;
			strmX << t.substr(0, t.find(_T(".")) + 4);
			m_pTextBoxX->SetText(strmX.str());
		}

		strm.str(_T(""));
		strm << pLight->GetPosition().Y;
		t = strm.str();
		stream << _T("Y: ") << _T("\n\n");

		if (!m_bTextBoxesSet || m_PreviousSelectedLight != selectedLight || m_bLightsMoving)
		{
			tstringstream strmY;
			strmY << t.substr(0, t.find(_T(".")) + 4);
			m_pTextBoxY->SetText(strmY.str());
		}

		strm.str(_T(""));
		strm << pLight->GetPosition().Z;
		t = strm.str();
		stream << _T("Z: ") << _T("\n\n\n");

		if (!m_bTextBoxesSet || m_PreviousSelectedLight != selectedLight || m_bLightsMoving)
		{
			tstringstream strmZ;
			strmZ << t.substr(0, t.find(_T(".")) + 4);
			m_pTextBoxZ->SetText(strmZ.str());
		}

		m_bTextBoxesSet = true;

		stream << _T("R:") << static_cast<int>(pLight->GetColor().R * 255) << _T(" ");
		stream << _T("G:") << static_cast<int>(pLight->GetColor().G * 255) << _T(" ");
		stream << _T("B:") << static_cast<int>(pLight->GetColor().B * 255) << _T("\n\n\n\n\n\n");

		stream << _T("Multiplier: ") << pLight->GetMulitplier() << _T("\n\n\n\n");
		stream << _T("Attenuation end: ") << pLight->GetAttenuationEnd() << _T("\n\n\n\n");

        if (type == LightType_Spot)
		{
			SpotLight* pSLight = dynamic_cast<SpotLight*>(pLight);

			float angle = ToDegrees(pSLight->GetOpeningsAngle());
			//if (angle < 0.1f) angle = 0.0f; //would be weird for the debug -- should never be sub 0

			stream << _T("Angle: ") << angle << _T("\n\n\n\n");

			stream << _T("ShadowMaps enabled: ") << pSLight->HasShadowMap() << _T("\n");
			stream << _T("ShadowMaps size: ") << pSLight->GetShadowMapType();
		}

		BX2D->SetColor(	static_cast<int>(pLight->GetColor().R * 255),
							static_cast<int>(pLight->GetColor().G * 255),
							static_cast<int>(pLight->GetColor().B * 255)	);

		BX2D->FillRect(20, 215, 60, 40);

		// multiplier
		float multiplier = pLight->GetMulitplier();

		if (m_pMultiplierAddButton->Down())
		{
			multiplier += 0.1f;
		}
		else if (m_pMultiplierSubtractButton->Down())
		{
			multiplier -= 0.1f;
		}

		if (multiplier < 0.1f)
		{
			multiplier = 0.0f;
		}

		pLight->SetMulitplier(multiplier);

		// attenuation
		int attenuationEnd = static_cast<int>(pLight->GetAttenuationEnd());

		if (m_pAttenuationAddButton->Down())
		{
			attenuationEnd += 1;
		}
			
		if (m_pAttenuationSubtractButton->Down())
		{
			attenuationEnd -= 1;
		}

		if (attenuationEnd < 1)
		{
			attenuationEnd = 0;
		}

		if (type == LightType_Spot)
		{
			SpotLight* pSLight = dynamic_cast<SpotLight*>(pLight);

			if (m_pPowerAddButton->Down())
			{
			    pSLight->AddOpeningsAngle(0.01f); //radians
		        if (pSLight->GetOpeningsAngle() >= Pi)
		        {
			        pSLight->SetOpeningsAngle(Pi - 0.01f);
		        }
			}
			
			if (m_pPowerSubtractButton->Down())
			{
			    pSLight->AddOpeningsAngle(-0.01f);
		        if (pSLight->GetOpeningsAngle() < 0.01f)
		        {
			        pSLight->SetOpeningsAngle(0.01f);
		        }
			}
		}

		pLight->SetAttenuationEnd(static_cast<float>(attenuationEnd));

		m_PreviousSelectedLight = selectedLight;

		if (m_pTextBoxX->Entered())
		{
			float f;

			tstring s1 = m_pTextBoxX->GetText();

			string s2(s1.begin(), s1.end());

			const char* s = s2.c_str();

			f = static_cast<float>(atof(s));

			if (!(f == 0 && s2 != "0"))
			{
				Vector3 pos = pLight->GetPosition();
				pos.X = f;

				pLight->SetPosition(pos);
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
				Vector3 pos = pLight->GetPosition();
				pos.Y = f;

				pLight->SetPosition(pos);
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
				Vector3 pos = pLight->GetPosition();
				pos.Z = f;

				pLight->SetPosition(pos);
			}

			m_pTextBoxZ->LoseFocus();
		}
		
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

		if (type == LightType_Spot)
		{
			m_pPowerAddButton->Show();
			m_pPowerSubtractButton->Show();

			BX2D->DrawLine(10,445,190,445);
		}
	}
	else if (GetNrLightsSelected() > 1)
	{
		
		stream << _T("Selected:\n\n");

		for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
		{
			if (m_pRenderContext->GetLightController()->GetLights()[i]->IsSelected())
			{
				stream << _T("Light ") << i << _T(",\n");
			}
		}

		HideTextBoxes();
	}
	else
		HideTextBoxes();

	/*tstringstream streamL;

	streamL << _T("lights: ") << GetNrLightsSelected() << _T(" / ") << GetTotalLightsInScene();

	BX2D->SetColor(255, 255, 255, 0.5f);
	BX2D->SetFont(m_pLightInfoFont);

	m_pLightInfoFont->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
	m_pLightInfoFont->SetVerticalAlignment(PARAGRAPH_ALIGNMENT_BOTTOM);

	BX2D->DrawString(	streamL.str(),
						RectF(0,0, BX2D->GetWindowSize().width - 5, BX2D->GetWindowSize().height - 4));*/

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
		for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)	
		{
			if (m_pRenderContext->GetLightController()->GetLights()[i]->IsSelected())
				++b;
		}
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