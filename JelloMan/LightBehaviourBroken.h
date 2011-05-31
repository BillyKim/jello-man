#pragma once
#include "LightBehaviour.h"
#include "Light.h"

enum BrokenLightType
{
    BrokenLightType_Fluorescent,
    BrokenLightType_LightBulb
};

class LightBehaviourBroken : public LightBehaviour
{
public:
    LightBehaviourBroken(BrokenLightType type);
    virtual ~LightBehaviourBroken(void);
    
    virtual void Init(Light* light);

    virtual LightBehaviour* Copy() const;

    virtual void Tick(float dTime);

	//Serialize
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::LightBehaviourBroken; }

private:
    Light* m_pLight;

    BrokenLightType m_Type;

    float m_Multiplier;
    float m_MultiplierNormal;

    bool m_IsOn;

    float m_WaitTime;
};

