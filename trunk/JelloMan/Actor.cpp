#include "Actor.h"


Actor::Actor(void): m_WorldMatrix(Matrix::Identity), m_pActor(0), m_pPhysX(0)
{
}


Actor::~Actor(void)
{
    if (m_pActor != 0)
	    m_pPhysX->GetScene()->releaseActor(*m_pActor);
}

void Actor::InitActor(PhysX* pPhysX, const PhysXShape& shape, bool moveable)
{
	ASSERT(m_pActor == 0); //would be weird if the actor is already intialized

	m_pPhysX = pPhysX;
	
	//Make Actor
	NxActorDesc actorDesc;
	
	if(moveable == true)
	{
		NxBodyDesc bodyDesc;
		bodyDesc.mass = shape.GetShape()->mass;
		bodyDesc.angularDamping = 0.5f;
        bodyDesc.linearDamping = 0.2f;
        bodyDesc.flags |= NX_BF_FILTER_SLEEP_VEL;
        bodyDesc.sleepEnergyThreshold = 5000.0f;
		//bodyDesc.sleepLinearVelocity = 0.001f;
		//bodyDesc.sleepAngularVelocity = 0.001f;
		actorDesc.body = &bodyDesc;		
	}
	else 
	{
		actorDesc.body = 0;
	}

	actorDesc.shapes.push_back(shape.GetShape());
	actorDesc.globalPose = static_cast<NxMat34>(m_WorldMatrix);

	m_pActor = m_pPhysX->GetScene()->createActor(actorDesc);

    ASSERT(m_pActor != 0);
}

void Actor::Tick(float dTime)
{
    m_WorldMatrix = m_pActor->getGlobalPose();
}

void Actor::AddForce(const Vector3& pos)
{
	NxVec3 v = static_cast<NxVec3>(pos);
	m_pActor->addForce(v);
}
void Actor::Translate(const Vector3& pos)
{
	ASSERT(m_pActor != 0);
    m_WorldMatrix *= Matrix::CreateTranslation(pos);

	NxMat34 mat = static_cast<NxMat34>(m_WorldMatrix);
	m_pActor->setGlobalPose(mat);
}
void Actor::SetPosition(const Vector3& pos)
{
	ASSERT(m_pActor != 0);
    m_WorldMatrix *= Matrix::CreateTranslation(-GetPosition() + pos);

	NxMat34 mat = static_cast<NxMat34>(m_WorldMatrix);
	m_pActor->setGlobalPose(mat);
}
Vector3 Actor::GetPosition() const
{
	if (m_pActor)
	{
		return Vector3(
			m_pActor->getGlobalPosition().x,
			m_pActor->getGlobalPosition().y,
			m_pActor->getGlobalPosition().z);
	}

	return Vector3(0,0,0);
}
void Actor::Rotate(const Vector3& axis, float angle)
{
	ASSERT(m_pActor != 0);
    Vector3 pos = GetPosition();
    m_WorldMatrix *= Matrix::CreateTranslation(-pos);
    m_WorldMatrix *= Matrix::CreateRotation(axis, angle);
    m_WorldMatrix *= Matrix::CreateTranslation(pos);

	NxMat34 mat = static_cast<NxMat34>(m_WorldMatrix);
	m_pActor->setGlobalPose(mat);
}
void Actor::Scale(const Vector3& scale)
{
	ASSERT(m_pActor != 0);
    Vector3 pos = GetPosition();
    m_WorldMatrix *= Matrix::CreateTranslation(-pos);
    m_WorldMatrix *= Matrix::CreateScale(scale);
    m_WorldMatrix *= Matrix::CreateTranslation(pos);

	NxMat34 mat = static_cast<NxMat34>(m_WorldMatrix);
	m_pActor->setGlobalPose(mat);
}