#pragma once
#include "LightBehaviour.h"

class LightBehaviourRotator : public LightBehaviour
{
public:
    LightBehaviourRotator(const Vector3& axis, float speed);
    virtual ~LightBehaviourRotator(void);
    
    virtual void Init(Light* light);

    virtual void Tick(float dTime);

private:
    Light* m_pLight;

    Vector3 m_Axis;
    float m_Speed;
};

