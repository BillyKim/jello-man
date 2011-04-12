#include "PointLight.h"
#include "LightBehaviourNormal.h"

PointLight::PointLight(const PointLightDesc& desc): 
    m_pLightBehaviour(0),
    m_IsEnabled(true),
    m_IsSelected(false),
    m_StartDesc(desc),
    m_Desc(desc)
{
    SetBehaviour(new LightBehaviourNormal());
}
PointLight::~PointLight()
{
    delete m_pLightBehaviour;
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