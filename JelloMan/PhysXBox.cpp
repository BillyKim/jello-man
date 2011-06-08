#include "PhysXBox.h"

PhysXBox::PhysXBox(): m_pBoxShapeDesc(0)
{
}
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

PhysXShape* PhysXBox::Copy() const
{
    PhysXBox* b = new PhysXBox();
    b->m_pBoxShapeDesc = new NxBoxShapeDesc();
    b->m_pBoxShapeDesc->dimensions = m_pBoxShapeDesc->dimensions;
    b->m_pBoxShapeDesc->mass = m_pBoxShapeDesc->mass;

    return b;
}


void PhysXBox::Serialize(Serializer* pSerializer) const
{
    pSerializer->GetStream()->storeVector3(Vector3(m_pBoxShapeDesc->dimensions));
    pSerializer->GetStream()->storeFloat(m_pBoxShapeDesc->mass);
}
void PhysXBox::Deserialize(Serializer* pSerializer)
{
    SafeDelete(m_pBoxShapeDesc);

    m_pBoxShapeDesc = new NxBoxShapeDesc();
    m_pBoxShapeDesc->dimensions = static_cast<NxVec3>(pSerializer->GetStream()->readVector3());
    m_pBoxShapeDesc->mass = pSerializer->GetStream()->readFloat();
}