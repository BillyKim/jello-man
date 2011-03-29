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

class ModelLoader
{
public:
    //------Constructor-Destructor------->
	ModelLoader(void);
	virtual ~ModelLoader(void);
    //<-----------------------------------
	
	Model* Load(const string& assetName);

private:
    void ReadObj(const string& assetName);

    void AddVertex(const Vector3& v);
    void AddNormal(const Vector3& v);
    void AddTexCoord(const Vector2& v);
    void AddMesh(const string& name);
    void AddTri(const vector<vector<int>>& data);
    void FlushMesh();

    vector<Vector3> m_VertexData;
    vector<Vector3> m_NormalData;
    vector<Vector2> m_TextureData;

    vector<VertexPosNormTex> m_VPNTData;
    map<string, unsigned int> m_VPNTMap;
    vector<VertexPos> m_VPData;
    map<string, unsigned int> m_VPMap;
    vector<unsigned int> m_IndexData;

    Model* m_pCurrentModel;
    ModelMesh<VertexPos>* m_pCurrentPhysXMesh;
    ModelMesh<VertexPosNormTex>* m_pCurrentDrawMesh;

    ProgressInfo* m_pProgress;

	ModelLoader(const ModelLoader& t);
	ModelLoader& operator=(const ModelLoader& t);

};
