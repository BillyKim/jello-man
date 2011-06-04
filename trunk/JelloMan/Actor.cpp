#include "Actor.h"


Actor::Actor(void): m_mtxWorldMatrix(Matrix::Identity), m_pActor(0), m_pPhysX(0),m_bTrigger(false)
{
}


Actor::~Actor(void)
{
    if (m_pActor != 0)
	    m_pPhysX->GetScene()->releaseActor(*m_pActor);
}

void Actor::InitActor(PhysX* pPhysX, const PhysXShape& shape, bool moveable, bool bTrigger)
{
	ASSERT(m_pActor == 0, ""); //would be weird if the actor is already intialized

	m_bTrigger = bTrigger;
	m_pPhysX = pPhysX;
	
	//Make Actor
	NxBodyDesc bodyDesc;
	NxActorDesc actorDesc;
	
	if (bTrigger)
		shape.GetShape()->shapeFlags |= NX_TRIGGER_ENABLE;
	else
	{
		if(moveable == true)
		{
			bodyDesc.mass = shape.GetShape()->mass;
			//bodyDesc.angularDamping = 0.5f;
			//bodyDesc.linearDamping = 0.2f;
			bodyDesc.flags |= NX_BF_FILTER_SLEEP_VEL;
			//bodyDesc.sleepEnergyThreshold = 50.0f;
			//bodyDesc.sleepLinearVelocity = 0.001f;
			//bodyDesc.sleepAngularVelocity = 0.001f;
			actorDesc.body = &bodyDesc;		
		}
		else 
		{
			actorDesc.body = 0;
		}
	}

	actorDesc.shapes.push_back(shape.GetShape());
	actorDesc.globalPose = static_cast<NxMat34>(m_mtxWorldMatrix);
    actorDesc.group = 0;

    m_pPhysX->GetPhysXLock().lock();
	m_pActor = m_pPhysX->GetScene()->createActor(actorDesc);
    m_pPhysX->GetPhysXLock().unlock();

    ASSERT(m_pActor != 0, "");
}

void Actor::Tick(float /*dTime*/)
{
    m_mtxWorldMatrix = m_pActor->getGlobalPose();
}

void Actor::WakeUp()
{
    if (m_pActor->isDynamic())
    {
        m_pActor->wakeUp();
    }
}
void Actor::AddForce(const Vector3& pos)
{
	NxVec3 v = static_cast<NxVec3>(pos);
	m_pActor->addForce(v);
}
void Actor::Translate(const Vector3& pos)
{
    SetPosition(GetPosition() + pos);
}
void Actor::SetPosition(const Vector3& pos)
{
	ASSERT(m_pActor != 0, "");
    m_pPhysX->GetPhysXLock().lock();
    m_pActor->setGlobalPosition(pos);
    m_pPhysX->GetPhysXLock().unlock();
    m_mtxWorldMatrix = m_pActor->getGlobalPose();
}
Vector3 Actor::GetPosition() const
{
    ASSERT(m_pActor != 0, "");
    return static_cast<Vector3>(m_pActor->getGlobalPosition());
}
void Actor::Rotate(const Vector3& axis, float angle)
{
	ASSERT(m_pActor != 0, "");
    Vector3 pos = GetPosition();
    m_mtxWorldMatrix *= Matrix::CreateTranslation(-pos);
    m_mtxWorldMatrix *= Matrix::CreateRotation(axis, angle);
    m_mtxWorldMatrix *= Matrix::CreateTranslation(pos);

	NxMat34 mat = static_cast<NxMat34>(m_mtxWorldMatrix);
	m_pActor->setGlobalPose(mat);
	
}
void Actor::Scale(const Vector3& /*scale*/)
{
	/*ASSERT(m_pActor != 0, "");

	if (m_pActor->getShapes()[0]->isSphere())
	{
		NxSphereShape* shape = dynamic_cast<NxSphereShape*>(m_pActor->getShapes()[0]);
		shape->setRadius(shape->getRadius() * ((scale.X + scale.Y + scale.Z) / 3.0f));
	}
	else if (m_pActor->getShapes()[0]->isBox())
	{
		NxBoxShape* shape = dynamic_cast<NxBoxShape*>(m_pActor->getShapes()[0]);
		shape->setDimensions(Vector3(shape->getDimensions()) * scale);
	}
	else if (m_pActor->getShapes()[0]->isConvexMesh())
	{
		NxConvexMesh* shape = dynamic_cast<NxConvexMesh*>(m_pActor->getShapes()[0]);
		shape->
	}

    Vector3 pos = GetPosition();
    m_mtxWorldMatrix *= Matrix::CreateTranslation(-pos);
    m_mtxWorldMatrix *= Matrix::CreateScale(scale);
    m_mtxWorldMatrix *= Matrix::CreateTranslation(pos);

	NxMat34 mat = static_cast<NxMat34>(m_mtxWorldMatrix);
	m_pActor->setGlobalPose(mat);*/
}
void Actor::Serialize(Serializer* pSerializer)  const
{
    Vector3 av(m_pActor->isDynamic()?m_pActor->getAngularVelocity():Vector3::Zero);
    pSerializer->GetStream()->storeVector3(av);

    Vector3 lv(m_pActor->isDynamic()?m_pActor->getLinearVelocity():Vector3::Zero);
    pSerializer->GetStream()->storeVector3(lv);

    pSerializer->GetStream()->storeMatrix(m_mtxWorldMatrix);

	pSerializer->GetStream()->storeByte(m_bTrigger?TRUE:FALSE);
}
void Actor::Deserialize(Serializer* pSerializer)
{
    ASSERT(m_pActor != 0, "");

    NxVec3 av(static_cast<NxVec3>(pSerializer->GetStream()->readVector3()));
    NxVec3 lv(static_cast<NxVec3>(pSerializer->GetStream()->readVector3()));

    if (m_pActor->isDynamic())
    {
        m_pActor->setAngularVelocity(av);
        m_pActor->setLinearVelocity(lv);
    }

    m_mtxWorldMatrix = pSerializer->GetStream()->readMatrix();
    NxMat34 mat(static_cast<NxMat34>(m_mtxWorldMatrix));
    m_pActor->setGlobalPose(mat);

	if (pSerializer->GetStream()->readByte() == TRUE)
	{
		m_pActor->getShapes()[0]->setFlag(NX_TRIGGER_ENABLE, true);
	}
}