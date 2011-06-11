#pragma once
#include "PhysXShape.h"
#include "Vector3.h"

class PhysX;

class PhysXTerrain : public PhysXShape
{
public:
    PhysXTerrain();
    PhysXTerrain(PhysX* pPhysX, const tstring& path, float planarScale, float heightScale);
    virtual ~PhysXTerrain(void);


    virtual NxShapeDesc* GetShape() const { return m_pHeightShapeDesc; }
	
	virtual void Scale(const Vector3& /*scale*/) {};

	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::PhysXShapeBox; };
    
    virtual PhysXShape* Copy() const;

private:
    void Init();

    NxHeightFieldShapeDesc* m_pHeightShapeDesc;
    PhysX* m_pPhysX;
    tstring m_Path;
    float m_PlanarScale;
    float m_HeightScale;


    //disable
    PhysXTerrain(const PhysXTerrain& other);
    PhysXTerrain& operator=(const PhysXTerrain& other);
};

