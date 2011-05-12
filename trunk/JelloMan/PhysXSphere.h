#pragma once
#include "PhysXShape.h"

class PhysXSphere : public PhysXShape
{
public:
    PhysXSphere(float radius, float mass);
    PhysXSphere();
    virtual ~PhysXSphere(void);
    
    virtual NxShapeDesc* GetShape() const { return m_pSphereDesc; }

	virtual void Scale(const Vector3& scale);
    
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::PhysXShapeSphere; };

private:
    NxSphereShapeDesc* m_pSphereDesc;
    
    //disable
    PhysXSphere(const PhysXSphere& other);
    PhysXSphere& operator=(const PhysXSphere& other);
};

