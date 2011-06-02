#include "ModelLoader.h"
#include "CookingStream.h"

ModelLoader::ModelLoader(void): 
                m_pCurrentModel(0), 
                m_pCurrentMesh(0),
                m_pAssetContainer(new AssetContainer<Model<VertexPosNormTanTex>>()),
                m_pSBAssetContainer(new AssetContainer<SoftbodyMesh>())
{
}


ModelLoader::~ModelLoader(void)
{
    delete m_pAssetContainer;
    delete m_pSBAssetContainer;
}

Model<VertexPosNormTanTex>* ModelLoader::Load(ID3D10Device *pDXDevice, const tstring& assetName)
{
    if (m_pAssetContainer->IsAssetPresent(assetName))
    {
        return m_pAssetContainer->GetAsset(assetName);
    }
    else
    {
        m_pCurrentModel = new Model<VertexPosNormTanTex>(pDXDevice);
        m_VertexData.clear();
        m_NormalData.clear();
        m_TextureData.clear();
		m_TangentData.clear();

        if (assetName.rfind(_T(".obj")) != -1)
            ReadASCIIObj(assetName); //assigns all meshes
        else if (assetName.rfind(_T(".binobj")) != -1)
            ReadBinObj(assetName);

        m_pAssetContainer->AddAsset(assetName, m_pCurrentModel);

        return m_pCurrentModel;
    }
}
SoftbodyMesh* ModelLoader::LoadSoftbodyMesh(ID3D10Device *pDXDevice, const tstring& assetName)
{
    if (m_pSBAssetContainer->IsAssetPresent(assetName))
    {
        return m_pSBAssetContainer->GetAsset(assetName);
    }
    else
    {
        if (assetName.rfind(_T(".obj")) != -1)
        {
            wcout << L"Error: softbodymesh can not be an .obj -- " << assetName << "\n";
            return 0;
        }
        else 
        {          
            m_pCurrentSBMesh = new SoftbodyMesh(pDXDevice, _T("Softbody"));
            if (assetName.rfind(_T(".binobj")) != -1)
                ReadBinSBObj(assetName);
        }

        m_pSBAssetContainer->AddAsset(assetName, m_pCurrentSBMesh);
        return m_pCurrentSBMesh;
    }
}
void ModelLoader::ReadBinObj(const tstring& assetName)
{
    //LAYOUT
    //DWord : #meshes
        //Word: nameLength
            //buffer: name
        //DWord : #vert
            //vector3: pos, vector3: norm, vector3 tan, vector2: tex     : vertex
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
        tempname = tempname.substr(0, nameLength);
        delete name;
        m_pCurrentMesh = m_pCurrentModel->AddMesh(tstring(tempname.cbegin(), tempname.cend()));

        m_VPNTTData.clear();
        DWORD vertices = stream.readDword();

        for (DWORD v = 0; v < vertices; ++v)
        {
            Vector3 pos = stream.readVector3();
            Vector3 norm = stream.readVector3();
            Vector3 tan = stream.readVector3();
            Vector2 tex = stream.readVector2();

            m_VPNTTData.push_back(VertexPosNormTanTex(pos, norm, tan, tex));
        }

        DWORD indices = stream.readDword() * 3;
        m_IndexData.clear();

        for (DWORD i = 0; i < indices; ++i)
        {
            DWORD index = stream.readDword();
            m_IndexData.push_back(index);
        }
        m_pCurrentMesh->SetVertices(m_VPNTTData);
        m_pCurrentMesh->SetIndices(m_IndexData);
    }
}
void ModelLoader::ReadBinSBObj(const tstring& assetName)
{
    //LAYOUT
    //Word: nameLength
        //buffer: name
    //DWord : #vert
        //vector3: pos, vector3: norm, vector2: tex, DWORD tetra, Vector3 bc     : vertex
    //DWord : #triangles
        //DWord, DWord, DWord : triangle


    string filename(assetName.cbegin(), assetName.cend());
    UserStream stream(filename.c_str(), true);

    WORD nameLength = stream.readWord();
    char* name = new char[nameLength];
    stream.readBuffer(name, nameLength);
        
    string tempname(name);
    delete name;
    m_pCurrentSBMesh->SetName(tstring(tempname.cbegin(), tempname.cend()));
      
    m_VPNTTData.clear();
    vector<DWORD> tetraData;
    vector<Vector3> bcData;

    DWORD numVertices = stream.readDword();
    for (DWORD v = 0; v < numVertices; ++v)
    {
        Vector3 pos = stream.readVector3();
        Vector3 norm = stream.readVector3();
        Vector2 tex = stream.readVector2();
        DWORD tetra = stream.readDword();
        Vector3 bc = stream.readVector3();
        m_VPNTTData.push_back(VertexPosNormTanTex(pos, norm, Vector3(), tex));

        tetraData.push_back(tetra);
        bcData.push_back(bc);
    }

    DWORD tris = stream.readDword();
    m_IndexData.clear();
    for (DWORD i = 0; i < tris; ++i)
    {
        DWORD index0 = stream.readDword();
        DWORD index1 = stream.readDword();
        DWORD index2 = stream.readDword();
        m_IndexData.push_back(index2);
        m_IndexData.push_back(index1);
        m_IndexData.push_back(index0);

		CalculateTangents(m_VPNTTData[index2], m_VPNTTData[index1], m_VPNTTData[index0]);
    }
    m_pCurrentSBMesh->SetVertices(m_VPNTTData);
    m_pCurrentSBMesh->SetIndices(m_IndexData);
    m_pCurrentSBMesh->SetTetra(tetraData);
    m_pCurrentSBMesh->SetBaryCentricCoords(bcData);
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
void ModelLoader::AddTangent(const Vector3& v)
{
	m_TangentData.push_back(v);
}
void ModelLoader::AddTexCoord(const Vector2& v)
{
	Vector2 copy = v;
	copy.Y *= -1;
    m_TextureData.push_back(copy);
}
void ModelLoader::AddMesh(const tstring& name)
{
    if (m_pCurrentMesh != 0) //apply the vertex and indexbuffer to the mesh
        FlushMesh();

    m_pCurrentMesh  = m_pCurrentModel->AddMesh(name);
}
void ModelLoader::AddTri(const vector<vector<int>>& data)
{
    ASSERT (m_pCurrentMesh != 0, "");

    unsigned int index[3];

    for (int i = 0; i < 3; ++i)
    {
        index[i] = m_VPNTTData.size();
		Vector3 tangent = Vector3(0,0,0);
        VertexPosNormTanTex vpntt(
                m_VertexData[data[i][0] - 1],
                m_NormalData[data[i][2] - 1],
				tangent,
                m_TextureData[data[i][1] - 1]);

        stringstream stream;
        stream << data[i][0] << "/" << data[i][1] << "/" << data[i][2];
        if (m_VPNTTMap.count(stream.str()) == 0)
        {
            m_VPNTTData.push_back(vpntt);
            m_VPNTTMap[stream.str()] = index[i];
        }
        else
        {
            index[i] = m_VPNTTMap[stream.str()];
        }   
    }
    
    m_IndexData.push_back(index[2]);
    m_IndexData.push_back(index[1]);
    m_IndexData.push_back(index[0]);

	CalculateTangents(m_VPNTTData[index[2]], m_VPNTTData[index[1]], m_VPNTTData[index[0]]);
}

void ModelLoader::FlushMesh()
{
    ASSERT(m_pCurrentMesh != 0, "");
	
	// TANGENTS
	for (unsigned int i = 0; i < m_VPNTTData.size() / 3; ++i)
	{
		CalculateTangents(m_VPNTTData[i*3],m_VPNTTData[(i*3)+1],m_VPNTTData[(i*3)+2]);
	}

    m_pCurrentMesh->SetVertices(m_VPNTTData);
    m_pCurrentMesh->SetIndices(m_IndexData);
    m_VPNTTData.clear();
    m_VPNTTMap.clear();
    m_pCurrentMesh = 0;
    m_IndexData.clear();
}

//Deprecated does not work properly
void ModelLoader::CalculateTangents(VertexPosNormTanTex& vpntx1, VertexPosNormTanTex& vpntx2, VertexPosNormTanTex& vpntx3)
{
	Vector3& v1 = vpntx1.position;
	Vector3& v2 = vpntx2.position;
	Vector3& v3 = vpntx2.position;

	Vector3& n1 = vpntx1.normal;
	//Vector3& n2 = vpntx2.normal;
	//Vector3& n3 = vpntx3.normal;

	Vector2& tx1 = vpntx1.tex;
	Vector2& tx2 = vpntx2.tex;
	Vector2& tx3 = vpntx3.tex;

	Vector3 t1;
	Vector3 t2;
	Vector3 t3;

	float x1 = v2.X - v1.X;
	float x2 = v3.X - v1.X;
	float y1 = v2.Y - v1.Y;
	float y2 = v3.Y - v1.Y;
	float z1 = v2.Z - v1.Z;
	float z2 = v3.Z - v1.Z;

	float s1 = tx2.X - tx1.X;
	float s2 = tx3.X - tx1.X;
	float st1 = tx2.Y - tx1.Y;
	float st2 = tx3.Y - tx1.Y;

	float r = 1.0f / (s1 * st2 - s2 * st1);

	Vector3 tan1((st2 * x1 - st1 * x2) * r, (st2 * y1 - st1 * y2) * r, (st2 * z1 - st1 * z2) * r );
	Vector3 tan2((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r );

	float n1tan1dot = n1.Dot(tan1);
	t1 = n1 * n1tan1dot;
	t1 = tan1 - t1;
	t1.Normalize();
	
	t2 = n1 * n1tan1dot;
	t2 = tan1 - t1;
	t2.Normalize();

	t3 = n1 * n1tan1dot;
	t3 = tan1 - t1;
	t3.Normalize();

	vpntx1.tangent += t1;
	vpntx2.tangent += t2;
	vpntx3.tangent += t3;
}