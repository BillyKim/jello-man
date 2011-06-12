#include "Softbody.h"
#include "CookingStream.h"
#include "ContentManager.h"

Softbody::Softbody(): m_pSoftbody(0), m_pPhysX(0), m_numPositions(0), m_numIndices(0), m_Radius(0), m_CenterPoint(Vector3::Zero), m_Dimension(Vector3::Zero)
{
}

Softbody::~Softbody(void)
{
    NxSoftBodyMesh* mesh = m_pSoftbody->getSoftBodyMesh();
    m_pPhysX->GetScene()->releaseSoftBody(*m_pSoftbody);
    m_pPhysX->GetSDK()->releaseSoftBodyMesh(*mesh);
}


void Softbody::InitSoftbody(PhysX* pPhysX, SoftbodyMesh* pSoftbodyMesh, const tstring& path, const Vector3& pos)
{
	ASSERT(m_pSoftbody == 0, "Eeek softbody is already initialized");
    ASSERT(pSoftbodyMesh != 0, "");

	m_pPhysX = pPhysX;

    m_pSoftbodyMesh = pSoftbodyMesh;

    NxU32 maxTetra = *max_element(m_pSoftbodyMesh->GetTetra().cbegin(), m_pSoftbodyMesh->GetTetra().cend()) + 1;
    m_vecPositions.resize(m_pSoftbodyMesh->GetVertices().size());
    m_vecIndices.resize(maxTetra*4);

    NxMeshData meshData;
    meshData.verticesPosBegin = &m_vecPositions[0];
    meshData.verticesPosByteStride = sizeof(Vector3);
    meshData.maxVertices = m_pSoftbodyMesh->GetVertices().size();
    meshData.numVerticesPtr = &m_numPositions;
    meshData.indicesBegin = &m_vecIndices[0];
    meshData.indicesByteStride = sizeof(NxU32);
    meshData.maxIndices = maxTetra * 4;
    meshData.numIndicesPtr = &m_numIndices;

	//Make Actor
	NxSoftBodyDesc softbodyDesc;
	
	string str = string(path.begin(), path.end());
	UserStream stream(UserStream(str.c_str(), true));
    pPhysX->GetPhysXLock().lock();
	NxSoftBodyMesh* pMesh = m_pPhysX->GetSDK()->createSoftBodyMesh(stream);
    pPhysX->GetPhysXLock().unlock();
	softbodyDesc.softBodyMesh = pMesh;
    softbodyDesc.volumeStiffness = 0.8f;
    softbodyDesc.stretchingStiffness = 0.6f;
    softbodyDesc.friction = 1.0f;
    softbodyDesc.solverIterations = 8;
    softbodyDesc.flags = NX_SBF_GRAVITY | NX_SBF_VOLUME_CONSERVATION;
    softbodyDesc.meshData = meshData;

    softbodyDesc.globalPose = static_cast<NxMat34>(Matrix::CreateTranslation(pos));

	m_pSoftbody = m_pPhysX->GetScene()->createSoftBody(softbodyDesc);
    ASSERT(m_pSoftbody != 0, "");

    NxBounds3 b;
    m_pSoftbody->getWorldBounds(b);

    NxVec3 center, dim;
    b.getCenter(center);
    b.getDimensions(dim);

    m_CenterPoint = Vector3(center);
    m_Dimension = Vector3(dim);
    m_Radius = m_Dimension.Length() / 2.0f;

}

void Softbody::TransformPositions()
{
    int size = m_pSoftbodyMesh->GetVertices().size();

    const vector<VertexPosNormTanTex>& vert = m_pSoftbodyMesh->GetVertices();
    const vector<DWORD>& tetra = m_pSoftbodyMesh->GetTetra();
    const vector<Vector3>& bc = m_pSoftbodyMesh->GetBaryCentricCoords();

    vector<VertexPosNormTanTex> newVert;
    newVert.reserve(size);

    Vector3 maxP = Vector3(FLT_MIN, FLT_MIN, FLT_MIN), 
            minP = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

    for (int i = 0; i < size; ++i)
    {
        Vector3 position =	m_vecPositions[m_vecIndices[tetra[i] * 4 + 0]] * bc[i].X + 
						    m_vecPositions[m_vecIndices[tetra[i] * 4 + 1]] * bc[i].Y + 
						    m_vecPositions[m_vecIndices[tetra[i] * 4 + 2]] * bc[i].Z + 
						    m_vecPositions[m_vecIndices[tetra[i] * 4 + 3]] * (1.0f - bc[i].X - bc[i].Y - bc[i].Z);

        maxP = Max(maxP, position);
        minP = Min(minP, position);

        m_CenterPoint += position;

        newVert.push_back(VertexPosNormTanTex(position, Vector3(), Vector3(), vert[i].tex));
    }

    m_CenterPoint /= static_cast<float>(size);
    m_Dimension = maxP - minP;
    ASSERT(m_Dimension.X >= 0 && m_Dimension.Y >= 0 && m_Dimension.Z >= 0, "Softbody dimension == 0");
    m_Radius = m_Dimension.Length() / 2.0f;

    const vector<DWORD>& ind = m_pSoftbodyMesh->GetIndices();
    DWORD numTris = ind.size() / 3;
    for (DWORD i = 0; i < numTris; ++i)
    {
        Vector3 normal;

        Vector3 pos1 = newVert[ind[i * 3]].position, 
                pos2 = newVert[ind[i * 3 + 1]].position, 
                pos3 = newVert[ind[i * 3 + 2]].position;
        Vector3 v1 = pos2 - pos1;
        Vector3 v2 = pos3 - pos1;
        v1.Normalize();
        v2.Normalize();
        Vector3 cross = v1.Cross(v2);
        cross.Normalize();

        if (newVert[ind[i * 3 + 0]].normal == Vector3::Zero)
            newVert[ind[i * 3 + 0]].normal = cross;
        else
        {
            newVert[ind[i * 3 + 0]].normal += cross;
            newVert[ind[i * 3 + 0]].normal.Normalize();
        }

        if (newVert[ind[i * 3 + 1]].normal == Vector3::Zero)
            newVert[ind[i * 3 + 1]].normal = cross;
        else
        {
            newVert[ind[i * 3 + 1]].normal += cross;
            newVert[ind[i * 3 + 1]].normal.Normalize();
        }

        if (newVert[ind[i * 3 + 2]].normal == Vector3::Zero)
            newVert[ind[i * 3 + 2]].normal = cross;
        else
        {
            newVert[ind[i * 3 + 2]].normal += cross;
            newVert[ind[i * 3 + 2]].normal.Normalize();
        }

    }

	m_TransformedVertices = newVert;
}
void Softbody::SetVertices()
{    
	if (m_TransformedVertices.size() != 0)
		m_pSoftbodyMesh->SetVertices(m_TransformedVertices);
}

void Softbody::AddSpeed(const Vector3& speed)
{
    m_pSoftbody->addDirectedForceAtPos(m_CenterPoint, speed, m_Radius, NX_VELOCITY_CHANGE);
}
void Softbody::AddSpeed(const Vector3& speed, const Vector3& pos)
{
    m_pSoftbody->addDirectedForceAtPos(pos, speed, m_Radius, NX_VELOCITY_CHANGE);
}
void Softbody::AddForce(const Vector3& force)
{
	m_pSoftbody->addDirectedForceAtPos(m_CenterPoint, force, m_Radius, NX_FORCE);
}
void Softbody::AddForce(const Vector3& force, const Vector3& pos)
{
	m_pSoftbody->addDirectedForceAtPos(pos, force, m_Radius, NX_FORCE);
}

Vector3 Softbody::GetSpeed() const
{
    return Vector3(m_pSoftbody->getVelocity(0));
}

void Softbody::Translate(const Vector3& add)
{
    if (m_numPositions == 0)
    {
        PANIC("Trying to move softbody with 0 positions");
        return;
    }
    vector<Vector3> buffer;
    buffer.resize(m_numPositions);
    m_pSoftbody->getPositions(&buffer[0]);

    for_each(buffer.begin(), buffer.end(), [&](Vector3& pos)
    {
        pos += add;
    });

    m_pSoftbody->setPositions(&buffer[0]);

    m_CenterPoint += add;
}
void Softbody::SetPosition(const Vector3& newPos)
{
    if (m_numPositions == 0)
    {
        PANIC("Trying to move softbody with 0 positions");
        return;
    }
    vector<Vector3> buffer;
    buffer.resize(m_numPositions);
    m_pSoftbody->getPositions(&buffer[0]);

    for_each(buffer.begin(), buffer.end(), [&](Vector3& pos)
    {
        pos += -GetPosition() + newPos;
    });

    m_pSoftbody->setPositions(&buffer[0]);

    m_CenterPoint = newPos;
}
Vector3 Softbody::GetPosition() const
{
    return m_CenterPoint;
}

void Softbody::Rotate(const Vector3& axis, float angle)
{    
    if (m_numPositions == 0)
    {
        PANIC("Trying to rotate softbody with 0 positions");
        return;
    }

    vector<Vector3> buffer;
    buffer.resize(m_numPositions);
    m_pSoftbody->getPositions(&buffer[0]);

    Matrix mtxRot = Matrix::CreateRotation(axis, angle);
    for_each(buffer.begin(), buffer.end(), [&](Vector3& pos)
    {
        pos -= GetPosition();
        pos = Vector3::Transform(pos, mtxRot).XYZ();
        pos += GetPosition();
    });

    m_pSoftbody->setPositions(&buffer[0]);
}

void Softbody::Scale(const Vector3& scale)
{
    if (m_numPositions == 0)
    {
        PANIC("Trying to rotate softbody with 0 positions");
        return;
    }

    vector<Vector3> buffer;
    buffer.resize(m_numPositions);
    m_pSoftbody->getPositions(&buffer[0]);

    Matrix mtxScale = Matrix::CreateScale(scale);
    for_each(buffer.begin(), buffer.end(), [&](Vector3& pos)
    {
        pos -= GetPosition();
        pos = Vector3::Transform(pos, mtxScale).XYZ();
        pos += GetPosition();
    });

    m_pSoftbody->setPositions(&buffer[0]);
}
