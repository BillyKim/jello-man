#pragma once
#include "PhysX.h"
#pragma warning(disable:4512)
#include <NxCapsuleController.h>
#include <NxBoxController.h>
#pragma warning(default:4512)
#include "Vector3.h"

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

    void InitCharacterAsBox(PhysX* pPhysX, const Vector3& dim);
    void InitCharacterAsCapsule(PhysX* pPhysX, float radius, float height);
    
    PhysXCharacterCollisionType Move(const Vector3& speed);
    void SetPosition(const Vector3& pos);

    Vector3 GetPosition() const;

    virtual NxControllerAction OnShapeHit(const NxControllerShapeHit&) { return NX_ACTION_PUSH; }
    virtual NxControllerAction OnControllerHit(const NxControllersHit&) { return NX_ACTION_PUSH; }

private:
    virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit);
    virtual NxControllerAction onControllerHit(const NxControllersHit& hit);

    PhysX* m_pPhysX;
    NxController* m_pController;

	/* Disableing default copyconstructor and assignment operator */
	PhysXCharacter(const PhysXCharacter&);
	PhysXCharacter& operator=(const PhysXCharacter&);
};

