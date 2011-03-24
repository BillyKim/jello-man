#include "Actor.h"


Actor::Actor(void)
{
}


Actor::~Actor(void)
{
	m_pPhysX->GetScene()->releaseActor(*m_pActor);
}

void Actor::InitActor(PhysX* pPhysX, bool moveable, const PhysXShape& shape)
{
	ASSERT(m_pActor == 0); //would be weird if the actor is already intialized

	m_pPhysX = pPhysX;
	
	//Make Actor
	NxActorDesc ActorDesc;
	ActorDesc.shapes.push_back(shape.GetShape());
	
	if(moveable == true)
	{
		//bodyNx
		NxBodyDesc bodyDesc;
		bodyDesc.angularDamping	= 0.9f;
		bodyDesc.linearVelocity = NxVec3(0,0,0);
		ActorDesc.body = &bodyDesc;
	}
	else 
	{
		ActorDesc.body = 0;
	}

	NxMat34 mat = static_cast<NxMat34>(m_WorldMatrix);
	ActorDesc.globalPose = mat;
	ActorDesc.density = 10.0f;
	m_pActor = m_pPhysX->GetScene()->createActor(ActorDesc);
}

void Actor::Translate(const Vector3& pos)
{
	ASSERT(m_pActor == 0);

	NxMat34 mat = static_cast<NxMat34>(m_WorldMatrix);
	m_pActor->setGlobalPose(mat);
}

void Actor::AddForce(const Vector3& pos)
{
	NxVec3 v = static_cast<NxVec3>(pos);
	m_pActor->addForce(v);
}