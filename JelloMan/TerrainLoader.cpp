#include "TerrainLoader.h"
#include "Texture2D.h"
#include "ContentManager.h"

//------Constructor-Destructor------->
TerrainLoader::TerrainLoader(void) : m_pAssets(new AssetContainer<Model<VertexPosNormTanTex>>())
{
}
TerrainLoader::~TerrainLoader(void)
{
    delete m_pAssets;
}
//<-----------------------------------

Model<VertexPosNormTanTex>* TerrainLoader::Load(ID3D10Device* pDXDevice, const tstring& key)
{
    Model<VertexPosNormTanTex>* ret = 0;
	if ( m_pAssets->IsAssetPresent(key))
	{
        ret = m_pAssets->GetAsset(key);
	}
	else
	{
        vector<VertexPosNormTanTex> vertices;
        vector<DWORD> indices;

        Texture2D* heightMap = Content->LoadTexture2D(key, true);
        ID3D10Texture2D* pTex2D = heightMap->GetTextureResource();
        
        vertices.reserve(heightMap->GetWidth() * heightMap->GetHeight());
        indices.reserve((heightMap->GetWidth()-1) * (heightMap->GetHeight()-1) * 6);

        D3D10_MAPPED_TEXTURE2D pMappedTex2D;
        pTex2D->Map(0, D3D10_MAP_READ, 0, &pMappedTex2D);

        float heightMult = 1.0f;
        float planarMult = 1.0f / max(heightMap->GetWidth(), heightMap->GetHeight());
        BYTE* pData = static_cast<BYTE*>(pMappedTex2D.pData);
        for (UINT x = 0; x < heightMap->GetHeight(); ++x)
        {
            for (UINT z = 0; z < heightMap->GetWidth(); ++z)
            {
                float height = pData[z * 4 + x * pMappedTex2D.RowPitch + 0] / 255.0f; //get red

                vertices.push_back(VertexPosNormTanTex(
                    static_cast<float>(x) * planarMult, height * heightMult, static_cast<float>(z) * planarMult,
                    0, 0, 0, 
                    0, 0, 0, 
                    static_cast<float>(z) / heightMap->GetWidth(), static_cast<float>(x) / heightMap->GetHeight()));

                if (z != heightMap->GetWidth() - 1 && x != heightMap->GetHeight() - 1)
                {
                    indices.push_back(z + x * heightMap->GetWidth());
                    indices.push_back(z + x * heightMap->GetWidth() + 1);
                    indices.push_back(z + (x + 1) * heightMap->GetWidth());

                    indices.push_back(z + x * heightMap->GetWidth() + 1);
                    indices.push_back(z + (x + 1) * heightMap->GetWidth() + 1);
                    indices.push_back(z + (x + 1) * heightMap->GetWidth());
                }
            }
        }

        pTex2D->Unmap(0);

        CalculateNormals(vertices, indices);
        CalculateTangents(vertices, indices);

        Model<VertexPosNormTanTex>* pModel = new Model<VertexPosNormTanTex>(pDXDevice);
        ModelMesh<VertexPosNormTanTex>* pMesh = pModel->AddMesh(_T(""));

        pMesh->SetVertices(vertices);
        pMesh->SetIndices(indices);

        m_pAssets->AddAsset(key, pModel);
        ret = pModel;
    }

    return ret;
}
void CalculateNormals(vector<VertexPosNormTanTex>& vertices, const vector<DWORD>& indices)
{
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

        /*vertices[indices[i * 3 + 0]].normal += cross;
        vertices[indices[i * 3 + 1]].normal += cross;
        vertices[indices[i * 3 + 2]].normal += cross;*/

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
    /*for_each(vertices.begin(), vertices.end(), [](VertexPosNormTanTex& vpnt)
    {
        vpnt.normal.Normalize();
    });*/
}
void CalculateTangents(vector<VertexPosNormTanTex>& vecVPNTData, const vector<DWORD>& vecIndexData)
{
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
}