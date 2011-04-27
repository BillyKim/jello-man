#pragma once
#include "IJoint.h"

class RevoluteJoint : public IJoint
{
public:
    RevoluteJoint(PhysX* pPhysX, Actor* pActor1, Actor* pActor2 = 0);
    virtual ~RevoluteJoint(void);

    virtual void SetBreakable(float maxForce = FLT_MAX, float maxTorque = FLT_MAX);
    virtual void SetGlobalAnchor(const Vector3& anchor);
    virtual void SetGlobalAxis(const Vector3& axis);

    virtual void SetMotor(bool enable, float velocityTarget = 0, float maxForce = FLT_MAX, bool freeSpin = false);
    virtual void SetLimits(bool enable, float low = -Pi, float high = Pi);

    virtual JointType GetType();

private:
    NxRevoluteJoint* m_pJoint;
};

