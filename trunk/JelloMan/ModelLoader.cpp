#include "ModelLoader.h"
#include "CookingStream.h"

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

        if (assetName.rfind(_T(".obj")) != -1)
            ReadASCIIObj(assetName); //assigns all meshes
        else if (assetName.rfind(_T(".binobj")) != -1)
            ReadBinObj(assetName);

        AddAsset(assetName, m_pCurrentModel);
        return m_pCurrentModel;
    }
}
void ModelLoader::ReadBinObj(const tstring& assetName)
{
    //LAYOUT
    //DWord : #meshes
        //Word: nameLength
            //buffer: name
        //DWord : #vert
            //vector3: pos, vector3: norm, vector2: tex     : vertex
        //DWord : #triangles
            //DWord, DWord, DWord : triangle
        //...


    string name(assetName.cbegin(), assetName.cend());
    UserStream stream(name.c_str(), true);

    DWORD meshes = stream.readDword();
    for (DWORD m = 0; m < meshes; ++m)
    {
        WORD nameLength = stream.readWord();
        char* name = new char[nameLength];
        stream.readBuffer(name, nameLength);
        

        string tempname(name);
        delete name;
        m_pCurrentMesh = m_pCurrentModel->AddMesh(tstring(tempname.cbegin(), tempname.cend()));


        m_VPNTData.clear();
        DWORD vertices = stream.readDword();
        for (DWORD v = 0; v < vertices; ++v)
        {
            Vector3 pos = stream.readVector3();
            Vector3 norm = stream.readVector3();
            Vector2 tex = stream.readVector2();
            m_VPNTData.push_back(VertexPosNormTex(pos, norm, tex));
        }

        DWORD indices = stream.readDword() * 3;
        m_IndexData.clear();
        for (DWORD i = 0; i < indices; ++i)
        {
            DWORD index = stream.readDword();
            m_IndexData.push_back(index);
        }
        m_pCurrentMesh->SetVertices(m_VPNTData);
        m_pCurrentMesh->SetIndices(m_IndexData);
    }
}

//*********************
//  ASCII
//******************
void ModelLoader::ReadASCIIObj(const tstring& assetName)
{
    ifstream stream;
    stream.open(assetName.c_str());
    if (stream.fail())
    {
        wcout << "File open fail: '" << assetName << "'\n";
        return;
    }

    string line;
    
    vector<vector<int>> faceData;
    for (int i = 0; i < 3; ++i)
    {
        faceData.push_back(vector<int>(3));
    }
    while (stream.eof() == false)
    {
        getline(stream, line);
        if (line.find("vn", 0) == 0)
        {
            Vector3 v;
            sscanf_s(line.c_str(), "vn %f %f %f", &v.X, &v.Y, &v.Z);
            AddNormal(v);
        }
        else if (line.find("vt", 0) == 0)
        {
            Vector2 v;
            sscanf_s(line.c_str(), "vt %f %f ", &v.X, &v.Y);
            AddTexCoord(v);
        }
        else if (line.find("f", 0) == 0)
        {
            sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                                          &faceData[0][0], &faceData[0][1], &faceData[0][2],
                                          &faceData[1][0], &faceData[1][1], &faceData[1][2],
                                          &faceData[2][0], &faceData[2][1], &faceData[2][2]);
            AddTri(faceData);
        }
        else if (line.find("g", 0) == 0)
        {
            wchar_t s[40];
            sscanf_s(line.c_str(), "g %s40", s, _countof(s));
            AddMesh(s);
        }
        else if (line.find("v", 0) == 0) //v is 0'd char
        {
            Vector3 v;
            sscanf_s(line.c_str(), "v %f %f %f", &v.X, &v.Y, &v.Z);
            AddVertex(v);
        }
    }
    FlushMesh(); //apply last mesh

    stream.close();
}

void ModelLoader::AddVertex(const Vector3& v)
{
	Vector3 copy = v;
	copy.Z *= -1;
    m_VertexData.push_back(copy);
}
void ModelLoader::AddNormal(const Vector3& v)
{
	Vector3 copy = v;
	copy.Z *= -1;
    m_NormalData.push_back(copy);
}
void ModelLoader::AddTexCoord(const Vector2& v)
{
	Vector2 copy = v;
	copy.Y *= -1;
    m_TextureData.push_back(v);
}
void ModelLoader::AddMesh(const tstring& name)
{
    if (m_pCurrentMesh != 0) //apply the vertex and indexbuffer to the mesh
        FlushMesh();

    m_pCurrentMesh  = m_pCurrentModel->AddMesh(name);
}
void ModelLoader::AddTri(const vector<vector<int>>& data)
{
    ASSERT (m_pCurrentMesh != 0);

    unsigned int index[3];
    for (int i = 0; i < 3; ++i)
    {
        index[i] = m_VPNTData.size();
        VertexPosNormTex vpnt(
                m_VertexData[data[i][0] - 1],
                m_NormalData[data[i][2] - 1],
                m_TextureData[data[i][1] - 1]);

        stringstream stream;
        stream << data[i][0] << "/" << data[i][1] << "/" << data[i][2];
        if (m_VPNTMap.count(stream.str()) == 0)
        {
            m_VPNTData.push_back(vpnt);
            m_VPNTMap[stream.str()] = index[i];
        }
        else
        {
            index[i] = m_VPNTMap[stream.str()];
        }   
    }
    
    m_IndexData.push_back(index[2]);
    m_IndexData.push_back(index[1]);
    m_IndexData.push_back(index[0]);
}

void ModelLoader::FlushMesh()
{
    ASSERT (m_pCurrentMesh != 0);
    m_pCurrentMesh->SetVertices(m_VPNTData);
    m_pCurrentMesh->SetIndices(m_IndexData);
    m_VPNTData.clear();
    m_VPNTMap.clear();
    m_pCurrentMesh = 0;
    m_IndexData.clear();
}