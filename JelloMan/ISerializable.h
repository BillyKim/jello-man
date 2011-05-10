#pragma once
#include "Serializer.h"
#include "SerializeTypes.h"

class ISerializable
{
public:
    virtual ~ISerializable() {}

	//Serialize
	virtual void Serialize(Serializer* pSerializer) = 0;
	virtual void Deserialize(Serializer* pSerializer) = 0;
    virtual DWORD GetUniqueIdentifier() = 0;
};