#include "LightBehaviourRotator.h"
#include "Light.h"

LightBehaviourRotator::LightBehaviourRotator(const Vector3& axis, float speed): m_Axis(axis), m_Speed(speed)
{
}


LightBehaviourRotator::~LightBehaviourRotator(void)
{
    //nothing to destroy
}

void LightBehaviourRotator::Init(Light* light)
{
    m_pLight = light;
}

void LightBehaviourRotator::Tick(float dTime)
{
    m_pLight->Rotate(m_Axis, m_Speed * dTime);
}