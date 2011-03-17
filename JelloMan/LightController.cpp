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
}
void LightController::AddLight(const DirectionalLight& light)
{
    m_DirectionalLights.push_back(light);
}
void LightController::AddLight(const SpotLight& light)
{
    m_SpotLights.push_back(light);
}

const vector<PointLight>& LightController::GetPointLights() const
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

	// POINTLIGHTS
	for (unsigned int i = 0; i < m_PointLights.size(); ++i)
	{
		D3DXVECTOR3 temp;
		D3DXVECTOR3 pos = m_PointLights[i].position.ToD3DVector3();
		D3DXVec3Project(&temp,&pos,&viewP,&matProj,&matView,&ident);

		Vector3 length = pCamera->GetPosition() - pos;
		float l = length.Length();
		l *= 0.001f;

		BLOX_2D->SetColor(255,255,0,0.4f/l);
		BLOX_2D->FillEllipse((int)temp.x,(int)temp.y,(int)(20/l),(int)(20/l));
		BLOX_2D->SetColor(255,255,255,0.4f/l);
		BLOX_2D->DrawEllipse((int)temp.x,(int)temp.y,(int)(20/l),(int)(20/l),2.0f);
		BLOX_2D->SetColor(0,0,0,0.4f/l);
		BLOX_2D->SetFont(_T("Arial"),true,false,10/(l/2));
			
		BLOX_2D->DrawString(_T("P"),
			RectF(temp.x-((20/l)/2),temp.y-((20/l)/2),temp.x+(20/l)-((20/l)/2),temp.y+(20/l)-((20/l)/2)),
			Blox2D::HORIZONTAL_ALIGN_CENTER,Blox2D::VERTICAL_ALIGN_MIDDLE);
	}
}