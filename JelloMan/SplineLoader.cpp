#include "SplineLoader.h"
#include "CookingStream.h"

SplineLoader::SplineLoader(void): 
                m_pCurrentModel(0), 
                m_pCurrentMesh(0),
                m_pAssetContainer(new AssetContainer<Model<VertexPos>>())
{
}


SplineLoader::~SplineLoader(void)
{
    delete m_pAssetContainer;
}

Model<VertexPos>* SplineLoader::Load(ID3D10Device *pDXDevice, const tstring& assetName)
{
    if (m_pAssetContainer->IsAssetPresent(assetName))
    {
        return m_pAssetContainer->GetAsset(assetName);
    }
    else
    {
		m_pCurrentModel = 0;
        m_pCurrentModel = new Model<VertexPos>(pDXDevice);
		m_VertexData.clear();
		m_IndexData.clear();
		m_VPNTTData.clear();
		m_pCurrentMesh = 0;

        if (assetName.rfind(_T(".obj")) != -1)
            ReadASCIIObj(assetName); //assigns all meshes
        else if (assetName.rfind(_T(".binobj")) != -1)
            ReadBinObj(assetName);

        m_pAssetContainer->AddAsset(assetName, m_pCurrentModel);

        return m_pCurrentModel;
    }
}

void SplineLoader::ReadBinObj(const tstring& assetName)
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
		m_pCurrentMesh = m_pCurrentModel->AddMesh(tstring(tempname.cbegin(), tempname.cend()), D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

        m_VPNTTData.clear();
        DWORD vertices = stream.readDword();

        for (DWORD v = 0; v < vertices; ++v)
        {
            Vector3 pos = stream.readVector3();

			m_VPNTTData.push_back(VertexPos(pos));
        }

        m_pCurrentMesh->SetVertices(m_VPNTTData);
    }
}

//*********************
//  ASCII
//******************
void SplineLoader::ReadASCIIObj(const tstring& assetName)
{
    ifstream stream;
    stream.open(assetName.c_str());
    if (stream.fail())
    {
        wcout << "File open fail: '" << assetName << "'\n";
        return;
    }

    string line;
    
    while (stream.eof() == false)
    {
        getline(stream, line);
        if (line.find("v", 0) == 0) //v is 0'd char
        {
            Vector3 v;
            sscanf_s(line.c_str(), "v %f %f %f", &v.X, &v.Y, &v.Z);
            AddVertex(v);
        }

		else if (line.find("l", 0) == 0) //v is 0'd char
        {
			tstringstream strm;
			strm << line.c_str();

			tstring s = strm.str().substr(strm.str().find(_T("l")) + 2, strm.str().size() - 1);

			tstringstream s1;

			for (tstring::const_iterator it = s.cbegin(); it != s.cend(); ++it)
			{
				if ((*it) == ' ')
				{
					tstring tstrIndex = s1.str();
					string strIndex(tstrIndex.begin(), tstrIndex.end());

					const char* cIndex = strIndex.c_str();

					int index = atoi(cIndex);

					if (index != 0)
						m_IndexData.push_back(index - 1);

					s1.clear();
					s1.str(_T(""));
				}
				else
				{
					s1 << (*it);
				}
			}

			tstring tstrIndex = s1.str();
			string strIndex(tstrIndex.begin(), tstrIndex.end());

			const char* cIndex = strIndex.c_str();

			int index = atoi(cIndex);

			if (index != 0)
				m_IndexData.push_back(index - 1);
        }
		else if (line.find("g", 0) == 0)
        {
            wchar_t s[40];
            sscanf_s(line.c_str(), "g %s40", s, _countof(s));
            AddMesh(s);
        }
    }

    FlushMesh(); //apply last mesh

    stream.close();
}

void SplineLoader::AddVertex(const Vector3& v)
{
	Vector3 copy = v;
	copy.Z *= -1;
    m_VertexData.push_back(copy);

	m_VPNTTData.push_back(VertexPos(copy));
}
void SplineLoader::AddMesh(const tstring& name)
{
    if (m_pCurrentMesh != 0) //apply the vertex and indexbuffer to the mesh
		FlushMesh();

	m_pCurrentMesh = m_pCurrentModel->AddMesh(name, D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
}

void SplineLoader::FlushMesh()
{
    ASSERT (m_pCurrentMesh != 0, "");
    m_pCurrentMesh->SetVertices(m_VPNTTData);
	m_pCurrentMesh->SetIndices(m_IndexData);
	m_IndexData.clear();
	m_VertexData.clear();
    m_pCurrentMesh = 0;
}