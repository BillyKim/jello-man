#pragma once
#include "Trigger.h"

class ITriggerable
{
public:
    virtual ~ITriggerable(void) {}

    virtual void OnTriggerEnter(const Trigger* pTrigger) = 0;
    virtual void OnTriggerLeave(const Trigger* pTrigger) = 0;
};

