#include "PhysXTerrain.h"
#include "Model.h"
#include "ContentManager.h"

PhysXTerrain::PhysXTerrain(): 
    m_HeightScale(1.0f), m_PlanarScale(0.0f),
    m_Path(_T("")), m_pHeightShapeDesc(0),
    m_pPhysX(0)
{
}
PhysXTerrain::PhysXTerrain(PhysX* pPhysX, const tstring& path, float planarScale, float heightScale):
        m_pPhysX(pPhysX),
        m_Path(path),
        m_PlanarScale(planarScale),
        m_HeightScale(heightScale), 
        m_pHeightShapeDesc(0)
{
    Init();
}

PhysXTerrain::~PhysXTerrain(void)
{
    m_pPhysX->GetSDK()->releaseHeightField(*m_pHeightShapeDesc->heightField);
    delete m_pHeightShapeDesc;
}

void PhysXTerrain::Init()
{
    Model<VertexPosNormTanTex>* pModel = Content->LoadTerrain(m_Path);
    Texture2D* pTexTemp = Content->LoadTexture2D(m_Path, true);
    
    vector<NxHeightFieldSample> samples;
    samples.reserve(pTexTemp->GetWidth() * pTexTemp->GetHeight());
    for_each(pModel->GetModelMeshes()[0]->GetVertices().cbegin(), pModel->GetModelMeshes()[0]->GetVertices().cend(),
        [&](const VertexPosNormTanTex& vpnt)
    {
        NxHeightFieldSample s;
        s.height = static_cast<NxI16>(vpnt.position.Y * NX_MAX_I16);
        s.tessFlag = 0;
        //s.materialIndex0 = 0;
        //s.materialIndex1 = 0;
        //s.unused = 0;
        samples.push_back(s);
    });

    NxHeightFieldDesc desc;
    desc.convexEdgeThreshold = 0;
    desc.nbColumns = pTexTemp->GetWidth();
    desc.nbRows = pTexTemp->GetHeight();
    desc.samples = &samples[0];
    desc.sampleStride = sizeof(NxHeightFieldSample);
    desc.thickness = -1.0f;

    NxHeightField* pHeightField = m_pPhysX->GetSDK()->createHeightField(desc);
    ASSERT(pHeightField != 0, "heightfield creation failed");


    delete m_pHeightShapeDesc;
    m_pHeightShapeDesc = new NxHeightFieldShapeDesc();

    m_pHeightShapeDesc->heightField = pHeightField;    
    m_pHeightShapeDesc->heightScale = m_HeightScale / NX_MAX_I16;    
    m_pHeightShapeDesc->rowScale = m_PlanarScale / max(pTexTemp->GetWidth(), pTexTemp->GetHeight());    
    m_pHeightShapeDesc->columnScale = m_PlanarScale / max(pTexTemp->GetWidth(), pTexTemp->GetHeight());    
    m_pHeightShapeDesc->materialIndexHighBits = 0;    
    m_pHeightShapeDesc->holeMaterial = 0;
}

PhysXShape* PhysXTerrain::Copy() const
{
    /*PhysXBox* b = new PhysXBox();
    b->m_pBoxShapeDesc = new NxBoxShapeDesc();
    b->m_pBoxShapeDesc->dimensions = m_pBoxShapeDesc->dimensions;
    b->m_pBoxShapeDesc->mass = m_pBoxShapeDesc->mass;*/

    return 0;
}


void PhysXTerrain::Serialize(Serializer* pSerializer) const
{
    /*pSerializer->GetStream()->storeVector3(Vector3(m_pBoxShapeDesc->dimensions));
    pSerializer->GetStream()->storeFloat(m_pBoxShapeDesc->mass);*/
}
void PhysXTerrain::Deserialize(Serializer* pSerializer)
{/*
    SafeDelete(m_pBoxShapeDesc);

    m_pBoxShapeDesc = new NxBoxShapeDesc();
    m_pBoxShapeDesc->dimensions = static_cast<NxVec3>(pSerializer->GetStream()->readVector3());
    m_pBoxShapeDesc->mass = pSerializer->GetStream()->readFloat();*/
}