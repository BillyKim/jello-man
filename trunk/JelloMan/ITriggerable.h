#pragma once
#include "ITrigger.h"

class ITriggerable
{
public:
    virtual ~ITriggerable(void) {}

    virtual void OnTriggerEnter(const ITrigger* pTrigger) = 0;
    virtual void OnTriggerLeave(const ITrigger* pTrigger) = 0;
};

