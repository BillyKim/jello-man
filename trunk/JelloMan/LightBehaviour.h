#pragma once
#include "RenderContext.h"

class Light;

class LightBehaviour
{
public:
    virtual ~LightBehaviour(void) {}

    virtual void Init(Light* light) = 0;
    
	virtual void Tick(float dTime) = 0;

protected:
    LightBehaviour(void) {}
};

