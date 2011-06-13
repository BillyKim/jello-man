#pragma once

#include "D3DUtil.h"
#include "Vector3.h"

#include "ISerializable.h"

class PhysX;

class ForceField : public ISerializable
{
public:

	/* CONSTRUCTOR - DESTRUCTOR*/
	ForceField();
	virtual ~ForceField();

	/* GENERAL */
	void Create(PhysX* pPhysX, const Vector3& force, const Vector3& dimensions, const Vector3& position);

	/* GETTERS */
	NxForceField* GetNxForceField() const
	{ return m_pForceField; }

	/* SERIALIZE */
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
	virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::PhysXForceField; }

private:

	/* DATAMEMBERS */
	NxForceField* m_pForceField;

	Vector3 m_Force;
	Vector3 m_Dimensions;
	Vector3 m_Position;

	PhysX* m_pPhysX;

	/* COPY & ASSIGNMENT */
	ForceField(const ForceField&);
	ForceField& operator=(const ForceField&);
};

