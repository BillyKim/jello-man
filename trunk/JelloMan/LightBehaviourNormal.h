#pragma once
#include "LightBehaviour.h"

class LightBehaviourNormal : public LightBehaviour
{
public:
    LightBehaviourNormal(void);
    virtual ~LightBehaviourNormal(void);
    
    virtual void Init(Light*) {};

    virtual void Tick(float) {}

	//Serialize
	virtual void Serialize(Serializer*);
	virtual void Deserialize(Serializer*);
    virtual DWORD GetUniqueIdentifier() { return SerializeTypes::LightBehaviourNormal; }
};

