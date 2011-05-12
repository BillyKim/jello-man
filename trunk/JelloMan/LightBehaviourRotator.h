#pragma once
#include "LightBehaviour.h"

class LightBehaviourRotator : public LightBehaviour
{
public:
    LightBehaviourRotator(const Vector3& axis, float speed);
    virtual ~LightBehaviourRotator(void);
    
    virtual void Init(Light* light);

    virtual void Tick(float dTime);
    
	//Serialize
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::LightBehaviourRotator; }

private:
    Light* m_pLight;

    Vector3 m_Axis;
    float m_Speed;
};

