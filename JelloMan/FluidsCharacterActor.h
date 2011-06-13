#pragma once
#include "Actor.h"
#include "ISerializable.h"
#include "ITriggerable.h"
#include "ICharacter.h"
#include "FollowCamera.h"

class Level;
struct Vector3;
class PhysX;
class Fluid;
class NxFluidEmitter;
struct UserData;


class FluidsCharacterActor : public Actor, public ICharacter, public ITriggerable
{
public:
	/* CONSTRUCTOR - DESTRUCTOR */
	FluidsCharacterActor(Level* pLevel);
	virtual ~FluidsCharacterActor();

	/* GENERAL */
	void Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, 
              Graphics::Camera::FollowCamera* pCamera, 
              Vector3 startPos);

    void RotateMoveDirection(float rad);
    void RotateGravityDirection(float rad);
    void ChangeMoveSpeed(float speed);
	
    /* ICharacter */
    virtual bool IsDead() const { return m_IsDead; }
    virtual void Respawn(const SpawnPoint* pSpawnPoint);

	virtual void Tick(float dTime);
	virtual void Draw(const RenderContext* pRenderContext);

    /* ITriggerable */
    virtual void OnTriggerEnter(const ITrigger* pTrigger);
    virtual void OnTriggerLeave(const ITrigger* pTrigger);

private:
	void CheckIfOnGround();

	/* DATAMEMBERS */

	PhysX* m_pPhysXEngine;
    Level* m_pLevel;
    UserData* m_pUserData;

    Vector3 m_MoveDir;
    Vector3 m_RightDir;
    float m_MoveSpeed;

    bool m_CanSwitchGravity;
	float m_Radius;

    float m_GravityRotation;
    float m_MoveRotation;

	Fluid* m_pFluid;
	NxFluidEmitter* m_pEmitter;

	bool m_bIsSelected;

	Graphics::Camera::FollowCamera* m_pCamera;

    bool m_IsTouchingGround;

    bool m_IsDead;

    UINT m_Score;
    float m_ScoreTimer;
};

