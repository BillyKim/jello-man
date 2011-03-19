#include "LightController.h"


LightController::LightController(void)	:	m_bLockX(false),
											m_bLockY(false),
											m_bLockZ(false),
											m_bMoveable(true),
											m_bClick(false)
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
	m_MPos.push_back(D3DXVECTOR3(0.0f,0.0f,0.0f));
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
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	Matrix ident;
	D3DXMatrixIdentity(&ident);

	int size = 10;

	BLOX_2D->SetColor(0,0,0,0.5f);
	BLOX_2D->FillRect(0,0,220,BLOX_2D->GetWindowSize().height);

	BLOX_2D->SetColor(255,255,255);
	// DEBUG INFO
	BLOX_2D->SetFont(_T("Arial"),false,false,12);
	tstringstream stream;
	stream << _T("::VISUAL LIGHT DEBUGGER::\n---------------------------------\n");
	stream << _T("Nr. lights in scene: ") << GetPointLights().size() << _T("\n");
	
	int b = 0;
	for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
	{
		if (m_LightsSelected[i] == true)
			++b;
	}

	stream << _T("Nr. lights selected: ") << b << _T("\n\n");

	for (unsigned int i = 0; i < m_LightsSelected.size(); ++i)
	{
		if (m_LightsSelected[i] == true)
		{
			stream << _T("PointLight ") << i << _T(":\n   ");
			stream << _T("X:") << m_PointLights[i].position.X << _T(" ");
			stream << _T("Y:") << m_PointLights[i].position.Y << _T(" ");
			stream << _T("Z:") << m_PointLights[i].position.Z << _T("\n   ");
			stream << _T("R:") << static_cast<int>(m_PointLights[i].color.R*255) << _T(" ");
			stream << _T("G:") << static_cast<int>(m_PointLights[i].color.G*255) << _T(" ");
			stream << _T("B:") << static_cast<int>(m_PointLights[i].color.B*255) << _T("\n   ");
			stream << _T("Multiplier: ") << m_PointLights[i].multiplier << _T("\n   ");
			stream << _T("Attenuation end: ") << m_PointLights[i].AttenuationEnd << _T("\n\n");
		}
	}

	BLOX_2D->DrawString(stream.str(),2,230);

	if (CONTROLS->LeftMBDown())
	{
		if (!m_bClick)
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

		m_bClick = true;

		}
	}
	else
		m_bClick = false;

	/*if (CONTROLS->RightMBClicked())
	{
		for (unsigned int i = 0; i < m_pHitRectLights.size(); ++i)
			m_LightsSelected[i] = false;
	}*/

	// POINTLIGHTS
	for (unsigned int i = 0; i < m_PointLights.size(); ++i)
	{
		D3DXVECTOR3 pos = m_PointLights[i].position.ToD3DVector3();

		Vector3 length = pCamera->GetPosition() - pos;
		float l = length.Length();
		l *= 0.001f;

		// VIEWPORT PROJECTION
		D3DXVECTOR3 temp;
		D3DXVECTOR3 temp2;
		D3DXVECTOR3 temp3;
		D3DXVECTOR3 temp4;

		D3DXVECTOR3 temp5;
		D3DXVECTOR3 temp6;
		D3DXVECTOR3 temp7;

		D3DXVECTOR3 temp8;
		D3DXVECTOR3 temp9;
		D3DXVECTOR3 temp10;
	
		D3DXVECTOR3 posLineX = pos;
		posLineX.x += l*100;

		D3DXVECTOR3 posLineY = pos;
		posLineY.y += l*100;

		D3DXVECTOR3 posLineZ = pos;
		posLineZ.z += l*100;

		D3DXVECTOR3 posLineX2 = pos;
		posLineX2.x += l*50;

		D3DXVECTOR3 posLineY2 = pos;
		posLineY2.y += l*50;

		D3DXVECTOR3 posLineZ2 = pos;
		posLineZ2.z += l*50;

		D3DXVECTOR3 posLineXY = pos;
		posLineXY.x += l*50;
		posLineXY.y += l*50;

		D3DXVECTOR3 posLineYZ = pos;
		posLineYZ.y += l*50;
		posLineYZ.z += l*50;

		D3DXVECTOR3 posLineZX = pos;
		posLineZX.z += l*50;
		posLineZX.x += l*50;

		D3DXVec3Project(&temp,&pos,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp2,&posLineX,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp3,&posLineY,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp4,&posLineZ,&viewP,&matProj,&matView,&ident);

		D3DXVec3Project(&temp5,&posLineX2,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp6,&posLineY2,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp7,&posLineZ2,&viewP,&matProj,&matView,&ident);

		D3DXVec3Project(&temp8,&posLineXY,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp9,&posLineYZ,&viewP,&matProj,&matView,&ident);
		D3DXVec3Project(&temp10,&posLineZX,&viewP,&matProj,&matView,&ident);

		ColorF col(m_PointLights[i].color.R,m_PointLights[i].color.G,m_PointLights[i].color.B,0.4f/l);

		Vector3 lo = pCamera->GetLook();

		if (lo.Dot(length) < 0)
		{
			// HITRECTS
			m_pHitRectLights[i]->SetSize(static_cast<int>(size/l),static_cast<int>(size/l));
			m_pHitRectLights[i]->SetPosition(static_cast<int>(temp.x),static_cast<int>(temp.y));

			BLOX_2D->SetColor(0,0,0,0.4f/l);
			BLOX_2D->SetFont(_T("Arial"),true,false,(size/2)/(l/2));

			// DRAW
			if (m_pHitRectLights[i]->HitTest(CONTROLS->GetMousePos()) || m_LightsSelected[i] == true)
			{
				BLOX_2D->SetColor(255,255,255,0.4f/l);
				BLOX_2D->FillEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l));
				BLOX_2D->SetColor(0,0,0,0.4f/l);
				BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l),3.0f/l);

				BLOX_2D->DrawString(_T("P"),
				RectF(temp.x-((size/l)/2),temp.y-((size/l)/2),temp.x+(size/l)-((size/l)/2),temp.y+(size/l)-((size/l)/2)),
				Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);

				BLOX_2D->SetColor(255,255,255,0.4f/l);
				BLOX_2D->SetFont(_T("Arial"),true,false,(size/4)/(l/2));

				tstringstream s;
				s << _T("PointLight ") << i;
				BLOX_2D->DrawString(s.str(),
				RectF(temp.x-(((size-40)/l)/2),temp.y-(((size+40)/l)/2),temp.x+(size/l)-(((size+40)/l)/2),temp.y+(size/l)-(((size+40)/l)/2)),
				Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);

				BLOX_2D->SetColor(255,255,255);
				BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(m_PointLights[i].AttenuationEnd/l),static_cast<int>(m_PointLights[i].AttenuationEnd/l),2);
			
				if (m_bMoveable)
				{
					// X
					HitRegion* hitX = new HitRegion(HitRegion::TYPE_ELLIPSE,temp2.x,temp2.y,size,size);
					BLOX_2D->SetColor(255,0,0);
					BLOX_2D->DrawLine(temp5.x,temp5.y,temp2.x,temp2.y,2.0f);
					BLOX_2D->FillEllipse(static_cast<int>(temp2.x),static_cast<int>(temp2.y),static_cast<int>(size/2),static_cast<int>(size/2));
					BLOX_2D->SetColor(255,0,0);
					BLOX_2D->SetFont(_T("Verdana"),false,false,14);
					BLOX_2D->DrawString(_T("X"),static_cast<int>(temp2.x-5),static_cast<int>(temp2.y) - 25);

					BLOX_2D->SetColor(50,50,255,0.4f);
					D2D1_POINT_2F r[4];
					r[0].x = temp8.x;
					r[0].y = temp8.y;
					r[1].x = temp6.x;
					r[1].y = temp6.y;
					r[2].x = temp.x;
					r[2].y = temp.y;
					r[3].x = temp5.x;
					r[3].y = temp5.y;

					BLOX_2D->FillPolygon(r,4);

					BLOX_2D->SetColor(100,100,255,0.8f);
					BLOX_2D->DrawLine(r[0],r[1]);
					BLOX_2D->DrawLine(r[1],r[2]);
					BLOX_2D->DrawLine(r[2],r[3]);
					BLOX_2D->DrawLine(r[3],r[0]);

					if (hitX->HitTest(CONTROLS->GetMousePos()))
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp2.x),static_cast<int>(temp2.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						if (CONTROLS->LeftMBDown() && !m_bLockY && !m_bLockZ)
						{
							m_bLockX = true;
						}
					}

					if (!CONTROLS->LeftMBDown())
					{
						m_bLockX = false;
						m_bLockY = false;
						m_bLockZ = false;
					}

					D3DXVECTOR3 testt(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,temp2.z);
					D3DXVECTOR3 ttempp;
					D3DXVec3Unproject(&ttempp,&testt,&viewP,&matProj,&matView,&ident);
					ttempp.x -= l*100;
					D3DXVECTOR3 ttempp2;
					D3DXVec3Project(&ttempp2,&ttempp,&viewP,&matProj,&matView,&ident);
					D3DXVECTOR3 ttempp3;
					D3DXVec3Unproject(&ttempp3,&ttempp2,&viewP,&matProj,&matView,&ident);

					if (m_bLockX)
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp2.x),static_cast<int>(temp2.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						float difX = m_MPos[i].x - ttempp3.x;
						m_PointLights[i].position.X -= difX;

						m_MPos[i].x = ttempp3.x;
					}
					else
					{
						m_MPos[i].x = ttempp3.x;
					}
				}

				if (m_bMoveable)
				{
					// Y
					HitRegion* hitY = new HitRegion(HitRegion::TYPE_ELLIPSE,temp3.x,temp3.y,size,size);
					BLOX_2D->SetColor(0,255,0);
					BLOX_2D->DrawLine(temp6.x,temp6.y,temp3.x,temp3.y,2.0f);
					BLOX_2D->FillEllipse(static_cast<int>(temp3.x),static_cast<int>(temp3.y),static_cast<int>(size/2),static_cast<int>(size/2));
					BLOX_2D->SetColor(0,255,0);
					BLOX_2D->SetFont(_T("Verdana"),false,false,14);
					BLOX_2D->DrawString(_T("Y"),static_cast<int>(temp3.x-5),static_cast<int>(temp3.y) - 25);
					BLOX_2D->SetColor(50,50,255,0.4f);
					D2D1_POINT_2F r[4];
					r[0].x = temp6.x;
					r[0].y = temp6.y;
					r[1].x = temp9.x;
					r[1].y = temp9.y;
					r[2].x = temp7.x;
					r[2].y = temp7.y;
					r[3].x = temp.x;
					r[3].y = temp.y;

					BLOX_2D->FillPolygon(r,4);

					BLOX_2D->SetColor(100,100,255,0.8f);
					BLOX_2D->DrawLine(r[0],r[1]);
					BLOX_2D->DrawLine(r[1],r[2]);
					BLOX_2D->DrawLine(r[2],r[3]);
					BLOX_2D->DrawLine(r[3],r[0]);

					if (hitY->HitTest(CONTROLS->GetMousePos()))
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp3.x),static_cast<int>(temp3.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						if (CONTROLS->LeftMBDown() && !m_bLockX && !m_bLockZ)
						{
							m_bLockY = true;
						}
					}

					D3DXVECTOR3 testt(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,temp3.z);
					D3DXVECTOR3 ttempp;
					D3DXVec3Unproject(&ttempp,&testt,&viewP,&matProj,&matView,&ident);
					ttempp.y -= l*100;
					D3DXVECTOR3 ttempp2;
					D3DXVec3Project(&ttempp2,&ttempp,&viewP,&matProj,&matView,&ident);
					D3DXVECTOR3 ttempp3;
					D3DXVec3Unproject(&ttempp3,&ttempp2,&viewP,&matProj,&matView,&ident);

					if (m_bLockY)
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp3.x),static_cast<int>(temp3.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);
					
						float difY = m_MPos[i].y - ttempp3.y;
						m_PointLights[i].position.Y -= difY;

						m_MPos[i].y = ttempp3.y;
					}
					else
					{
						m_MPos[i].y = ttempp3.y;
					}
				}
				if (m_bMoveable)
				{
					// Z
					HitRegion* hitZ = new HitRegion(HitRegion::TYPE_ELLIPSE,temp4.x,temp4.y,size,size);
					BLOX_2D->SetColor(255,255,0);
					BLOX_2D->DrawLine(temp7.x,temp7.y,temp4.x,temp4.y,2.0f);
					BLOX_2D->FillEllipse(static_cast<int>(temp4.x),static_cast<int>(temp4.y),static_cast<int>(size/2),static_cast<int>(size/2));
					BLOX_2D->SetColor(255,255,0);
					BLOX_2D->SetFont(_T("Verdana"),false,false,14);
					BLOX_2D->DrawString(_T("Z"),static_cast<int>(temp4.x-5),static_cast<int>(temp4.y) - 25);
					BLOX_2D->SetColor(50,50,255,0.4f);
					D2D1_POINT_2F r[4];
					r[0].x = temp10.x;
					r[0].y = temp10.y;
					r[1].x = temp5.x;
					r[1].y = temp5.y;
					r[2].x = temp.x;
					r[2].y = temp.y;
					r[3].x = temp7.x;
					r[3].y = temp7.y;

					BLOX_2D->FillPolygon(r,4);

					BLOX_2D->SetColor(100,100,255,0.8f);
					BLOX_2D->DrawLine(r[0],r[1]);
					BLOX_2D->DrawLine(r[1],r[2]);
					BLOX_2D->DrawLine(r[2],r[3]);
					BLOX_2D->DrawLine(r[3],r[0]);

					if (hitZ->HitTest(CONTROLS->GetMousePos()))
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp4.x),static_cast<int>(temp4.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						if (CONTROLS->LeftMBDown() && !m_bLockX && !m_bLockY)
						{
							m_bLockZ = true;
						}
					}

					D3DXVECTOR3 testt(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,temp4.z);
					D3DXVECTOR3 ttempp;
					D3DXVec3Unproject(&ttempp,&testt,&viewP,&matProj,&matView,&ident);
					ttempp.z -= l*100;
					D3DXVECTOR3 ttempp2;
					D3DXVec3Project(&ttempp2,&ttempp,&viewP,&matProj,&matView,&ident);
					D3DXVECTOR3 ttempp3;
					D3DXVec3Unproject(&ttempp3,&ttempp2,&viewP,&matProj,&matView,&ident);

					if (m_bLockZ)
					{
						BLOX_2D->SetColor(255,255,255);
						BLOX_2D->DrawEllipse(static_cast<int>(temp4.x),static_cast<int>(temp4.y),static_cast<int>(size/2),static_cast<int>(size/2),2.0f);

						float difZ = m_MPos[i].z - ttempp3.z;
						m_PointLights[i].position.Z -= difZ;

						m_MPos[i].z = ttempp3.z;
					}
					else
					{
						m_MPos[i].z = ttempp3.z;
					}
				}
			}
			else
			{
				BLOX_2D->SetColor(col);
				BLOX_2D->FillEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>((size/l)));
				BLOX_2D->SetColor(255,255,255,0.4f/l);
				BLOX_2D->DrawEllipse(static_cast<int>(temp.x),static_cast<int>(temp.y),static_cast<int>(size/l),static_cast<int>(size/l),2.0f);

				BLOX_2D->DrawString(_T("P"),
				RectF(temp.x-((size/l)/2),temp.y-((size/l)/2),temp.x+(size/l)-((size/l)/2),temp.y+(size/l)-((size/l)/2)),
				Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);
			}

		
		}
		else
		{
			// HITRECTS
			m_pHitRectLights[i]->SetSize(1,1);
			m_pHitRectLights[i]->SetPosition(-10,-10);
		}

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