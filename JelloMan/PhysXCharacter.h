#pragma once
#include "PhysX.h"
#include <NxCapsuleController.h>
#include <NxBoxController.h>
#include "Vector3.h"

enum PhysXCharactertype
{
    PhysXCharactertype_Box, PhysXCharactertype_Capsule
};

enum PhysXCharacterCollisionType
{
    PhysXCharacterCollisionType_Side = 1 << 0,
	PhysXCharacterCollisionType_Up	 = 1 << 1,
	PhysXCharacterCollisionType_Down = 1 << 2
};

class PhysXCharacter : public NxUserControllerHitReport
{
public:
    PhysXCharacter(void);
    virtual ~PhysXCharacter(void);

    void InitCharacter(PhysX* pPhysX, PhysXCharactertype type);
    
    PhysXCharacterCollisionType Move(const Vector3& speed);
    void SetPosition(const Vector3& pos);

    Vector3 GetPosition() const;

    virtual NxControllerAction OnShapeHit(const NxControllerShapeHit&) { return NX_ACTION_NONE; }
    virtual NxControllerAction OnControllerHit(const NxControllersHit&) { return NX_ACTION_NONE; }

private:
    virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit);
    virtual NxControllerAction onControllerHit(const NxControllersHit& hit);

    PhysX* m_pPhysX;
    NxController* m_pController;
};

