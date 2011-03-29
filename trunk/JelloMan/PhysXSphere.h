#pragma once
#include "PhysXShape.h"

class PhysXSphere : public PhysXShape
{
public:
    PhysXSphere(float radius, float mass);
    virtual ~PhysXSphere(void);
    
    virtual NxShapeDesc* GetShape() const { return m_pSphereDesc; }

private:
    NxSphereShapeDesc* m_pSphereDesc;
};

