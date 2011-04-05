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
        m_pCurrentSBMesh = new SoftbodyMesh(pDXDevice, _T("Softbody"));

        if (assetName.rfind(_T(".obj")) != -1)
        {
            wcout << L"Error: softbodymesh can not be an .obj -- " << assetName << "\n";
            return 0;
        }
        else if (assetName.rfind(_T(".binobj")) != -1)
            ReadBinSBObj(assetName);

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


        m_VPNTTData.clear();
        DWORD vertices = stream.readDword();

		vector<Vector3> tempVertices;
		vector<Vector3> tempNormals;
		vector<Vector2> tempTexCoords;

        for (DWORD v = 0; v < vertices; ++v)
        {
            Vector3 pos = stream.readVector3();
            Vector3 norm = stream.readVector3();
            Vector2 tex = stream.readVector2();

			tex.Y *= -1; // fixes texcoords

			tempVertices.push_back(pos);
			tempNormals.push_back(norm);
			tempTexCoords.push_back(tex);

			if ((v+1) % 3 == 0)
			{
				Vector3 tangent = Vector3(0,0,0);

				CalculateTangent(	tempVertices[0], tempVertices[1], tempVertices[2],
									norm,
									tempTexCoords[0],  tempTexCoords[1],  tempTexCoords[2],
									tangent	);

				for (int i = 0; i < 3; ++i)
					m_VPNTTData.push_back(VertexPosNormTanTex(tempVertices[i], tempNormals[i], tangent, tempTexCoords[i]));

				tempVertices.clear();
				tempNormals.clear();
				tempTexCoords.clear();
			}
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
    ASSERT (m_pCurrentMesh != 0);

    unsigned int index[3];

	Vector3 tangent = Vector3(0,0,0);
	CalculateTangent(	m_VertexData[data[0][0] - 1], m_VertexData[data[1][0] - 1], m_VertexData[data[2][0] - 1],
						m_NormalData[data[0][2] - 1],
						m_TextureData[data[0][1] - 1],  m_TextureData[data[1][1] - 1],  m_TextureData[data[2][1] - 1],
						tangent	);

    for (int i = 0; i < 3; ++i)
    {
        index[i] = m_VPNTTData.size();
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
}

void ModelLoader::FlushMesh()
{
    ASSERT (m_pCurrentMesh != 0);
    m_pCurrentMesh->SetVertices(m_VPNTTData);
    m_pCurrentMesh->SetIndices(m_IndexData);
    m_VPNTTData.clear();
    m_VPNTTMap.clear();
    m_pCurrentMesh = 0;
    m_IndexData.clear();
}

void ModelLoader::CalculateTangent(	Vector3& v1, Vector3& v2, Vector3& v3,	
									Vector3& n1,							
									Vector2& t1, Vector2& t2, Vector2& t3,	
									Vector3& tangent	)
{
	/*Vector3 edge1, edge2;

	edge1.X = v2.X- v1.X;
	edge1.Y = v2.Y - v1.Y;
	edge1.Z = v2.Z - v1.Z;

	float dp;
	dp = n1.X * edge1.X + n1.Y * edge1.Y + n1.Z * edge1.Z;

	edge1.X = edge1.X - n1.X * dp;
	edge1.X = edge1.X - n1.X * dp;
	edge1.Z = edge1.Z - n1.Z * dp;

	edge2.X = v3.X - v1.X;
	edge2.X = v3.X - v1.X;
	edge2.Z = v3.Z - v1.Z;

	dp = n1.X * edge2.X + n1.X * edge2.X + n1.Z * edge2.Z;

	edge2.X = edge2.X - n1.X * dp;
	edge2.X = edge2.X - n1.X * dp;
	edge2.Z = edge2.Z - n1.Z * dp;

	Vector3 tcEdge1, tcEdge2;

	tcEdge1.X = t2.X - t1.X;
	tcEdge1.X = t2.X - t1.X;

	tcEdge2.X = t3.X - t1.X;
	tcEdge2.X = t3.X - t1.X;

	if ((tcEdge2.X * tcEdge1.X) > (tcEdge1.X * tcEdge2.X))
	{
		tcEdge1.X = -tcEdge1.X;
		tcEdge2.X = -tcEdge2.X;
	}

	float d = tcEdge1.X * tcEdge2.X - tcEdge2.X * tcEdge1.X;

	tangent.X = 1; tangent.X = 0; tangent.Z = 0;

	if (!(d > -0.0001f && d < 0.0001f))
	{
		tangent.X = (edge1.X * tcEdge2.X) -
					(edge2.X * tcEdge1.X);

		tangent.Y = (edge1.Y * tcEdge2.X) -
					(edge2.Y * tcEdge1.X);

		tangent.Z = (edge1.Z * tcEdge2.X) -
					(edge2.Z * tcEdge1.X);

		dp = (float)sqrt(	tangent.X * tangent.X +
							tangent.Y * tangent.Y +
							tangent.Z * tangent.Z	);

		if (!(dp > -0.0001f && dp < 0.0001f))
		{
			dp = 1 / dp;
			tangent.X *= dp;
			tangent.Y *= dp;
			tangent.Z *= dp;
		}
	}*/

	Vector3 e0 = v2 - v1;
	Vector3 e1 = v3 - v1;
	Vector3 normal = e0.Cross(e1);

	//using Eric Lengyel's approach with a few modifications
	//from Mathematics for 3D Game Programmming and Computer Graphics
	// want to be able to trasform a vector in Object Space to Tangent Space
	// such that the x-axis cooresponds to the 's' direction and the
	// y-axis corresponds to the 't' direction, and the z-axis corresponds
	// to <0,0,1>, straight up out of the texture map

	//let P = v1 - v0	
	Vector3 P = v2 - v1;

	//let Q = v2 - v0
	Vector3 Q = v3 - v1;

	float st1 = t2.X - t1.X;
	float tt1 = t2.Y - t1.Y;
	float st2 = t3.X - t1.X;
	float tt2 = t3.Y - t1.Y;

	//we need to solve the equation
	/*P = s1*T + t1*B
	floay Q = s2*T + t2*B*/
	// for T and B
	//this is a linear system with six unknowns and six equatinos, for TxTyTz BxByBz
	//[px,py,pz] = [s1,t1] * [Tx,Ty,Tz]
	// qx,qy,qz     s2,t2     Bx,By,Bz
	//multiplying both sides by the inverse of the s,t matrix gives
	//[Tx,Ty,Tz] = 1/(s1t2-s2t1) *  [t2,-t1] * [px,py,pz]
	// Bx,By,Bz                      -s2,s1	    qx,qy,qz  
	//solve this for the unormalized T and B to get from tangent to object space

	float tmp = 0.0f;
	if(fabsf(st1*tt2 - st2*tt1) <= 0.0001f)	
	{
		tmp = 1.0f;	
	}
	else
	{
		tmp = 1.0f/(st1*tt2 - st2*tt1 );
	}

	tangent.X = (tt2*P.X - tt1*Q.X);
	tangent.Y = (tt2*P.Y - tt1*Q.Y);
	tangent.Z = (tt2*P.Z - tt1*Q.Z);
	tangent = tangent*tmp;

	normal.Normalize();	
	tangent.Normalize();
}