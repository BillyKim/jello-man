#pragma once
#include "stdafx.h"

class NxShape;

class ITrigger
{
public:
    virtual ~ITrigger(void) {}
    
	virtual const tstring& GetTriggerName() const = 0;
	virtual void OnTriggerEnter(const NxShape& otherShape) = 0;
	virtual void OnTriggerLeave(const NxShape& otherShape) = 0;
};

