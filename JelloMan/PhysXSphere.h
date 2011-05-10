#pragma once
#include "PhysXShape.h"

class PhysXSphere : public PhysXShape
{
public:
    PhysXSphere(float radius, float mass);
    PhysXSphere();
    virtual ~PhysXSphere(void);
    
    virtual NxShapeDesc* GetShape() const { return m_pSphereDesc; }
    
	virtual void Serialize(Serializer* pSerializer);
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() { return SerializeTypes::PhysXShapeSphere; };

private:
    NxSphereShapeDesc* m_pSphereDesc;
    
    //disable
    PhysXSphere(const PhysXSphere& other);
    PhysXSphere& operator=(const PhysXSphere& other);
};

