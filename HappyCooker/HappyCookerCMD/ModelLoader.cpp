#include "ModelLoader.h"

ModelLoader::ModelLoader(void): 
            m_pCurrentModel(0), 
            m_pCurrentPhysXMesh(0),
            m_pCurrentDrawMesh(0),
            m_pProgress(0)
{
    m_pProgress = new ProgressInfo();
}


ModelLoader::~ModelLoader(void)
{
    delete m_pProgress;
}

Model* ModelLoader::Load(const string& assetName)
{
    m_pCurrentModel = new Model();
    m_VertexData.clear();
    m_NormalData.clear();
    m_TextureData.clear();
    
    FILE* f;
    if (fopen_s(&f, assetName.c_str(), "r") != 0)
    {
        cout << "File Open Failed";
        return 0;
    }
    fseek(f, 0, SEEK_END);
    int end = ftell(f);

    fclose(f);

    m_pProgress->SetMax(end);

    ReadObj(assetName); //assigns all meshes

    return m_pCurrentModel;
}

void ModelLoader::ReadObj(const string& assetName)
{
    ifstream stream;
    stream.open(assetName.c_str());
    if (stream.fail())
    {
        cout << "File open fail: '" << assetName << "'\n";
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
        m_pProgress->AddTick(line.size());
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
            char s[40];
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
void ModelLoader::AddMesh(const string& name)
{
    if (m_pCurrentDrawMesh != 0 || m_pCurrentPhysXMesh != 0) //apply the vertex and indexbuffer to the mesh
        FlushMesh();

    if (name.find("PhysX_") == 0)
        m_pCurrentPhysXMesh = m_pCurrentModel->AddPhysXMesh(name);
    else
        m_pCurrentDrawMesh  = m_pCurrentModel->AddDrawMesh(name);
}
void ModelLoader::AddTri(const vector<vector<int>>& data)
{
    unsigned int index[3];
    if (m_pCurrentDrawMesh != 0)
    {
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
    }
    else
    {  
        for (int i = 0; i < 3; ++i)
        {
            index[i] = m_VPData.size();
            VertexPos vp(
                    m_VertexData[data[i][0] - 1]
                        );

            stringstream stream;
            stream << data[i][0]; //only position matters
            if (m_VPMap.count(stream.str()) == 0)
            {
                m_VPData.push_back(vp);
                m_VPMap[stream.str()] = index[i];
            }
            else
            {
                index[i] = m_VPMap[stream.str()];
            }   
        }
    }
    
    m_IndexData.push_back(index[2]);
    m_IndexData.push_back(index[1]);
    m_IndexData.push_back(index[0]);
}

void ModelLoader::FlushMesh()
{
    if (m_pCurrentDrawMesh != 0)
    {
        m_pCurrentDrawMesh->SetVertices(m_VPNTData);
        m_pCurrentDrawMesh->SetIndices(m_IndexData);
        m_VPNTData.clear();
        m_VPNTMap.clear();
        m_pCurrentDrawMesh = 0;
    }
    else if (m_pCurrentPhysXMesh != 0)
    {
        m_pCurrentPhysXMesh->SetVertices(m_VPData);
        m_pCurrentPhysXMesh->SetIndices(m_IndexData);
        m_VPData.clear();
        m_VPMap.clear();
        m_pCurrentPhysXMesh = 0;
    }
    m_IndexData.clear();
}