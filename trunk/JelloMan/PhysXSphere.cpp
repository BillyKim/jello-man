#include "PhysXSphere.h"


PhysXSphere::PhysXSphere(float radius, float mass): m_pSphereDesc(new NxSphereShapeDesc())
{
    m_pSphereDesc->radius = radius;
    m_pSphereDesc->mass = mass;
    //float volume = 4.0f / 3.0f * Pi * pow(radius, 3.0f);
    //m_pSphereDesc->density = mass / volume;                   //physX calculates this
}


PhysXSphere::~PhysXSphere(void)
{
    delete m_pSphereDesc;
}
