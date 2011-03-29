#pragma once
#include "PhysXShape.h"
#include "Model.h"
#include "PhysX.h"

class PhysXMesh : public PhysXShape
{
public:
    PhysXMesh(PhysX* physX, const string& path, float mass);

    virtual ~PhysXMesh(void);

    virtual NxShapeDesc* GetShape() const { return m_pShapeDesc; }

private:
    NxShapeDesc* m_pShapeDesc;
};

