#pragma once
#include "CookingStream.h"

class ISerializable
{
public:
	//Serialize
	virtual void Serialize(UserStream* stream) = 0;
	virtual void Deserialize(UserStream* stream) = 0;
};