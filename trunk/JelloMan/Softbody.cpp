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
    softbodyDesc.flags = NX_SBF_GRAVITY | NX_SBF_VOLUME_CONSERVATION | NX_SBF_COLLISION_TWOWAY;
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

#pragma region normal
    const vector<DWORD>& indices = m_pSoftbodyMesh->GetIndices();
    vector<VertexPosNormTanTex>& vertices = newVert;
    DWORD numTris = indices.size() / 3;
    for (DWORD i = 0; i < numTris; ++i)
    {
        Vector3 normal;

        const Vector3& pos1 = vertices[indices[i * 3]].position, 
                       pos2 = vertices[indices[i * 3 + 1]].position, 
                       pos3 = vertices[indices[i * 3 + 2]].position;

        Vector3 v1 = pos2 - pos1;
        Vector3 v2 = pos3 - pos1;
        v1.Normalize();
        v2.Normalize();
        Vector3 cross = v1.Cross(v2);
        cross.Normalize();

        if (vertices[indices[i * 3 + 0]].normal == Vector3::Zero)
            vertices[indices[i * 3 + 0]].normal = cross;
        else
        {
            vertices[indices[i * 3 + 0]].normal += cross;
            vertices[indices[i * 3 + 0]].normal.Normalize();
        }

        if (vertices[indices[i * 3 + 1]].normal == Vector3::Zero)
            vertices[indices[i * 3 + 1]].normal = cross;
        else
        {
            vertices[indices[i * 3 + 1]].normal += cross;
            vertices[indices[i * 3 + 1]].normal.Normalize();
        }

        if (vertices[indices[i * 3 + 2]].normal == Vector3::Zero)
            vertices[indices[i * 3 + 2]].normal = cross;
        else
        {
            vertices[indices[i * 3 + 2]].normal += cross;
            vertices[indices[i * 3 + 2]].normal.Normalize();
        }
    }
#pragma endregion
#pragma region tangent
    vector<VertexPosNormTanTex>& vecVPNTData = newVert;
    const vector<DWORD>& vecIndexData = indices;
    Vector3* tan1 = new Vector3[vecVPNTData.size()];
    Vector3* tan2 = new Vector3[vecVPNTData.size()];
    ZeroMemory(tan1, vecVPNTData.size() * 12);
    ZeroMemory(tan2, vecVPNTData.size() * 12);
    for (DWORD i = 0; i < vecIndexData.size(); i += 3)
    {
        int i1 = vecIndexData[i], 
            i2 = vecIndexData[i+1], 
            i3 = vecIndexData[i+2];

	    const Vector3& v1 = vecVPNTData[i1].position;
	    const Vector3& v2 = vecVPNTData[i2].position;
	    const Vector3& v3 = vecVPNTData[i3].position;

	    const Vector2& tx1 = vecVPNTData[i1].tex;
	    const Vector2& tx2 = vecVPNTData[i2].tex;
	    const Vector2& tx3 = vecVPNTData[i3].tex;

	    float x1 = v2.X - v1.X;
	    float x2 = v3.X - v1.X;
	    float y1 = v2.Y - v1.Y;
	    float y2 = v3.Y - v1.Y;
	    float z1 = v2.Z - v1.Z;
	    float z2 = v3.Z - v1.Z;

	    float s1 = tx2.X - tx1.X;
	    float s2 = tx3.X - tx1.X;
	    float t1 = tx2.Y - tx1.Y;
	    float t2 = tx3.Y - tx1.Y;

	    float r = 1.0f / (s1 * t2 - s2 * t1);

	    Vector3 sdir(
            (t2 * x1 - t1 * x2) * r, 
            (t2 * y1 - t1 * y2) * r, 
            (t2 * z1 - t1 * z2) * r );
	    Vector3 tdir(
            (s1 * x2 - s2 * x1) * r, 
            (s1 * y2 - s2 * y1) * r, 
            (s1 * z2 - s2 * z1) * r );

        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;

        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }

    for(DWORD i = 0; i < vecVPNTData.size(); ++i)
    {
        const Vector3& n = vecVPNTData[i].normal;
        const Vector3& t = tan1[i];

        vecVPNTData[i].tangent = (t - n * n.Dot(t));
        vecVPNTData[i].tangent.Normalize();

        if (vecVPNTData[i].tangent.Dot(vecVPNTData[i].normal) > 0.0001f)
            PANIC("Tangent not loodrecht op Normal");
    }


    delete[] tan1;
    delete[] tan2;
#pragma endregion

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
