#pragma once
#include "IJoint.h"

class FixedJoint : public IJoint
{
public:
    FixedJoint(PhysX* pPhysX, Actor* pActor1, Actor* pActor2 = 0);
    virtual ~FixedJoint(void);

    virtual void SetBreakable(float maxForce = FLT_MAX, float maxTorque = FLT_MAX);
    virtual void SetGlobalAnchor(const Vector3& anchor);
    
    virtual JointType GetType() = 0;

private:
    NxFixedJoint* m_pJoint;
};

