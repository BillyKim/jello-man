#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Model.h"
#include "Vector3.h"
#include "vertex.h"
#include <vector>
#include <map>
#include "ProgressInfo.h"

struct SBLink
{
    SBLink(int t, const Vector3& bc) : tetra(t), barycentricCoord(bc) {}

    int tetra;
    Vector3 barycentricCoord;
};

class ModelLoader
{
public:
    //------Constructor-Destructor------->
	ModelLoader(void);
	virtual ~ModelLoader(void);
    //<-----------------------------------
	
	Model* Load(const string& assetName);

private:
    void ReadObj(const string& assetName, bool isSoftbody);
    void ReadTet(const string& fileName);
    void LinkSBtoModel(const string& fileName);

    void AddVertex(const Vector3& v, bool isSoftbody);
    void AddNormal(const Vector3& v);
    void AddTexCoord(const Vector2& v);
    void AddMesh(const string& name);
    void AddTri(const vector<vector<int>>& data);
    void AddTetra(int t1, int t2, int t3, int t4);
    void AddSBLink(int t, const Vector3& bc);
    void FlushMesh();

    vector<Vector3> m_VertexData;
    vector<Vector3> m_NormalData;
    vector<Vector2> m_TextureData;

    vector<VertexPosNormTex> m_VPNTData;
    map<string, DWORD> m_VPNTMap;

    vector<VertexPos> m_VPData;
    map<string, DWORD> m_VPMap;

    vector<DWORD> m_IndexData;

    vector<SBLink> m_SBLinkData;
    vector<DWORD> m_TetraData;

    Model* m_pCurrentModel;
    ModelMesh<VertexPos>* m_pCurrentPhysXMesh;
    ModelMesh<VertexPosNormTex>* m_pCurrentDrawMesh;

    ProgressInfo* m_pProgress;

	ModelLoader(const ModelLoader& t);
	ModelLoader& operator=(const ModelLoader& t);

};
