#include "LightBehaviourBroken.h"


LightBehaviourBroken::LightBehaviourBroken(BrokenLightType type): 
        m_IsOn(true), 
        m_Multiplier(0), 
        m_MultiplierNormal(0),
        m_pLight(0),
        m_Type(type),
        m_WaitTime(0)
{
}


LightBehaviourBroken::~LightBehaviourBroken(void)
{
}

void LightBehaviourBroken::Init(Light* light)
{
    m_pLight = light;
    m_Multiplier = m_pLight->GetMulitplier();
    m_MultiplierNormal = m_pLight->GetMulitplier();
}

void LightBehaviourBroken::Tick(float dTime)
{
    m_WaitTime -= dTime;
    if (m_WaitTime <= 0)
    {
        if (m_Type == BrokenLightType_LightBulb)
        {
            if (m_IsOn)
            {
                m_Multiplier = rand() % 1000 / 1000.0f * m_MultiplierNormal / 2.0f;
                m_WaitTime = rand() % 1000 / 1000.0f * 0.1f;
                m_IsOn = false;
            }
            else
            {
                m_Multiplier = rand() % 1000 / 1000.0f * m_MultiplierNormal + m_MultiplierNormal / 2.0f;
                m_WaitTime = rand() % 1000 / 1000.0f * 0.5f;
                m_IsOn = true;
            }
        }
        else
        {
            if (rand() % 25 == 0)
                m_IsOn = true;
            if (m_IsOn)
            {
                m_IsOn = false;
                m_Multiplier = rand() % 1000 / 1000.0f * m_MultiplierNormal + m_MultiplierNormal;
                m_WaitTime = rand() % 1000 / 1000.0f * 1.0f;
            }
            else
            {
                m_Multiplier = m_Multiplier + rand() % 1000 / 1000.0f * m_MultiplierNormal / 5.0f - m_MultiplierNormal / 2.5f;
                m_WaitTime = rand() % 1000 / 1000.0f * 0.1f; //light flicker
            }
        }
        m_pLight->SetMulitplier(m_Multiplier);
    }
}

//Serialize
void LightBehaviourBroken::Serialize(Serializer* pSerializer)
{
    pSerializer->GetStream()->storeByte(static_cast<BYTE>(m_Type));
}
void LightBehaviourBroken::Deserialize(Serializer* pSerializer)
{
    m_Type = static_cast<BrokenLightType>(pSerializer->GetStream()->readByte());
}