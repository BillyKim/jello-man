#include "PhysXMesh.h"
#include "CookingStream.h"

PhysXMesh::PhysXMesh(PhysX* physX, const string& path, float mass): m_pShapeDesc(0)
{
    if (path.rfind(".nxconvex") != -1)
    {
        NxConvexShapeDesc* desc = new NxConvexShapeDesc();
        desc->mass = mass;
        desc->meshData = physX->GetSDK()->createConvexMesh(UserStream(path.c_str(), true));
        m_pShapeDesc = desc;
    }
    else if (path.rfind(".nxconcave") != -1)
    {
        NxTriangleMeshShapeDesc* desc = new NxTriangleMeshShapeDesc();
        desc->mass = mass;
        desc->meshData = physX->GetSDK()->createTriangleMesh(UserStream(path.c_str(), true));
        m_pShapeDesc = desc;
    }
    else
    {
        ASSERT("Wrong extension");
    }
}

PhysXMesh::~PhysXMesh(void)
{
    delete m_pShapeDesc;
}
