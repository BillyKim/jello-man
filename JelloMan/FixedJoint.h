#pragma once
#include "IJoint.h"

class FixedJoint : public IJoint
{
public:
    FixedJoint(PhysX* pPhysX, NxActor* pActor1, NxActor* pActor2 = 0);
    virtual ~FixedJoint(void);

    virtual void SetBreakable(float maxForce = FLT_MAX, float maxTorque = FLT_MAX);
    virtual void SetGlobalAnchor(const Vector3& anchor);
    
	virtual JointType GetType() const { return JointType_Fixed; };

private:
    NxFixedJoint* m_pJoint;

	
	/* Disableing default copyconstructor and assignment operator */
	FixedJoint(const FixedJoint&);
	FixedJoint& operator=(const FixedJoint&);
};

