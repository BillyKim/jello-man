#include "FixedJoint.h"


FixedJoint::FixedJoint(PhysX* pPhysX, Actor* pActor1, Actor* pActor2) : m_pJoint(0)
{
    NxFixedJointDesc desc;
    desc.actor[0] = pActor1->GetActor();
    desc.actor[1] = pActor2->GetActor();
    
    m_pJoint = dynamic_cast<NxFixedJoint*>(pPhysX->GetScene()->createJoint(desc));
}


FixedJoint::~FixedJoint(void)
{
    m_pJoint->getScene().releaseJoint(*m_pJoint);
}

void FixedJoint::SetBreakable(float maxForce, float maxTorque)
{
    m_pJoint->setBreakable(maxForce, maxTorque);
}
void FixedJoint::SetGlobalAnchor(const Vector3& anchor)
{
    m_pJoint->setGlobalAnchor(anchor);
}
