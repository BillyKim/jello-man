#include "RevoluteJoint.h"


RevoluteJoint::RevoluteJoint(PhysX* pPhysX, Actor* pActor1, Actor* pActor2): m_pJoint(0)
{
    NxRevoluteJointDesc desc;
    desc.actor[0] = pActor1->GetActor();
    desc.actor[1] = pActor2->GetActor();

    m_pJoint = dynamic_cast<NxRevoluteJoint*>(pPhysX->GetScene()->createJoint(desc));
}


RevoluteJoint::~RevoluteJoint(void)
{
    m_pJoint->getScene().releaseJoint(*m_pJoint);
}

void RevoluteJoint::SetBreakable(float maxForce, float maxTorque)
{
    m_pJoint->setBreakable(maxForce, maxTorque);
}
void RevoluteJoint::SetGlobalAnchor(const Vector3& anchor)
{
    m_pJoint->setGlobalAnchor(anchor);
}
void RevoluteJoint::SetGlobalAxis(const Vector3& axis)
{
    m_pJoint->setGlobalAxis(axis);
}

void RevoluteJoint::SetMotor(bool enable, float velocityTarget, float maxForce, bool freeSpin)
{
    if (enable)
    {
        m_pJoint->setFlags(m_pJoint->getFlags() | NX_RJF_MOTOR_ENABLED);

        NxMotorDesc desc;
        desc.freeSpin = freeSpin;
        desc.maxForce = maxForce;
        desc.velTarget = velocityTarget;
        m_pJoint->setMotor(desc);
    }
    else
    {
        m_pJoint->setFlags(m_pJoint->getFlags() | ~NX_RJF_MOTOR_ENABLED);
    }
}
void RevoluteJoint::SetLimits(bool enable, float low, float high)
{
    if (enable)
    {
        m_pJoint->setFlags(m_pJoint->getFlags() | NX_RJF_LIMIT_ENABLED);

        NxJointLimitPairDesc desc;
        NxJointLimitDesc lowDesc, highDesc;
        lowDesc.value = low;
        highDesc.value = high;
        desc.low = lowDesc;
        desc.high = highDesc;
        m_pJoint->setLimits(desc);
    }
    else
    {
        m_pJoint->setFlags(m_pJoint->getFlags() | ~NX_RJF_LIMIT_ENABLED);
    }
}

JointType RevoluteJoint::GetType()
{
    return static_cast<JointType>(m_pJoint->getType());
}