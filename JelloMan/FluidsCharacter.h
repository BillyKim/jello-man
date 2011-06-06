#pragma once

#include "D3DUtil.h"
#include "Fluid.h"
#include "SoftbodyCharacter.h"
#include "Vector3.h"
#include "PhysX.h"
#include "RenderContext.h"
#include "PhysXCharacter.h"
#include "PhysXSphere.h"
#include "Blox2D.h"
#include "ILevelObject.h"
#include "ISerializable.h"
#include "FollowCamera.h"
#include "ICharacter.h"

class Level;

class FluidsCharacter : public PhysXCharacter, public ILevelObject, public ISerializable, public ICharacter
{
    enum GravityType
    {
        GravityType_Left, GravityType_Right, GravityType_Up, GravityType_Down
    };

public:

	/* CONSTRUCTOR - DESTRUCTOR */
	FluidsCharacter(Level* pLevel);
	virtual ~FluidsCharacter();

	/* GENERAL */
	void Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, 
              Graphics::Camera::FollowCamera* pCamera, 
              unsigned int maxParticles, 
              Vector3 startPos);

    void ChangeMoveDirection(const Vector3& dir); //must be normalized!
    void ChangeGravityDirection(const Vector3& dir); //must be normalized!
    void ChangeMoveSpeed(float speed);

    virtual IEditorObject* Copy() const { return 0; } //non copyable
	
    /* ICharacter */
    virtual bool IsDead() const { return m_IsDead; }
    virtual void Respawn(const SpawnPoint* pSpawnPoint);

	/* IUpdateable */
	virtual void Tick(float dTime);

	/* IDrawable */
	virtual void Draw(RenderContext* pRenderContext);
	virtual void DrawShadow(RenderContext* /*pRenderContext*/) {}

	virtual void Serialize(Serializer* /*pSerializer*/)  const {}
	virtual void Deserialize(Serializer* /*pSerializer*/) {}
	virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::SimpleObject; }

	/* ILevelObject */
	virtual void Init(PhysX* /*pPhysX*/) {}
	virtual void Selected(bool selected) { m_bIsSelected = selected; }
    virtual bool IsSelected() const { return m_bIsSelected; }

	/* ITransformable */
    virtual void Rotate(const Vector3& /*axis*/, float /*angle*/) { }

    virtual void Translate(const Vector3& add) { PhysXCharacter::Move(add); }
    virtual void SetPosition(const Vector3& pos) { PhysXCharacter::SetPosition(pos); }
    virtual Vector3 GetPosition() const { return PhysXCharacter::GetPosition(); }

	virtual void Scale(const Vector3& /*scale*/) { }

private:
	/* DATAMEMBERS */

	PhysX* m_pPhysXEngine;
    Level* m_pLevel;

    Vector3 m_Speed;

    Vector3 m_MoveDir;
    float m_MoveSpeed;

    Vector3 m_RightDir;
    GravityType m_GravityType;
    bool m_CanSwitchGravity;

	Fluid* m_pFluid;
	NxFluidEmitter* m_pEmitter;

	bool m_bIsSelected;

	Graphics::Camera::FollowCamera* m_pCamera;

    bool m_IsTouchingGround;

    bool m_IsDead;
};

