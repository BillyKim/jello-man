#include "ModelLoader.h"

ModelLoader::ModelLoader(void): m_pCurrentModel(0), m_pCurrentMesh(0)
{
}


ModelLoader::~ModelLoader(void)
{
	AssetContainer::~AssetContainer();
}

Model<VertexPosNormTex>* ModelLoader::Load(ID3D10Device *pDXDevice, const tstring& assetName)
{
    if (IsAssetPresent(assetName))
    {
        return GetAsset(assetName);
    }
    else
    {
        m_pCurrentModel = new Model<VertexPosNormTex>(pDXDevice);
        m_VertexData.clear();
        m_NormalData.clear();
        m_TextureData.clear();

        ReadObj(assetName); //assigns all meshes

        //Effect?

        AddAsset(assetName, m_pCurrentModel);
        return m_pCurrentModel;
    }
}

void ModelLoader::ReadObj(const tstring& assetName)
{
    tifstream stream;
    stream.open(assetName.c_str());

    tstring line;
    
    vector<vector<int>> faceData(3);
    for (int i = 0; i < 3; ++i)
        faceData[i].reserve(3);
    while (stream.eof() == false)
    {
        getline(stream, line);
        if (line.find(_T("v"), 0) == 0) //v is 0'd char
        {
            Vector3 v;
            swscanf_s(line.c_str(), _T("v %f %f %f"), &v.X, &v.Y, &v.Z);
            AddVertex(v);
        }
        else if (line.find(_T("vn"), 0) == 0)
        {
            Vector3 v;
            swscanf_s(line.c_str(), _T("vn %f %f %f"), &v.X, &v.Y, &v.Z);
            AddNormal(v);
        }
        else if (line.find(_T("vt"), 0) == 0)
        {
            Vector2 v;
            swscanf_s(line.c_str(), _T("vt %f %f "), &v.X, &v.Y);
            AddTexCoord(v);
        }
        else if (line.find(_T("f"), 0) == 0)
        {
            swscanf_s(line.c_str(), _T("f %d/%d/%d %d/%d/%d %d/%d/%d"), 
                                          &faceData[0][0], &faceData[0][1], &faceData[0][2],
                                          &faceData[1][0], &faceData[1][1], &faceData[1][2],
                                          &faceData[2][0], &faceData[2][1], &faceData[2][2]);
            AddTri(faceData);
        }
        else if (line.find(_T("g"), 0) == 0)
        {
            tstring s;
            swscanf_s(line.c_str(), _T("g %s"), s);
            AddMesh(s);
        }
    }
    FlushMesh(); //apply last mesh

    stream.close();
}

void ModelLoader::AddVertex(const Vector3& v)
{
    m_VertexData.push_back(v);
}
void ModelLoader::AddNormal(const Vector3& v)
{
    m_NormalData.push_back(v);
}
void ModelLoader::AddTexCoord(const Vector2& v)
{
    m_TextureData.push_back(v);
}
void ModelLoader::AddMesh(const tstring& name)
{
    if (m_pCurrentMesh != 0) //apply the vertex and indexbuffer to the mesh
        FlushMesh();

    m_pCurrentMesh = m_pCurrentModel->AddMesh(name);
}
void ModelLoader::AddTri(const vector<vector<int>>& data)
{
    int index = m_VPNTData.size();
    for (int i = 0; i < 3; ++i)
        m_VPNTData.push_back(
            VertexPosNormTex(
                m_VertexData[data[i][0] - 1],
                m_NormalData[data[i][2] - 1],
               m_TextureData[data[i][1] - 1]));
    
    m_IndexData.push_back(index + 2);
    m_IndexData.push_back(index + 1);
    m_IndexData.push_back(index + 0);
}

void ModelLoader::FlushMesh()
{
    if (m_pCurrentMesh == 0)
        return;
    m_pCurrentMesh->SetVertices(m_VPNTData);
    m_pCurrentMesh->SetIndices(m_IndexData);

    m_pCurrentMesh = 0;
    m_VPNTData.clear();
    m_IndexData.clear();
}