#include "Actor.h"


Actor::Actor(void): m_WorldMatrix(Matrix::Identity), m_pActor(0), m_pPhysX(0)
{
}


Actor::~Actor(void)
{
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
        bodyDesc.flags |= NX_BF_ENERGY_SLEEP_TEST;
        bodyDesc.sleepEnergyThreshold = 5000.0f;
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

void Actor::Update(float dTime)
{
    m_WorldMatrix = m_pActor->getGlobalPose();
}

void Actor::Translate(const Vector3& pos)
{
	ASSERT(m_pActor != 0);
    m_WorldMatrix *= Matrix::CreateTranslation(pos);

	NxMat34 mat = static_cast<NxMat34>(m_WorldMatrix);
	m_pActor->setGlobalPose(mat);
}

void Actor::AddForce(const Vector3& pos)
{
	NxVec3 v = static_cast<NxVec3>(pos);
	m_pActor->addForce(v);
}

Vector3 Actor::GetPosition() const
{
	return Vector3(
		m_pActor->getGlobalPosition().x,
		m_pActor->getGlobalPosition().y,
		m_pActor->getGlobalPosition().z);
}