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

//Serialize
void LightBehaviourRotator::Serialize(Serializer* pSerializer)
{
    pSerializer->GetStream()->storeFloat(m_Speed);
    pSerializer->GetStream()->storeVector3(m_Axis);
}
void LightBehaviourRotator::Deserialize(Serializer* pSerializer)
{
    m_Speed = pSerializer->GetStream()->readFloat();
    m_Axis = pSerializer->GetStream()->readVector3();
}