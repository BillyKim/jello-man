#include "PhysXMesh.h"
#include "CookingStream.h"


PhysXMesh::PhysXMesh(): m_pShapeDesc(0), m_pPhysX(0), m_isConvex(false), m_Path("")
{
}
PhysXMesh::PhysXMesh(PhysX* physX, const string& path, float mass): m_pShapeDesc(0), m_pPhysX(physX), m_isConvex(false), m_Path(path)
{
    Init(path, mass);
}

PhysXMesh::~PhysXMesh(void)
{
    //WARNING MEMORYLEAK
    //meshdata should be released
    delete m_pShapeDesc;
}

PhysXShape* PhysXMesh::Copy() const
{
    PhysXMesh* m = new PhysXMesh(m_pPhysX, m_Path, m_pShapeDesc->mass);
    return m;
}

void PhysXMesh::Init(const string& path, float mass)
{
    m_Path = path;

    if (path.rfind(".nxconvex") != -1)
    {
        NxConvexShapeDesc* desc = new NxConvexShapeDesc();
        desc->mass = mass;
        desc->meshData = m_pPhysX->GetSDK()->createConvexMesh(UserStream(path.c_str(), true)); //mem leak
        m_pShapeDesc = desc;
        m_isConvex = true;
    }
    else if (path.rfind(".nxconcave") != -1)
    {
        NxTriangleMeshShapeDesc* desc = new NxTriangleMeshShapeDesc();
        desc->mass = mass;
        desc->meshData = m_pPhysX->GetSDK()->createTriangleMesh(UserStream(path.c_str(), true)); //mem leak
        m_pShapeDesc = desc;
        m_isConvex = false;
    }
    else
    {
        ASSERT(false, "Wrong extension");
    }
}

void PhysXMesh::Serialize(Serializer* pSerializer) const
{
    pSerializer->GetStream()->storeString(m_Path);
    pSerializer->GetStream()->storeFloat(m_pShapeDesc->mass);
}

void PhysXMesh::Deserialize(Serializer* pSerializer)
{
    SafeDelete(m_pShapeDesc);

    m_Path = pSerializer->GetStream()->readString();

    m_pPhysX = pSerializer->GetPhysX();
    Init(m_Path, pSerializer->GetStream()->readFloat());
}
