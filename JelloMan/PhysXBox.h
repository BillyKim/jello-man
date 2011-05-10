#pragma once
#include "PhysXShape.h"
#include "Vector3.h"

class PhysXBox : public PhysXShape
{
public:
    PhysXBox();
    PhysXBox(const Vector3& dimension, float mass);
    virtual ~PhysXBox(void);

    virtual NxShapeDesc* GetShape() const { return m_pBoxShapeDesc; }

	virtual void Serialize(Serializer* pSerializer);
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() { return SerializeTypes::PhysXShapeBox; };

private:
    NxBoxShapeDesc* m_pBoxShapeDesc;

    //disable
    PhysXBox(const PhysXBox& other);
    PhysXBox& operator=(const PhysXBox& other);
};

