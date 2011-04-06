#include "VisualLightDebugger.h"

// CONVERT STRING TO OTHER TYPE - FLOAT IE
template <class T>
bool from_string(T& t, 
                 const string& s, 
                 ios_base& (*f)(ios_base&))
{
  istringstream iss(s);
  return !(iss >> f >> t).fail();
}

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
												m_bLightsMoving(false)
{
	
	// POINT LIGHT
	m_pPointLightBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/plight_normal.png")));
	m_pPointLightBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/plight_hover.png")));

	// SPOT LIGHT
	m_pSpotLightBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/slight_normal.png")));
	m_pSpotLightBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/slight_hover.png")));

	// ADD - SUBTRACT BUTTON
	m_pAddSubtractBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/add_small_normal.png")));
	m_pAddSubtractBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/add_small_hover.png")));
	m_pAddSubtractBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/subtract_small_normal.png")));
	m_pAddSubtractBitmaps.push_back(new Bitmap(_T("Content/Images/Editor/subtract_small_hover.png")));

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

	// TEXTBOX
	m_pTextBoxX = new TextBox();
	m_pTextBoxX->SetBounds(30,85,80,20);

	m_pTextBoxY = new TextBox();
	m_pTextBoxY->SetBounds(30,115,80,20);

	m_pTextBoxZ = new TextBox();
	m_pTextBoxZ->SetBounds(30,145,80,20);
}


VisualLightDebugger::~VisualLightDebugger()
{
	m_pRenderContext = 0;

	for (vector<Bitmap*>::iterator it = m_pPointLightBitmaps.begin(); it != m_pPointLightBitmaps.end(); ++it)
	{
		delete (*it);
	}

	for (vector<Bitmap*>::iterator it = m_pSpotLightBitmaps.begin(); it != m_pSpotLightBitmaps.end(); ++it)
	{
		delete (*it);
	}

	for (vector<HitRegion*>::iterator it = m_pPLightHitRects.begin(); it != m_pPLightHitRects.end(); ++it)
	{
		delete (*it);
	}

	for (vector<HitRegion*>::iterator it = m_pSLightHitRects.begin(); it != m_pSLightHitRects.end(); ++it)
	{
		delete (*it);
	}

	for (vector<Bitmap*>::iterator it = m_pAddSubtractBitmaps.begin(); it != m_pAddSubtractBitmaps.end(); ++it)
	{
		delete (*it);
	}

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

	while (pRenderContext->GetLightController()->GetPointLights().size() > m_PLightsSelected.size())
	{
		m_pPLightHitRects.push_back(new HitRegion(HitRegion::TYPE_ELLIPSE, -10 ,-10, 10, 10));
		m_PLightsSelected.push_back(false);
	}

	while (pRenderContext->GetLightController()->GetSpotLights().size() > m_SLightsSelected.size())
	{
		m_pSLightHitRects.push_back(new HitRegion(HitRegion::TYPE_ELLIPSE, -10 ,-10, 10, 10));
		m_SLightsSelected.push_back(false);
	}

	if (!m_bLightsMoving)
		CheckControls();
}

void VisualLightDebugger::CheckControls()
{
	/*if (CONTROLS->IsKeyPressed(VK_DELETE))
	{
		int i = 0;

		vector<PointLight>::iterator it = m_pRenderContext->GetLightController()->GetPointLights().begin();

		vector<vector<PointLight>::iterator> positionsToDelete;

		for (; it != m_pRenderContext->GetLightController()->GetPointLights().end(); ++it)
		{
			if (m_PLightsSelected[i])
				positionsToDelete.push_back(it);

			++i;
		}

		for (int i2 = 0; i2 < positionsToDelete.size(); ++i2)
		{
			m_pRenderContext->GetLightController()->GetPointLights().erase(positionsToDelete[i2]);
		}
	}*/

	if (CONTROLS->IsKeyDown(VK_LCONTROL) && CONTROLS->IsKeyDown('D'))
	{
		for (unsigned int i = 0; i < m_PLightsSelected.size(); ++i)
		{
			m_PLightsSelected[i] = false;
		}

		for (unsigned int i = 0; i < m_SLightsSelected.size(); ++i)
		{
			m_SLightsSelected[i] = false;
		}
	}
	if (CONTROLS->LeftMBDown() || CONTROLS->RightMBDown())
	{
		m_pTextBoxX->LoseFocus();
		m_pTextBoxY->LoseFocus();
		m_pTextBoxZ->LoseFocus();
	}

	if (CONTROLS->LeftMBDown())
		m_bClick = true;

	if (CONTROLS->LeftMBUp())
	{
		if (m_bClick)
		{
			if (CONTROLS->IsKeyDown(VK_LCONTROL))
			{
				for (unsigned int i = 0; i < m_pPLightHitRects.size(); ++i)
				{
					if (m_pPLightHitRects[i]->HitTest(CONTROLS->GetMousePos()))
						m_PLightsSelected[i] = !m_PLightsSelected[i];
				}

				for (unsigned int i = 0; i < m_pSLightHitRects.size(); ++i)
				{
					if (m_pSLightHitRects[i]->HitTest(CONTROLS->GetMousePos()))
						m_SLightsSelected[i] = !m_SLightsSelected[i];
				}
			}
			else
			{
				for (unsigned int i = 0; i < m_pPLightHitRects.size(); ++i)
				{
					if (m_pPLightHitRects[i]->HitTest(CONTROLS->GetMousePos()))
					{
						m_PLightsSelected[i] = !m_PLightsSelected[i];
					
						for (unsigned int i2 = 0; i2 < m_PLightsSelected.size(); ++i2)
						{
							if (i != i2)
								m_PLightsSelected[i2] = false;
						}

						DeselectAllSpotLights();
					}
				}

				for (unsigned int i = 0; i < m_pSLightHitRects.size(); ++i)
				{
					if (m_pSLightHitRects[i]->HitTest(CONTROLS->GetMousePos()))
					{
						m_SLightsSelected[i] = !m_SLightsSelected[i];
					
						for (unsigned int i2 = 0; i2 < m_SLightsSelected.size(); ++i2)
						{
							if (i != i2)
								m_SLightsSelected[i2] = false;
						}
						
						DeselectAllPointLights();
					}
				}
			}

		m_bClick = false;

		}
	}
}

void VisualLightDebugger::Draw()
{
	// MATRIX
	D3DXMATRIX matProj = m_pRenderContext->GetCamera()->GetProjection();
	D3DXMATRIX matView = m_pRenderContext->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();
	Vector3 vRight = m_pRenderContext->GetCamera()->GetRight();

	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 0.0f;
	viewP.MaxDepth = 1.0f;

	int size = 10;

	// POINTLIGHTS
	for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetPointLights().size(); ++i)
	{
		D3DXVECTOR3 pos = m_pRenderContext->GetLightController()->GetPointLights()[i].position.ToD3DVector3();
	
		Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - Vector3(pos);
		float l = length.Length();
		l *= 0.001f;

		// VIEWPORT PROJECTION
		D3DXVECTOR3 pos2D;
		D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

		// HITRECT
		SafeDelete(m_pPLightHitRects[i]);
		m_pPLightHitRects[i] = new HitRegion(	
			HitRegion::TYPE_ELLIPSE,
			static_cast<int>(pos2D.x),
			static_cast<int>(pos2D.y), 
			static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (16 * l)),
			static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (16 * l)));

		if (vLook.Dot(length) < 0)
		{
			// DRAW
			if (m_pPLightHitRects[i]->HitTest(CONTROLS->GetMousePos()) || m_PLightsSelected[i] == true)
			{
				BLOX_2D->DrawBitmap(m_pPointLightBitmaps[1],
									static_cast<int>(pos2D.x - ((m_pPointLightBitmaps[0]->GetSize().width / (8 * l)) / 2)),
									static_cast<int>(pos2D.y - (m_pPointLightBitmaps[0]->GetSize().height / (8 * l)) / 2),
									1.0f / l,
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (8 * l)),
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (8 * l)));
			}
			else
			{
				BLOX_2D->DrawBitmap(m_pPointLightBitmaps[0],
									static_cast<int>(pos2D.x - ((m_pPointLightBitmaps[0]->GetSize().width / (8 * l)) / 2)),
									static_cast<int>(pos2D.y - (m_pPointLightBitmaps[0]->GetSize().height / (8 * l)) / 2),
									1.0f / l,
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (8 * l)),
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (8 * l)));
			}
		}
	}

	// SPOTLIGHTS
	for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetSpotLights().size(); ++i)
	{
		D3DXVECTOR3 pos = m_pRenderContext->GetLightController()->GetSpotLights()[i].position.ToD3DVector3();
	
		Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - Vector3(pos);
		float l = length.Length();
		l *= 0.001f;

		// VIEWPORT PROJECTION
		D3DXVECTOR3 pos2D;
		D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

		// HITRECT
		SafeDelete(m_pSLightHitRects[i]);
		m_pSLightHitRects[i] = new HitRegion(	
			HitRegion::TYPE_ELLIPSE,
			static_cast<int>(pos2D.x),
			static_cast<int>(pos2D.y), 
			static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (16 * l)),
			static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (16 * l)));

		if (vLook.Dot(length) < 0)
		{
			// DRAW
			if (m_pSLightHitRects[i]->HitTest(CONTROLS->GetMousePos()) || m_SLightsSelected[i] == true)
			{
				BLOX_2D->DrawBitmap(m_pSpotLightBitmaps[1],
									static_cast<int>(pos2D.x - ((m_pPointLightBitmaps[0]->GetSize().width / (8 * l)) / 2)),
									static_cast<int>(pos2D.y - (m_pPointLightBitmaps[0]->GetSize().height / (8 * l)) / 2),
									1.0f / l,
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (8 * l)),
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (8 * l)));
			}
			else
			{
				BLOX_2D->DrawBitmap(m_pSpotLightBitmaps[0],
									static_cast<int>(pos2D.x - ((m_pPointLightBitmaps[0]->GetSize().width / (8 * l)) / 2)),
									static_cast<int>(pos2D.y - (m_pPointLightBitmaps[0]->GetSize().height / (8 * l)) / 2),
									1.0f / l,
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().width / (8 * l)),
									static_cast<int>(m_pPointLightBitmaps[0]->GetSize().height / (8 * l)));
			}
		}
	}
}

void VisualLightDebugger::DeselectAllPointLights()
{
	for (unsigned int i = 0; i < m_PLightsSelected.size(); ++i)
	{
		m_PLightsSelected[i] = false;
	}
}

void VisualLightDebugger::DeselectAllSpotLights()
{
	for (unsigned int i = 0; i < m_SLightsSelected.size(); ++i)
	{
		m_SLightsSelected[i] = false;
	}
}

void VisualLightDebugger::DeselectAll()
{
	DeselectAllPointLights();
	DeselectAllSpotLights();
}

void VisualLightDebugger::ShowLightInfo()
{
	tstringstream stream;

	if (GetNrLightsSelected() == 1)
	{
		int type = 0;

		for (unsigned int i = 0; i < m_SLightsSelected.size(); ++i)
		{
			if (m_SLightsSelected[i] == true)
				type = 1;
		}

		m_pMultiplierAddButton->Tick();
		m_pMultiplierSubtractButton->Tick();
		m_pAttenuationAddButton->Tick();
		m_pAttenuationSubtractButton->Tick();

		if (type == 1)
		{
			m_pPowerAddButton->Tick();
			m_pPowerSubtractButton->Tick();
		}

		for (unsigned int i = 0; i < m_PLightsSelected.size(); ++i)
		{
			if (m_PLightsSelected[i] == true)
			{
				stream << _T("PointLight ") << i << _T(":\n\n");
				tstringstream strm;
				strm << m_pRenderContext->GetLightController()->GetPointLights()[i].position.X;
				tstring t = strm.str();
				stream << _T("X: ")  << _T("\n\n");
				
				if (!m_bTextBoxesSet || m_PreviousSelectedLight != i + 1000 || m_bLightsMoving)
				{
					tstringstream strmX;
					strmX << t.substr(0, t.find(_T(".")) + 4);
					m_pTextBoxX->SetText(strmX.str());
				}

				strm.str(_T(""));
				strm << m_pRenderContext->GetLightController()->GetPointLights()[i].position.Y;
				t = strm.str();
				stream << _T("Y: ") << _T("\n\n");

				if (!m_bTextBoxesSet || m_PreviousSelectedLight != i + 1000 || m_bLightsMoving)
				{
					tstringstream strmY;
					strmY << t.substr(0, t.find(_T(".")) + 4);
					m_pTextBoxY->SetText(strmY.str());
				}

				strm.str(_T(""));
				strm << m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z;
				t = strm.str();
				stream << _T("Z: ") << _T("\n\n\n");

				if (!m_bTextBoxesSet || m_PreviousSelectedLight != i + 1000 || m_bLightsMoving)
				{
					tstringstream strmZ;
					strmZ << t.substr(0, t.find(_T(".")) + 4);
					m_pTextBoxZ->SetText(strmZ.str());
				}

				m_bTextBoxesSet = true;

				stream << _T("R:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.R * 255) << _T(" ");
				stream << _T("G:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.G * 255) << _T(" ");
				stream << _T("B:") << static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.B * 255) << _T("\n\n\n\n\n\n");

				stream << _T("Multiplier: ") << m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier << _T("\n\n\n\n");
				stream << _T("Attenuation end: ") << m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd;

				BLOX_2D->SetColor(	static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.R * 255),
									static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.G * 255),
									static_cast<int>(m_pRenderContext->GetLightController()->GetPointLights()[i].color.B * 255)	);

				BLOX_2D->FillRect(20, 215, 60, 40);

				// multiplier
				if (m_pMultiplierAddButton->Down())
					m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier += 0.1f;
			
				if (m_pMultiplierSubtractButton->Down())
					m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier -= 0.1f;

				if (m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier < 0.1f)
						m_pRenderContext->GetLightController()->GetPointLights()[i].multiplier = 0;

				// attenuation
				if (m_pAttenuationAddButton->Down())
					m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd += 100;
			
				if (m_pAttenuationSubtractButton->Down())
					m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd -= 100;

				if (m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd < 100)
						m_pRenderContext->GetLightController()->GetPointLights()[i].AttenuationEnd = 0;

				m_PreviousSelectedLight = i + 1000;

				if (m_pTextBoxX->Entered())
				{
					float f;

					tstring s1 = m_pTextBoxX->GetText();

					string s2(s1.begin(), s1.end());

					const char* s = s2.c_str();

					f = atof(s);

					if (!(f == 0 && s2 != "0"))
						m_pRenderContext->GetLightController()->GetPointLights()[i].position.X = f;

					m_pTextBoxX->LoseFocus();
				}

				if (m_pTextBoxY->Entered())
				{
					float f;

					tstring s1 = m_pTextBoxY->GetText();

					string s2(s1.begin(), s1.end());

					const char* s = s2.c_str();

					f = atof(s);

					if (!(f == 0 && s2 != "0"))
						m_pRenderContext->GetLightController()->GetPointLights()[i].position.Y = f;

					m_pTextBoxY->LoseFocus();
				}

				if (m_pTextBoxZ->Entered())
				{
					float f;

					tstring s1 = m_pTextBoxZ->GetText();

					string s2(s1.begin(), s1.end());

					const char* s = s2.c_str();

					f = atof(s);

					if (!(f == 0 && s2 != "0"))
						m_pRenderContext->GetLightController()->GetPointLights()[i].position.Z = f;

					m_pTextBoxZ->LoseFocus();
				}
			}
		}

		for (unsigned int i = 0; i < m_SLightsSelected.size(); ++i)
		{
			if (m_SLightsSelected[i] == true)
			{
				stream << _T("SpotLight ") << i << _T(":\n\n");
				tstringstream strm;
				strm << m_pRenderContext->GetLightController()->GetSpotLights()[i].position.X;
				tstring t = strm.str();
				stream << _T("X: ")  << _T("\n\n");
				
				if (!m_bTextBoxesSet || m_PreviousSelectedLight != i || m_bLightsMoving)
				{
					tstringstream strmX;
					strmX << t.substr(0, t.find(_T(".")) + 4);
					m_pTextBoxX->SetText(strmX.str());
				}

				strm.str(_T(""));
				strm << m_pRenderContext->GetLightController()->GetSpotLights()[i].position.Y;
				t = strm.str();
				stream << _T("Y: ") << _T("\n\n");

				if (!m_bTextBoxesSet || m_PreviousSelectedLight != i  || m_bLightsMoving)
				{
					tstringstream strmY;
					strmY << t.substr(0, t.find(_T(".")) + 4);
					m_pTextBoxY->SetText(strmY.str());
				}

				strm.str(_T(""));
				strm << m_pRenderContext->GetLightController()->GetSpotLights()[i].position.Z;
				t = strm.str();
				stream << _T("Z: ") << _T("\n\n\n");

				if (!m_bTextBoxesSet || m_PreviousSelectedLight != i  || m_bLightsMoving)
				{
					tstringstream strmZ;
					strmZ << t.substr(0, t.find(_T(".")) + 4);
					m_pTextBoxZ->SetText(strmZ.str());
				}

				m_bTextBoxesSet = true;

				stream << _T("R:") << static_cast<int>(m_pRenderContext->GetLightController()->GetSpotLights()[i].color.R * 255) << _T(" ");
				stream << _T("G:") << static_cast<int>(m_pRenderContext->GetLightController()->GetSpotLights()[i].color.G * 255) << _T(" ");
				stream << _T("B:") << static_cast<int>(m_pRenderContext->GetLightController()->GetSpotLights()[i].color.B * 255) << _T("\n\n\n\n\n\n");

				stream << _T("Multiplier: ") << m_pRenderContext->GetLightController()->GetSpotLights()[i].multiplier << _T("\n\n\n\n");
				stream << _T("Attenuation end: ") << m_pRenderContext->GetLightController()->GetSpotLights()[i].AttenuationEnd << _T("\n\n\n\n");
				stream << _T("Power: ") << m_pRenderContext->GetLightController()->GetSpotLights()[i].power << _T("\n\n\n\n");

				stream << _T("ShadowMaps enabled: ") << m_pRenderContext->GetLightController()->GetSpotLights()[i].shadowsEnabled << _T("\n");
				stream << _T("ShadowMaps size: ") << m_pRenderContext->GetLightController()->GetSpotLights()[i].shadowMapSize;

				BLOX_2D->SetColor(	static_cast<int>(m_pRenderContext->GetLightController()->GetSpotLights()[i].color.R * 255),
									static_cast<int>(m_pRenderContext->GetLightController()->GetSpotLights()[i].color.G * 255),
									static_cast<int>(m_pRenderContext->GetLightController()->GetSpotLights()[i].color.B * 255)	);

				BLOX_2D->FillRect(20, 215, 60, 40);

				// multiplier
				if (m_pMultiplierAddButton->Down())
					m_pRenderContext->GetLightController()->GetSpotLights()[i].multiplier += 0.1f;

				if (m_pMultiplierSubtractButton->Down())
					m_pRenderContext->GetLightController()->GetSpotLights()[i].multiplier -= 0.1f;

				if (m_pRenderContext->GetLightController()->GetSpotLights()[i].multiplier < 0.1f)
						m_pRenderContext->GetLightController()->GetSpotLights()[i].multiplier = 0;

				// attenuation
				if (m_pAttenuationAddButton->Down())
					m_pRenderContext->GetLightController()->GetSpotLights()[i].AttenuationEnd += 100;
			
				if (m_pAttenuationSubtractButton->Down())
					m_pRenderContext->GetLightController()->GetSpotLights()[i].AttenuationEnd -= 100;

				if (m_pRenderContext->GetLightController()->GetSpotLights()[i].AttenuationEnd < 100)
						m_pRenderContext->GetLightController()->GetSpotLights()[i].AttenuationEnd = 0;

				// power
				if (m_pPowerAddButton->Down())
					m_pRenderContext->GetLightController()->GetSpotLights()[i].power += 0.1f;
			
				if (m_pPowerSubtractButton->Down())
					m_pRenderContext->GetLightController()->GetSpotLights()[i].power-= 0.1f;

				if (m_pRenderContext->GetLightController()->GetSpotLights()[i].power < 0.1f)
						m_pRenderContext->GetLightController()->GetSpotLights()[i].power = 0;

				m_PreviousSelectedLight = i;

				if (m_pTextBoxX->Entered())
				{
					float f;

					tstring s1 = m_pTextBoxX->GetText();

					string s2(s1.begin(), s1.end());

					const char* s = s2.c_str();

					f = atof(s);

					if (!(f == 0 && s2 != "0"))
						m_pRenderContext->GetLightController()->GetSpotLights()[i].position.X = f;

					m_pTextBoxX->LoseFocus();
				}

				if (m_pTextBoxY->Entered())
				{
					float f;

					tstring s1 = m_pTextBoxY->GetText();

					string s2(s1.begin(), s1.end());

					const char* s = s2.c_str();

					f = atof(s);

					if (!(f == 0 && s2 != "0"))
						m_pRenderContext->GetLightController()->GetSpotLights()[i].position.Y = f;

					m_pTextBoxY->LoseFocus();
				}

				if (m_pTextBoxZ->Entered())
				{
					float f;

					tstring s1 = m_pTextBoxZ->GetText();

					string s2(s1.begin(), s1.end());

					const char* s = s2.c_str();

					f = atof(s);

					if (!(f == 0 && s2 != "0"))
						m_pRenderContext->GetLightController()->GetSpotLights()[i].position.Z = f;

					m_pTextBoxZ->LoseFocus();
				}
			}
		}

		m_pTextBoxX->Show();
		m_pTextBoxY->Show();
		m_pTextBoxZ->Show();
		
		m_pMultiplierAddButton->Show();
		m_pMultiplierSubtractButton->Show();
		m_pAttenuationAddButton->Show();
		m_pAttenuationSubtractButton->Show();

		BLOX_2D->SetColor(40,40,40);
		BLOX_2D->DrawLine(10,180,190,180);
		BLOX_2D->DrawLine(10,270,190,270);
		BLOX_2D->DrawLine(10,330,190,330);
		BLOX_2D->DrawLine(10,387,190,387);

		if (type == 1)
		{
			m_pPowerAddButton->Show();
			m_pPowerSubtractButton->Show();

			BLOX_2D->DrawLine(10,445,190,445);
		}
	}
	else if (GetNrLightsSelected() > 1)
	{
		
		stream << _T("Selected:\n\n");

		for (unsigned int i = 0; i < m_PLightsSelected.size(); ++i)
		{
			if (m_PLightsSelected[i] == true)
			{
				stream << _T("pointLight ") << i << _T(",\n");
			}
		}

		stream << _T("\n");

		for (unsigned int i = 0; i < m_SLightsSelected.size(); ++i)
		{
			if (m_SLightsSelected[i] == true)
			{
				stream << _T("spotLight ") << i << _T(",\n");
			}
		}

		HideTextBoxes();
	}
	else
		HideTextBoxes();

	tstringstream streamL;

	streamL << _T("lights: ") << GetNrLightsSelected() << _T(" / ") << GetTotalLightsInScene();

	BLOX_2D->SetColor(255, 255, 255, 0.5f);
	BLOX_2D->SetFont(_T("Verdana"),false,false,10);
	BLOX_2D->DrawString(	streamL.str(),
							RectF(0,0, BLOX_2D->GetWindowSize().width - 5, BLOX_2D->GetWindowSize().height - 4),
							Blox2D::HORIZONTAL_ALIGN_RIGHT,
							Blox2D::VERTICAL_ALIGN_BOTTOM);

	BLOX_2D->SetColor(255, 255, 255);
	BLOX_2D->SetFont(_T("Verdana"),false,false,12);
	BLOX_2D->DrawString(stream.str(),10,60);
}

// GETTERS
int VisualLightDebugger::GetNrLightsSelected()
{
	int b = 0;

	for (unsigned int i = 0; i < m_PLightsSelected.size(); ++i)
	{
		if (m_PLightsSelected[i] == true)
			++b;
	}

	for (unsigned int i = 0; i < m_SLightsSelected.size(); ++i)
	{
		if (m_SLightsSelected[i] == true)
			++b;
	}

	return b;
}

int VisualLightDebugger::GetTotalLightsInScene()
{
	int s = 0;

	s += m_PLightsSelected.size();
	s += m_SLightsSelected.size();

	return s;
}

void VisualLightDebugger::HideTextBoxes()
{
	m_pTextBoxX->Hide();
	m_pTextBoxY->Hide();
	m_pTextBoxZ->Hide();

	m_bTextBoxesSet = false;
}

//bool VisualLightDebugger::SpotLightSelected(SpotLight* sl)
//{
//	return false;
//}
//
//bool VisualLightDebugger::PointLightSelected(PointLight* pl)
//{
//	return false;
//}