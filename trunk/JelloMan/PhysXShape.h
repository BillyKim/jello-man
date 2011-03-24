#pragma once
#include "NxPhysics.h"

class PhysXShape
{
public:
	PhysXShape(void);
	~PhysXShape(void);

	NxShapeDesc* GetShape() const { return m_Shape; }

protected:
	NxShapeDesc* m_Shape;
};

