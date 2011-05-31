#pragma once
#include "LightBehaviour.h"

class LightBehaviourNormal : public LightBehaviour
{
public:
    LightBehaviourNormal(void);
    virtual ~LightBehaviourNormal(void);
    
    virtual void Init(Light*) {};
    
    virtual LightBehaviour* Copy() const;

    virtual void Tick(float) {}

	//Serialize
	virtual void Serialize(Serializer*) const;
	virtual void Deserialize(Serializer*);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::LightBehaviourNormal; }
};

