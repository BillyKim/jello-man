#pragma once
#include "Actor.h"
#include "ISerializable.h"
#include "ICharacter.h"
#include "FollowCamera.h"

class Level;
struct Vector3;
class PhysX;
class Fluid;
class NxFluidEmitter;



class FluidsCharacterActor : public Actor, public ICharacter
{
public:
	/* CONSTRUCTOR - DESTRUCTOR */
	FluidsCharacterActor(Level* pLevel);
	virtual ~FluidsCharacterActor();

	/* GENERAL */
	void Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, 
              Graphics::Camera::FollowCamera* pCamera, 
              Vector3 startPos);

    void ChangeMoveDirection(const Vector3& dir); //must be normalized!
    void ChangeGravityDirection(const Vector3& dir); //must be normalized!
    void ChangeMoveSpeed(float speed);
	
    /* ICharacter */
    virtual bool IsDead() const { return m_IsDead; }
    virtual void Respawn(const SpawnPoint* pSpawnPoint);

	/* IUpdateable */
	virtual void Tick(float dTime);

	/* IDrawable */
	virtual void Draw(RenderContext* pRenderContext);
	virtual void DrawShadow(RenderContext* /*pRenderContext*/) {}

private:
	void CheckIfOnGround();

	/* DATAMEMBERS */

	PhysX* m_pPhysXEngine;
    Level* m_pLevel;

    Vector3 m_MoveDir;
    Vector3 m_RightDir;
    float m_MoveSpeed;

    bool m_CanSwitchGravity;
	float m_Radius;

	Fluid* m_pFluid;
	NxFluidEmitter* m_pEmitter;

	bool m_bIsSelected;

	Graphics::Camera::FollowCamera* m_pCamera;

    bool m_IsTouchingGround;

    bool m_IsDead;
};

