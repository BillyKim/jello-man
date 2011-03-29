#pragma once
#include "PhysXShape.h"
#include "Vector3.h"

class PhysXBox : public PhysXShape
{
public:
    PhysXBox(const Vector3& dimension, float mass);
    virtual ~PhysXBox(void);

    virtual NxShapeDesc* GetShape() const { return m_pBoxShapeDesc; }

private:
    NxBoxShapeDesc* m_pBoxShapeDesc;
};

