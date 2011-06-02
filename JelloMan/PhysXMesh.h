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
  
	virtual void Scale(const Vector3& /*scale*/) {};

	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::PhysXShapeMesh; };
    
    virtual PhysXShape* Copy();

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

