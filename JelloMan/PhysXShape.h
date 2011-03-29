#pragma once
#include "NxPhysics.h"

class PhysXShape
{
public:
	virtual NxShapeDesc* GetShape() const = 0;
};

