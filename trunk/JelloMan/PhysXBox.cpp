#include "PhysXBox.h"


PhysXBox::PhysXBox(const Vector3& dimension, float mass): m_pBoxShapeDesc(new NxBoxShapeDesc())
{
    m_pBoxShapeDesc->dimensions = static_cast<NxVec3>(dimension/2.0f);
    m_pBoxShapeDesc->mass = mass;
    //float volume = dimension.X * dimension.Y * dimension.Z;
    //m_pBoxShapeDesc->density = mass / volume;                     //physX calculates this
}


PhysXBox::~PhysXBox(void)
{
    delete m_pBoxShapeDesc;
}
