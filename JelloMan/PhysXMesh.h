#pragma once
#include "PhysXShape.h"
#include "Model.h"
#include "PhysX.h"

class PhysXMesh : public PhysXShape
{
public:
    PhysXMesh();
    PhysXMesh(PhysX* physX, const string& path, float mass);

    virtual ~PhysXMesh(void);

    virtual NxShapeDesc* GetShape() const { return m_pShapeDesc; }
  
	virtual void Serialize(Serializer* pSerializer);
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() { return SerializeTypes::PhysXShapeMesh; };

private:
    void Init(const string& path, float mass);

    NxShapeDesc* m_pShapeDesc;
    PhysX* m_pPhysX;
    string m_Path;
    bool m_isConvex;
    
    //disable
    PhysXMesh(const PhysXMesh& other);
    PhysXMesh& operator=(const PhysXMesh& other);
};

