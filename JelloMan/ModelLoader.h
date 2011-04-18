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
#include "SoftbodyMesh.h"

class ModelLoader
{
public:
    //------Constructor-Destructor------->
	ModelLoader(void);
	virtual ~ModelLoader(void);
    //<-----------------------------------
	
	Model<VertexPosNormTanTex>* Load(ID3D10Device *pDXDevice, const tstring& assetName);
	Model<VertexPosNormTanTex>* LoadWithTangents(ID3D10Device *pDXDevice, const tstring& assetName);
    SoftbodyMesh* LoadSoftbodyMesh(ID3D10Device *pDXDevice, const tstring& assetName);

private:
    void ReadBinObj(const tstring& assetName);
    void ReadBinSBObj(const tstring& assetName);
    void ReadASCIIObj(const tstring& assetName);

	void CalculateTangents(VertexPosNormTanTex& vpntx1, VertexPosNormTanTex& vpntx2, VertexPosNormTanTex& vpntx3);

    void AddVertex(const Vector3& v);
    void AddNormal(const Vector3& v);
	void AddTangent(const Vector3& v);
    void AddTexCoord(const Vector2& v);
    void AddMesh(const tstring& name);
    void AddTri(const vector<vector<int>>& data);
    void FlushMesh();

    vector<Vector3> m_VertexData;
    vector<Vector3> m_NormalData;
	vector<Vector3> m_TangentData;
    vector<Vector2> m_TextureData;

    vector<VertexPosNormTanTex> m_VPNTTData;
    map<string, DWORD> m_VPNTTMap;
    vector<DWORD> m_IndexData;

    Model<VertexPosNormTanTex>* m_pCurrentModel;
    ModelMesh<VertexPosNormTanTex>* m_pCurrentMesh;

    SoftbodyMesh* m_pCurrentSBMesh;

    AssetContainer<Model<VertexPosNormTanTex>>* m_pAssetContainer;
    AssetContainer<SoftbodyMesh>* m_pSBAssetContainer;

    //private
	ModelLoader(const ModelLoader& t);
	ModelLoader& operator=(const ModelLoader& t);
};
