#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include "AssetContainer.h"
#include "Model.h"
#include "Vector3.h"
#include "vertex.h"
#include <vector>

class ModelLoader : public AssetContainer<Model>
{
public:
    //------Constructor-Destructor------->
	ModelLoader(void);
	virtual ~ModelLoader(void);
    //<-----------------------------------

    Model* Load(ID3D10Device *pDXDevice, const tstring& assetName);

private:
    void ReadObj(const tstring& assetName);

    void AddVertex(const Vector3& v);
    void AddNormal(const Vector3& v);
    void AddTexCoord(const Vector2& v);
    void AddMesh(const tstring& name);
    void AddTri(const vector<vector<int>>& data);
    void FlushMesh();

    vector<Vector3> m_VertexData;
    vector<Vector3> m_NormalData;
    vector<Vector2> m_TextureData;

    vector<VertexPosNormTex> m_VPNTData;
    vector<DWORD> m_IndexData;

    Model* m_pCurrentModel;
    ModelMesh* m_pCurrentMesh;

	ModelLoader(const ModelLoader& t);
	ModelLoader& operator=(const ModelLoader& t);

};
