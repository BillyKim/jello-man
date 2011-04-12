#pragma once
#include "LightBehaviour.h"
#include "Light.h"

enum BrokenLightType
{
    BrokenLightType_Fluorescent,
    BrokenLightType_LightBulb
};

class LightBehaviourBroken
{
public:
    LightBehaviourBroken(BrokenLightType type);
    virtual ~LightBehaviourBroken(void);
    
    virtual void Init(Light* light);

    virtual void Tick(float dTime);

private:
    Light* m_pLight;

    BrokenLightType m_Type;

    float m_Multiplier;
    float m_MultiplierNormal;

    bool m_IsOn;

    float m_WaitTime;
};

