#include "Light.h"


void Light::SetPosition(const Vector3& position)
{
    m_Position = position;
}
void Light::SetColor(const Vector4& color)
{
    m_Color = color;
}
void Light::SetMultiplier(float mult)
{
    m_Multiplier = mult;
}

void Light::SetAttenuationStart(float startAtt)
{
    m_AttenuationStart = startAtt;
}
void Light::SetAttenuationEnd(float endAtt)
{
    m_AttenuationEnd = endAtt;
}

void Light::Enable()
{
    m_Enabled = true;
}
void Light::Disable()
{
    m_Enabled = false;
}


void DirectionalLight::SetDirection(const Vector3& dir)
{
    m_Direction = dir;
}