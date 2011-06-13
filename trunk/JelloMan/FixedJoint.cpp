#include "FixedJoint.h"


FixedJoint::FixedJoint(PhysX* pPhysX, NxActor* pActor1, NxActor* pActor2) : m_pJoint(0)
{
    NxFixedJointDesc desc;
    desc.actor[0] = pActor1;
    desc.actor[1] = pActor2;
    
	pPhysX->GetPhysXLock().lock();
    m_pJoint = dynamic_cast<NxFixedJoint*>(pPhysX->GetScene()->createJoint(desc));
	pPhysX->GetPhysXLock().unlock();
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
