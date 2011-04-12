#include "SpotLight.h"
#include "LightBehaviourNormal.h"

SpotLight::SpotLight(const SpotLightDesc& desc):
			m_Scale(1), 
            m_Rotation(Matrix::Identity), 
			m_Desc(desc),
            m_StartDesc(desc),
			m_IsEnabled(true),
			m_IsSelected(false),
			m_ShadowMap(0),
            m_pLightBehaviour(0)
{
    SetBehaviour(new LightBehaviourNormal());
}
SpotLight::~SpotLight()
{
	delete m_ShadowMap;
    delete m_pLightBehaviour;
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
}
void SpotLight::SetPosition(const Vector3& pos)
{
	m_Desc.position = pos;
}
const Vector3& SpotLight::GetPosition() const
{
    return m_Desc.position;
}

void SpotLight::Rotate(const Vector3& axis, float angle)
{
    m_Desc.direction = Vector3::Transform(m_Desc.direction, Matrix::CreateRotation(axis, angle)).XYZ();
}

void SpotLight::Scale(const Vector3& scale)
{
	m_Desc.attenuationEnd *= (scale.X + scale.Y + scale.Z) / 3;
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
void SpotLight::SetShadowMap(ShadowMapType type)
{
    delete m_ShadowMap;
    m_ShadowMap = 0;

	if (type != ShadowMapType_None)
	{
		m_ShadowMap = new Texture2D(0, type, type, false);
	}
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
}    
float SpotLight::GetAttenuationStart() const
{
    return m_Desc.attenuationStart;
}

void SpotLight::SetAttenuationEnd(float end)
{
    m_Desc.attenuationEnd = end;
}
float SpotLight::GetAttenuationEnd() const
{
    return m_Desc.attenuationEnd;
}

void SpotLight::SetPower(float power)
{
	m_Desc.power = power;
}
void SpotLight::AddPower(float add)
{
	m_Desc.power += add;
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