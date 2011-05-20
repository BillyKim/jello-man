#pragma once

#include "D3DUtil.h"
#include "Fluid.h"
#include "SoftbodyCharacter.h"
#include "Vector3.h"
#include "PhysX.h"
#include "RenderContext.h"
#include "Actor.h"
#include "PhysXSphere.h"
#include "Blox2D.h"
#include "ILevelObject.h"
#include "ISerializable.h"
#include "FollowCamera.h"

class FluidsCharacter : public Actor, public ILevelObject, public ISerializable
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	FluidsCharacter();
	virtual ~FluidsCharacter();

	/* GENERAL */
	void Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, Graphics::Camera::FollowCamera* pCamera, unsigned int maxParticles, Vector3 startPos);
	
	/* IUpdateable */
	virtual void Tick(float dTime);

	/* IDrawable */
	virtual void Draw(RenderContext* pRenderContext);
	virtual void DrawShadow(RenderContext* pRenderContext, PreShadowEffect* e) {}

	virtual void Serialize(Serializer* pSerializer)  const {}
	virtual void Deserialize(Serializer* pSerializer) {}
	virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::SimpleObject; }

	/* ILevelObject */
	virtual void Init(PhysX* pPhysX) {}
	virtual void Selected(bool selected) { m_bIsSelected = selected; }
    virtual bool IsSelected() const { return m_bIsSelected; }

	/* ITransformable */
    virtual void Rotate(const Vector3& axis, float angle) { Actor::Rotate(axis, angle); }

    virtual void Translate(const Vector3& add) { Actor::Translate(add); }
    virtual void SetPosition(const Vector3& pos) { Actor::SetPosition(pos); }
    virtual Vector3 GetPosition() const { return Actor::GetPosition(); }

	virtual void Scale(const Vector3& scale) { Actor::Scale(scale); }

private:

	/* DATAMEMBERS */

	PhysX* m_pPhysXEngine;

	Fluid* m_pFluid;

	NxFluidEmitter* m_pEmitter;

	Vector3 m_Pos;

	bool m_bIsSelected;

	Graphics::Camera::FollowCamera* m_pCamera;
};

