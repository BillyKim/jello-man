#pragma once
#include "LightBehaviour.h"

class LightBehaviourNormal : public LightBehaviour
{
public:
    LightBehaviourNormal(void);
    virtual ~LightBehaviourNormal(void);
    
    virtual void Init(Light* light) {};

    virtual void Tick(float dTime) {}
};

