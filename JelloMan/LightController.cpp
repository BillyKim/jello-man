#include "LightController.h"


LightController::LightController(void)
{
}


LightController::~LightController(void)
{
    //for (UINT i = 0; i < m_PointLights.size(); ++i)
    //    delete m_PointLights[i];
    //for (UINT i = 0; i < m_DirectionalLights.size(); ++i)
    //    delete m_DirectionalLights[i];
    //for (UINT i = 0; i < m_SpotLights.size(); ++i)
    //    delete m_DirectionalLights[i];
}

void LightController::AddLight(const PointLight& light)
{
    m_PointLights.push_back(light);

	// DEBUG
	m_pHitRectLights.push_back(new HitRegion(HitRegion::TYPE_ELLIPSE,0,0,20,20));
	m_LightsSelected.push_back(false);
}
void LightController::AddLight(const DirectionalLight& light)
{
    m_DirectionalLights.push_back(light);
}
void LightController::AddLight(const SpotLight& light)
{
    m_SpotLights.push_back(light);
}

vector<PointLight>& LightController::GetPointLights()
{
    return m_PointLights;
}
const vector<DirectionalLight>& LightController::GetDirectionalLights() const
{
    return m_DirectionalLights;
}
const vector<SpotLight>& LightController::GetSpotLights() const
{
    return m_SpotLights;
}

void LightController::VisualLightDebugger(const Camera* pCamera)
{
	// LIGHT DEBUG
	Matrix matProj = pCamera->GetProjection();
	Matrix matView = pCamera->GetView();
	
	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 1;
	viewP.MaxDepth = 10000;

	Matrix ident;
	D3DXMatrixIdentity(&ident);

	int size = 10;

	if (CONTROLS->LeftMBClicked())
	{
		for (unsigned int i = 0; i < m_pHitRectLights.size(); ++i)
		{
			if (CONTROLS->IsKeyDown(VK_LCONTROL))
			{
				if (m_pHitRectLights[i]->HitTest(CONTROLS->GetMousePos()))
				{
					m_LightsSelected[i] = !m_LightsSelected[i];
				}
			}
			else
			{
				if (m_pHitRectLights[i]->HitTest(CONTROLS->GetMousePos()))
				{
					m_LightsSelected[i] = !m_LightsSelected[i];
					
					for (unsigned int i2 = 0; i2 < m_pHitRectLights.size(); ++i2)
					{
						if (i != i2)
							m_LightsSelected[i2] = false;
					}
				}
			}
		}
	}

	if (CONTROLS->RightMBClicked())
	{
		for (unsigned int i = 0; i < m_pHitRectLights.size(); ++i)
			m_LightsSelected[i] = false;
	}

	// POINTLIGHTS
	for (unsigned int i = 0; i < m_PointLights.size(); ++i)
	{
		// VIEWPORT PROJECTION
		D3DXVECTOR3 temp;
		D3DXVECTOR3 pos = m_PointLights[i].position.ToD3DVector3();
		D3DXVec3Project(&temp,&pos,&viewP,&matProj,&matView,&ident);

		Vector3 length = pCamera->GetPosition() - pos;
		float l = length.Length();
		l *= 0.001f;

		ColorF col(m_PointLights[i].color.R,m_PointLights[i].color.G,m_PointLights[i].color.B,0.4f/l);

		// HITRECTS
		m_pHitRectLights[i]->SetSize(static_cast<int>(size/l),static_cast<int>(size/l));
		m_pHitRectLights[i]->SetPosition(static_cast<int>(temp.x),static_cast<int>(temp.y));

		// DRAW
		if (m_pHitRectLights[i]->HitTest(CONTROLS->GetMousePos()) || m_LightsSelected[i] == true)
		{
			BLOX_2D->SetColor(255,255,255,0.4f/l);
			BLOX_2D->FillEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l));
			BLOX_2D->SetColor(0,0,0,0.4f/l);
			BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l),3.0f/l);
		}
		else
		{
			BLOX_2D->SetColor(col);
			BLOX_2D->FillEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>((size/l)));
			BLOX_2D->SetColor(255,255,255,0.4f/l);
			BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l),2.0f);
		}

		BLOX_2D->SetColor(0,0,0,0.4f/l);
		BLOX_2D->SetFont(_T("Arial"),true,false,(size/2)/(l/2));
			
		BLOX_2D->DrawString(_T("P"),
			RectF(temp.x-((size/l)/2),temp.y-((size/l)/2),temp.x+(size/l)-((size/l)/2),temp.y+(size/l)-((size/l)/2)),
			Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);

		// MOVE
		Vector3 look = pCamera->GetLook();
		look.Normalize();
		Vector3 right = pCamera->GetRight();
		right.Normalize();

		if (m_LightsSelected[i] == true)
		{
			if (CONTROLS->IsKeyDown(VK_NUMPAD8))
			{
				m_PointLights[i].position.X += look.X*5;
				m_PointLights[i].position.Z += look.Z*5;
			}
			else if (CONTROLS->IsKeyDown(VK_NUMPAD2))
			{
				m_PointLights[i].position.X -= look.X*5;
				m_PointLights[i].position.Z -= look.Z*5;
			}
			if (CONTROLS->IsKeyDown(VK_NUMPAD6))
			{
				m_PointLights[i].position.X += right.X*5;
				m_PointLights[i].position.Z += right.Z*5;
			}
			else if (CONTROLS->IsKeyDown(VK_NUMPAD4))
			{
				m_PointLights[i].position.X -= right.X*5;
				m_PointLights[i].position.Z -= right.Z*5;
			}
			if (CONTROLS->IsKeyDown(VK_NUMPAD9))
				m_PointLights[i].position.Y += 2;
			else if (CONTROLS->IsKeyDown(VK_NUMPAD7))
				m_PointLights[i].position.Y -= 2;
			if (CONTROLS->IsKeyDown(VK_ADD))
				m_PointLights[i].multiplier += 0.1f;
			else if (CONTROLS->IsKeyDown(VK_SUBTRACT))
				m_PointLights[i].multiplier -= 0.1f;
			if (CONTROLS->IsKeyDown(VK_NUMPAD3))				
				m_PointLights[i].AttenuationEnd += 20;
			else if (CONTROLS->IsKeyDown(VK_NUMPAD1))
				if (m_PointLights[i].AttenuationEnd > 0)
				{
					m_PointLights[i].AttenuationEnd -= 20;
				}
		}
	}
}