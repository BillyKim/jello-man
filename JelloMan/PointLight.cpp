#include "PointLight.h"
#include "LightBehaviourNormal.h"
#include "Blox2D_Engine.h"
#include "ContentManager.h"
#include "ModelMesh.h"

PointLight::PointLight(const PointLightDesc& desc): 
    m_pLightBehaviour(0),
    m_IsEnabled(true),
    m_IsSelected(false),
    m_StartDesc(desc),
    m_Desc(desc),
	m_pHitRegion(0),
	m_pPointLightImage(0),
	m_pAttenuationSpline(0),
	m_pEffect(0)
{
    SetBehaviour(new LightBehaviourNormal());

	m_pPointLightImage = Content->LoadImage(_T("Content/Images/Editor/plight.png"));

	m_pAttenuationSpline = Content->LoadSpline(_T("Content/Models/orb_path.obj"), Color(255.0f,255.0f,255.0f,1.0f));
	m_pEffect = Content->LoadEffect<PosColEffect>(_T("poscol.fx"));
}
PointLight::~PointLight()
{
    delete m_pLightBehaviour;
	delete m_pHitRegion;
}

void PointLight::InitGame()
{
    m_StartDesc = m_Desc;
}
void PointLight::InitEditor()
{
    m_Desc = m_StartDesc;
}

void PointLight::Tick(float dTime)
{
    m_pLightBehaviour->Tick(dTime);
}
void PointLight::Draw(const RenderContext* rc)
{
	D3DXMATRIX matProj = rc->GetCamera()->GetProjection();
	D3DXMATRIX matView = rc->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	Vector3 vLook = rc->GetCamera()->GetLook();
	Vector3 vRight = rc->GetCamera()->GetRight();

	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BX2D->GetWindowSize().width;
	viewP.Height = (UINT)BX2D->GetWindowSize().height;
	viewP.MinDepth = 0.0f;
	viewP.MaxDepth = 1.0f;

	int size = 10;
	
	// VIEWPORT PROJECTION
	D3DXVECTOR3 pos = D3DXVECTOR3(m_Desc.position.X, m_Desc.position.Y, m_Desc.position.Z);
	
	Vector3 length = rc->GetCamera()->GetPosition() - Vector3(pos);
	float l = length.Length();
	l *= 0.001f;

	D3DXVECTOR3 pos2D;
	D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

	// HITRECT
	SafeDelete(m_pHitRegion);

	m_pHitRegion = new HitRegion(	
		HitRegion::TYPE_ELLIPSE,
		pos2D.x,
		pos2D.y, 
		size / l,
		size / l);

	if (vLook.Dot(length) < 0)
	{
		// DRAW
		if (m_pHitRegion->HitTest(CONTROLS->GetMousePos()) || m_IsSelected)
		{
			BX2D->SetColor(255,255,255);
			BX2D->FillEllipse(pos2D.x, pos2D.y, size / l, size / l);
			BX2D->SetColor(30,30,30);
			BX2D->DrawEllipse(pos2D.x, pos2D.y, size / l, size / l, 2.0f / l);
		}
		else
		{
			BX2D->SetColor(static_cast<int>(m_Desc.color.R * 255), static_cast<int>(m_Desc.color.G * 255), static_cast<int>(m_Desc.color.B * 255));
			BX2D->FillEllipse(pos2D.x, pos2D.y, size / l, size / l);
			BX2D->SetColor(230,230,230);
			BX2D->DrawEllipse(pos2D.x, pos2D.y, size / l, size / l, 2.0f / l);
		}

		BX2D->DrawBitmap(
			m_pPointLightImage,
			pos2D.x - (m_pPointLightImage->GetDimensions().width / (16 * l)),
			pos2D.y - (m_pPointLightImage->GetDimensions().height / (16 * l)),
			false,
			1.0f,
			m_pPointLightImage->GetDimensions().width / (8 * l),
			m_pPointLightImage->GetDimensions().height / (8 * l));
	}

	if (m_IsSelected)
	{
		// ATTENUATION
		Matrix matWorld =
			Matrix::CreateScale(
			Vector3(m_Desc.attenuationEnd / 100.0f,m_Desc.attenuationEnd / 100.0f,m_Desc.attenuationEnd / 100.0f)) * 
			Matrix::CreateTranslation(
			Vector3(m_Desc.position.X, m_Desc.position.Y, m_Desc.position.Z));

		m_pEffect->SetWorld(matWorld);
		m_pEffect->SetWorldViewProjection(matWorld * rc->GetCamera()->GetViewProjection());
		m_pAttenuationSpline->Draw(m_pEffect);
	}
}

void PointLight::Translate(const Vector3& add)
{
	m_Desc.position += add;
}
void PointLight::SetPosition(const Vector3& pos)
{
	m_Desc.position = pos;
}
const Vector3& PointLight::GetPosition() const
{
    return m_Desc.position;
}

void PointLight::Scale(const Vector3& scale)
{
	m_Desc.attenuationEnd *= (scale.X + scale.Y + scale.Z) / 3;
}

void PointLight::Enable()
{
	m_IsEnabled = true;
}
void PointLight::Disable()
{
	m_IsEnabled = false;
}
bool PointLight::IsEnabled() const
{
	return m_IsEnabled;
}

void PointLight::Select()
{
	m_IsSelected = true;
}
void PointLight::Deselect()
{
	m_IsSelected = false;
}
bool PointLight::IsSelected() const
{
	return m_IsSelected;
}

void PointLight::SetColor(const Color& color)
{
    m_Desc.color = color;
}
const Color& PointLight::GetColor() const
{
    return m_Desc.color;
}

void PointLight::SetMulitplier(float mult)
{
    m_Desc.multiplier = mult;
}
float PointLight::GetMulitplier() const
{
    return m_Desc.multiplier;
}

void PointLight::SetAttenuationStart(float start)
{
    m_Desc.attenuationStart = start;
}    
float PointLight::GetAttenuationStart() const
{
    return m_Desc.attenuationStart;
}

void PointLight::SetAttenuationEnd(float end)
{
    m_Desc.attenuationEnd = end;
}
float PointLight::GetAttenuationEnd() const
{
    return m_Desc.attenuationEnd;
}

const PointLightDesc& PointLight::GetDesc() const
{
    return m_Desc;
}

void PointLight::SetBehaviour(LightBehaviour* lightBehaviour)
{
    delete m_pLightBehaviour;
    m_pLightBehaviour = lightBehaviour;
    m_pLightBehaviour->Init(this);
}

D3D10_RECT PointLight::CalcScissorRect(Camera* camera, UINT backbufferWidth, UINT backbufferHeight) const
{
    Vector4 pos;
    pos = Vector3::Transform(m_Desc.position, camera->GetViewProjection());

    float len = abs(pos.W);
	len *= 0.001f;
	float size = m_Desc.attenuationEnd / len * 1.1f;//len.Length();

	pos.X /= pos.W;
	pos.Y /= pos.W;

	pos.X += 1.f; pos.X /= 2;
	pos.Y += 1.f; pos.Y /= 2; pos.Y = 1 - pos.Y;

	D3D10_RECT r;
	r.left = static_cast<LONG>(pos.X * backbufferWidth - size);
	r.right =  static_cast<LONG>(r.left + size * 2);
	r.top = static_cast<LONG>(pos.Y * backbufferHeight - size);
	r.bottom = static_cast<LONG>(r.top + size * 2);

    return r;
}
//D3D10_RECT PointLight::CalcScissorRect(const Matrix& worldViewProj, UINT backbufferWidth, UINT backbufferHeight)
//{
//    Vector4 pos;
//    pos = Vector3::Transform(m_Desc.position, worldViewProj);
//
//    float len = abs(pos.W);
//	len *= 0.001f;
//	float size = length / len * 1.1f;//len.Length();
//
//	pos.X /= pos.W;
//	pos.Y /= pos.W;
//
//	pos.X += 1.f; pos.X /= 2;
//	pos.Y += 1.f; pos.Y /= 2; pos.Y = 1 - pos.Y;
//
//	D3D10_RECT r;
//	r.left = static_cast<LONG>(pos.X * backbufferWidth - size);
//	r.right =  static_cast<LONG>(r.left + size * 2);
//	r.top = static_cast<LONG>(pos.Y * backbufferHeight - size);
//	r.bottom = static_cast<LONG>(r.top + size * 2);
//
//    return r;
//}