#pragma once
#include "RenderContext.h"
#include "ISerializable.h"

class Light;

class LightBehaviour : public ISerializable
{
public:
    virtual ~LightBehaviour(void) {}

    virtual void Init(Light* light) = 0;
    
	virtual void Tick(float dTime) = 0;
    
	//Serialize
	virtual void Serialize(Serializer* pSerializer) = 0;
	virtual void Deserialize(Serializer* pSerializer) = 0;
    virtual DWORD GetUniqueIdentifier() = 0;

    static ISerializable* Create(DWORD id);

protected:
    LightBehaviour(void) {}
};

