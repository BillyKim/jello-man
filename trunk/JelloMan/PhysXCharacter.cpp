#include "PhysXCharacter.h"

PhysXCharacter::PhysXCharacter(void): m_pPhysX(0), m_pController(0)
{
}


PhysXCharacter::~PhysXCharacter(void)
{
    m_pPhysX->GetControllerManager()->releaseController(*m_pController);
}

void PhysXCharacter::InitCharacterAsBox(PhysX* pPhysX, const Vector3& dim)
{
    m_pPhysX = pPhysX;

    NxBoxControllerDesc bdesc;

    bdesc.extents = dim;

    bdesc.callback = dynamic_cast<NxUserControllerHitReport*>(this);
    bdesc.interactionFlag = NXIF_INTERACTION_INCLUDE;
    bdesc.position = NxExtendedVec3(0, 0, 0);
    bdesc.skinWidth = 0.1f;
    bdesc.slopeLimit = 0.707f;
    bdesc.stepOffset = 0.5f;
    bdesc.upDirection = NX_Y;

    m_pController = pPhysX->GetControllerManager()->createController(pPhysX->GetScene(), bdesc);
}
void PhysXCharacter::InitCharacterAsCapsule(PhysX* pPhysX, float radius, float height)
{
    m_pPhysX = pPhysX;

    NxCapsuleControllerDesc cdesc;

    cdesc.climbingMode = CLIMB_CONSTRAINED;
    cdesc.height = height;
    cdesc.radius = radius;

    cdesc.callback = dynamic_cast<NxUserControllerHitReport*>(this);
    cdesc.interactionFlag = NXIF_INTERACTION_INCLUDE;
    cdesc.position = NxExtendedVec3(0, 0, 0);
    cdesc.skinWidth = 0.1f;
    cdesc.slopeLimit = 0.707f;
    cdesc.stepOffset = 0.5f;
    cdesc.upDirection = NX_Y;

    m_pController = pPhysX->GetControllerManager()->createController(pPhysX->GetScene(), cdesc);
    m_pController->setCollision(true);
}
NxControllerAction PhysXCharacter::onControllerHit(const NxControllersHit& hit)
{
    return OnControllerHit(hit);
}
NxControllerAction PhysXCharacter::onShapeHit(const NxControllerShapeHit& hit)
{
    return OnShapeHit(hit);
}


PhysXCharacterCollisionType PhysXCharacter::Move(const Vector3& speed)
{
    NxU32 flags;
    m_pController->move(speed, 0xffffffff, 0.001f, flags);

    return ((PhysXCharacterCollisionType)flags);
}

void PhysXCharacter::SetPosition(const Vector3& pos)
{
    m_pController->setPosition(pos);
}

Vector3 PhysXCharacter::GetPosition() const
{
    return m_pController->getPosition();
}