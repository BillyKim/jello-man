#include "SpotLight.h"
#include "LightBehaviourNormal.h"

SpotLight::SpotLight():
			m_Scale(1), 
            m_Rotation(Matrix::Identity),
			m_IsEnabled(true),
			m_IsSelected(false),
			m_ShadowMap(0),
            m_pLightBehaviour(0),
            m_pShadowCamera(0),
            m_OpeningsAngle(0.0f),
            m_vUp(0, 1, 0)
{
    SetBehaviour(new LightBehaviourNormal());

    m_Desc.attenuationStart = 0;
    m_Desc.attenuationEnd = 500;
    m_Desc.color = Color(1.0f, 1.0f, 1.0f);
    m_Desc.direction = Vector3(1, 0, 0);
    m_Desc.multiplier = 1.0f;
    m_Desc.position = Vector3(0, 0, 0);
    m_Desc.power = 1.0f;
    m_StartDesc = m_Desc;
}
SpotLight::~SpotLight()
{
	delete m_ShadowMap;
    delete m_pLightBehaviour;
    delete m_pShadowCamera;
}

void SpotLight::InitGame()
{
    m_StartDesc = m_Desc;
}
void SpotLight::InitEditor()
{
    m_Desc = m_StartDesc;
}

void SpotLight::Tick(float dTime)
{
    m_pLightBehaviour->Tick(dTime);
}
void SpotLight::Draw(const RenderContext* rc)
{
}

void SpotLight::Translate(const Vector3& add)
{
	m_Desc.position += add;
    UpdateShadowCameraView();
}
void SpotLight::SetPosition(const Vector3& pos)
{
	m_Desc.position = pos;
    UpdateShadowCameraView();
}
const Vector3& SpotLight::GetPosition() const
{
    return m_Desc.position;
}

void SpotLight::Rotate(const Vector3& axis, float angle)
{
    Matrix rot(Matrix::CreateRotation(axis, angle));
    m_Desc.direction = Vector3::Transform(m_Desc.direction, rot).XYZ();
    m_vUp = Vector3::Transform(m_vUp, rot).XYZ();
    UpdateShadowCameraView();
}

void SpotLight::Scale(const Vector3& scale)
{
	m_Desc.attenuationEnd *= (scale.X + scale.Y + scale.Z) / 3;
    UpdateShadowCameraProjection();
}

void SpotLight::Enable()
{
	m_IsEnabled = true;
}
void SpotLight::Disable()
{
	m_IsEnabled = false;
}
bool SpotLight::IsEnabled() const
{
	return m_IsEnabled;
}

void SpotLight::Select()
{
	m_IsSelected = true;
}
void SpotLight::Deselect()
{
	m_IsSelected = false;
}
bool SpotLight::IsSelected() const
{
	return m_IsSelected;
}
bool SpotLight::HasShadowMap() const
{
	return m_ShadowMap != 0;
}
Texture2D* SpotLight::GetShadowMap() const
{
	return m_ShadowMap;
}
Camera* SpotLight::GetShadowCamera() const
{
    return m_pShadowCamera;
}
void SpotLight::SetShadowMap(ID3D10Device* pDXDevice, ShadowMapType type)
{
    delete m_ShadowMap;
    m_ShadowMap = 0;
    delete m_pShadowCamera;
    m_pShadowCamera = 0;

	if (type != ShadowMapType_None)
	{
		m_ShadowMap = new Texture2D(pDXDevice, type, type, false);
        m_pShadowCamera = new Camera(type, type, false);
        m_pShadowCamera->SetPosition(m_Desc.position);    

        UpdateShadowCameraProjection();
        UpdateShadowCameraView();
	}
}

void SpotLight::UpdateShadowCameraView()
{
    if (m_pShadowCamera != 0)
        m_pShadowCamera->LookAt(m_Desc.position, m_Desc.position + m_Desc.direction, m_vUp); 
}
void SpotLight::UpdateShadowCameraProjection()
{
    if (m_pShadowCamera != 0)
        m_pShadowCamera->SetLens(1.0f, m_OpeningsAngle, 1.0f, m_Desc.attenuationEnd);
}

D3D10_RECT SpotLight::CalcScissorRect(Camera* camera, UINT backbufferWidth, UINT backbufferHeight) const
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

void SpotLight::SetColor(const Color& color)
{
    m_Desc.color = color;
}
const Color& SpotLight::GetColor() const
{
    return m_Desc.color;
}

void SpotLight::SetMulitplier(float mult)
{
    m_Desc.multiplier = mult;
}
float SpotLight::GetMulitplier() const
{
    return m_Desc.multiplier;
}

void SpotLight::SetAttenuationStart(float start)
{
    m_Desc.attenuationStart = start;
    UpdateShadowCameraProjection();
}    
float SpotLight::GetAttenuationStart() const
{
    return m_Desc.attenuationStart;
}

void SpotLight::SetAttenuationEnd(float end)
{
    m_Desc.attenuationEnd = end;
    UpdateShadowCameraProjection();
}
float SpotLight::GetAttenuationEnd() const
{
    return m_Desc.attenuationEnd;
}

void SpotLight::SetOpeningsAngle(float rad)
{
    m_OpeningsAngle = rad;

    m_Desc.power = -(log(100.0f) / (log(cos(m_OpeningsAngle / 2.0f)))); //use only a half of the angle

    UpdateShadowCameraProjection();
}
void SpotLight::AddOpeningsAngle(float rad)
{
	SetOpeningsAngle(-m_OpeningsAngle + rad);
}

const SpotLightDesc& SpotLight::GetDesc() const
{
    return m_Desc;
}

void SpotLight::SetBehaviour(LightBehaviour* lightBehaviour)
{
    delete m_pLightBehaviour;
    m_pLightBehaviour = lightBehaviour;
    m_pLightBehaviour->Init(this);
}