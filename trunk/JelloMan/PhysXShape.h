#pragma once
#include "NxPhysics.h"
#include "ISerializable.h"

class PhysXShape : public ISerializable
{
public:
	virtual NxShapeDesc* GetShape() const = 0;

	virtual void Serialize(Serializer* pSerializer) = 0;
	virtual void Deserialize(Serializer* pSerializer) = 0;
    virtual DWORD GetUniqueIdentifier() = 0;

    static ISerializable* GetShape(DWORD type);
};

