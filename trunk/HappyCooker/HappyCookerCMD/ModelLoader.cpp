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
    
    int end = 0;
    bool isSoftbody = false;
    string filename(assetName.substr(0, assetName.rfind(".")));
     
    FILE* f;
    if (fopen_s(&f, assetName.c_str(), "r") != 0)
    {
        cout << "Error: File Open Failed\n";
        return 0;
    }
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    fclose(f);
    if (fopen_s(&f, (filename + "_SB.tet").c_str(), "r") == 0)
    {
        cout << "Info: tet file found, loading softbody data\n";
        fseek(f, 0, SEEK_END);
        end += ftell(f);
        fclose(f);
        if (fopen_s(&f, (filename + "_SB.obj").c_str(), "r") == 0)
        {
            fseek(f, 0, SEEK_END);
            end += ftell(f);
            fclose(f);
            isSoftbody = true;
        }
        else
        {
            cout << "Error: " << filename << "_SB.obj" << "  not found!\n";
            return 0;
        }
    }
    m_pProgress->SetMax(end);

    m_pCurrentModel->SetIsSoftBody(isSoftbody);

    ReadObj(assetName); //assigns all draw/physXmeshes

    if (isSoftbody)
    {
        ReadTet(filename + "_SB.tet");
        LinkSBtoModel(filename + "_SB.obj");
    }

    return m_pCurrentModel;
}

#pragma region Read ASCII obj

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
void ModelLoader::AddMesh(const string& name)
{
    FlushMesh();

    if (name.find("PhysX_") == 0)
        m_pCurrentPhysXMesh = m_pCurrentModel->AddPhysXMesh(name);
    else if (m_pCurrentModel->IsSoftBody() == false)
        m_pCurrentDrawMesh = m_pCurrentModel->AddDrawMesh(name);
}
void ModelLoader::AddTri(const vector<vector<int>>& data)
{
    unsigned int index[3];
    if (m_pCurrentDrawMesh != 0 || m_pCurrentModel->IsSoftBody())
    {
        for (int i = 0; i < 3; ++i)
        {
            index[i] = m_VPNTData.size();
            VertexPosNormTanTex vpnt(
                    m_VertexData[data[i][0] - 1],
                    m_NormalData[data[i][2] - 1],
                    Vector3(0, 0, 0),
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
        if (m_pCurrentModel->IsSoftBody() == false)
        {
            CalculateTangents();
            m_pCurrentDrawMesh->SetVertices(m_VPNTData);
            m_pCurrentDrawMesh->SetIndices(m_IndexData);
            m_VPNTData.clear();
            m_pCurrentDrawMesh = 0;
            m_IndexData.clear();
        }
    }
    else if (m_pCurrentPhysXMesh != 0)
    {
        m_pCurrentPhysXMesh->SetVertices(m_VPData);
        m_pCurrentPhysXMesh->SetIndices(m_IndexData);
        m_VPData.clear();
        m_VPMap.clear();
        m_pCurrentPhysXMesh = 0;
        m_IndexData.clear();
    }
    if (m_pCurrentModel->IsSoftBody())
    {
        m_VPNTMap.clear();
    }
}

#pragma endregion

void ModelLoader::ReadTet(const string& fileName)
{
    ifstream stream;
    stream.open(fileName.c_str());
    if (stream.fail())
    {
        cout << "Error: file open fail: '" << fileName << "'\n";
        return;
    }

    m_SBLinkData.clear();
    m_TetraData.clear();

    string line;
    vector<VertexPos> vertices;
    while (stream.eof() == false)
    {
        getline(stream, line);
        m_pProgress->AddTick(line.size());
        
        if (line.find("v", 0) == 0)
        {
            Vector3 v;
            sscanf_s(line.c_str(), "v %f %f %f", &v.X, &v.Y, &v.Z);
              
            v.Z *= -1;
            vertices.push_back(VertexPos(v));
        }
        else if (line.find("t", 0) == 0)
        {
            int t1, t2, t3, t4;
            sscanf_s(line.c_str(), "t %d %d %d %d", &t1, &t2, &t3, &t4);
            AddTetra(t1, t2, t3, t4);
        }
        else if (line.find("l", 0) == 0)
        {
            int t;
            Vector3 bc;
            sscanf_s(line.c_str(), "l %d %f %f %f", &t, &bc.X, &bc.Y, &bc.Z);
            AddSBLink(t, bc);
        }
    }
    stream.close();

    ModelMesh<VertexPos>* mesh = m_pCurrentModel->SetSoftbodyMesh();
    mesh->SetVertices(vertices);
    mesh->SetIndices(m_TetraData);
}
void ModelLoader::LinkSBtoModel(const string& fileName)
{
    ifstream stream;
    stream.open(fileName.c_str());
    if (stream.fail())
    {
        cout << "Error: file open fail: '" << fileName << "'\n";
        return;
    }


    map<string, DWORD> sbLinkMap;

    string line;
    DWORD counter = 0;
    while (stream.eof() == false)
    {
        getline(stream, line);
        m_pProgress->AddTick(line.size());
        if (line.find("v", 0) == 0)
        {
            Vector3 v;
            sscanf_s(line.c_str(), "v %f %f %f", &v.X, &v.Y, &v.Z);
            v.Z *= -1;

            char buf[50];
            _snprintf_s(buf, 50, "%.5f %.5f %.5f", v.X, v.Y, v.Z);
            string sv(buf);

            sbLinkMap[sv] = counter;
            ++counter;
        }
    }
    stream.close();

    ModelMesh<VertexPNTSoftbody>* mesh = m_pCurrentModel->SetSoftbodyDrawMesh();
    vector<VertexPNTSoftbody> vertices;
    //vector<DWORD> softbodyLinkIndices;
    for (DWORD i = 0; i < m_VPNTData.size(); ++i)
    {
        char buf[50];
        _snprintf_s(buf, 50, "%.5f %.5f %.5f", m_VPNTData[i].position.X, m_VPNTData[i].position.Y, m_VPNTData[i].position.Z);
        string sv(buf);

        VertexPNTSoftbody v;
        DWORD vi = sbLinkMap[sv];
        v.position = m_VPNTData[i].position;
        v.normal = m_VPNTData[i].normal;
        v.tex = m_VPNTData[i].tex;
        v.tetra = m_SBLinkData[vi].tetra;
        v.barycentricCoords = m_SBLinkData[vi].barycentricCoord;

        vertices.push_back(v);
        //softbodyLinkIndices.push_back(vi);
    }
    mesh->SetVertices(vertices);
    mesh->SetIndices(m_IndexData);


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
    m_TextureData.push_back(copy);
}
void ModelLoader::AddTetra(int t1, int t2, int t3, int t4)
{
    m_TetraData.push_back(t1);
    m_TetraData.push_back(t2);
    m_TetraData.push_back(t3);
    m_TetraData.push_back(t4);
}
void ModelLoader::AddSBLink(int t, const Vector3& bc)
{
    m_SBLinkData.push_back(SBLink(t, bc));
}


void ModelLoader::CalculateTangents()
{
    Vector3* tan1 = new Vector3[m_VPNTData.size()];
    Vector3* tan2 = new Vector3[m_VPNTData.size()];
    ZeroMemory(tan1, m_VPNTData.size() * 12);
    ZeroMemory(tan2, m_VPNTData.size() * 12);
    for (UINT i = 0; i < m_IndexData.size(); i += 3)
    {
        int i1 = m_IndexData[i], 
            i2 = m_IndexData[i+1], 
            i3 = m_IndexData[i+2];

	    const Vector3& v1 = m_VPNTData[i1].position;
	    const Vector3& v2 = m_VPNTData[i2].position;
	    const Vector3& v3 = m_VPNTData[i3].position;

	    const Vector2& tx1 = m_VPNTData[i1].tex;
	    const Vector2& tx2 = m_VPNTData[i2].tex;
	    const Vector2& tx3 = m_VPNTData[i3].tex;

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

    for (UINT i = 0; i < m_VPNTData.size(); ++i)
    {
        const Vector3& n = m_VPNTData[i].normal;
        const Vector3& t = tan1[i];

        m_VPNTData[i].tangent = (t - n * n.Dot(t));
        m_VPNTData[i].tangent.Normalize();

        if (m_VPNTData[i].tangent.Dot(m_VPNTData[i].normal) > 0.0001f)
            PANIC("Tangent not loodrecht op Normal");
    }


    delete[] tan1;
    delete[] tan2;
}