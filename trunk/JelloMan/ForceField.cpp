#include "ForceField.h"
#include "Actor.h"
#include "PhysX.h"

/* CONSTRUCTOR - DESTRUCTOR*/
ForceField::ForceField() :	m_Force(0,0,0),
							m_Dimensions(0,0,0),
							m_Position(0,0,0)
{
}

ForceField::~ForceField()
{
	m_pPhysX->GetScene()->releaseForceField(*m_pForceField);
}

/* GENERAL */
void ForceField::Create(PhysX* pPhysX, const Vector3& force, const Vector3& dimensions, const Vector3& position)
{
	ASSERT(pPhysX != 0, "PhysX engine error while creating forcefield");

	m_Force = force;
	m_Dimensions = dimensions;
	m_Position = position;

	NxForceFieldLinearKernelDesc linearKernelDesc;
	linearKernelDesc.constant = force;
	   
	//The forces do not depend on where the objects are positioned
	NxMat33 m;
	m.zero();
	linearKernelDesc.positionMultiplier = m;
	linearKernelDesc.noise = NxVec3(1.3,1.3,1.3); //adds a random noise on the forces to make the objects a little more chaotic

	//Set target velocity along the radius to 20
	linearKernelDesc.velocityTarget = NxVec3(10,0,0);
	m.diagonal(NxVec3(1,0,0)); //Acts with a force relative to the current velocity to reach the
							   //target velocities. 0 means that those components won't be affected
	linearKernelDesc.velocityMultiplier = m;

	NxForceFieldLinearKernel* pLinearKernel;
	pLinearKernel = pPhysX->GetScene()->createForceFieldLinearKernel(linearKernelDesc);

	NxForceFieldDesc fieldDesc;
	fieldDesc.coordinates = NX_FFC_SPHERICAL;
	fieldDesc.kernel = pLinearKernel;
	fieldDesc.group = 4;

	Matrix mat = Matrix::CreateTranslation(position);

	fieldDesc.pose = mat;

	/*if (pActor)
		fieldDesc.actor = pActor->GetActor();*/

	m_pForceField = pPhysX->GetScene()->createForceField(fieldDesc);

	NxForceFieldShape* s = NULL;
	NxBoxForceFieldShapeDesc b;
	b.dimensions = dimensions;
	b.pose.t = NxVec3(0, 0, 0);
	s = m_pForceField->getIncludeShapeGroup().createShape(b);
}

void ForceField::Serialize(Serializer* pSerializer) const
{
	pSerializer->GetStream()->storeVector3(m_Force);
	pSerializer->GetStream()->storeVector3(m_Dimensions);
	pSerializer->GetStream()->storeVector3(m_Position);
}
void ForceField::Deserialize(Serializer* pSerializer)
{
	Vector3 force(pSerializer->GetStream()->readVector3());
	Vector3 dim(pSerializer->GetStream()->readVector3());
	Vector3 pos(pSerializer->GetStream()->readVector3());

	Create(pSerializer->GetPhysX(), force, dim, pos);
}