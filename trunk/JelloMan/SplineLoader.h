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
#include "Color.h"

class SplineLoader
{
public:
    //------Constructor-Destructor------->
	SplineLoader(void);
	virtual ~SplineLoader(void);
    //<-----------------------------------
	
	Model<VertexPosCol>* Load(ID3D10Device *pDXDevice, const tstring& assetName, Color& col);

private:
    void ReadBinObj(const tstring& assetName, Color col);
    void ReadASCIIObj(const tstring& assetName, Color col);

	void AddVertex(const Vector3& v, Color& col);
    void AddMesh(const tstring& name);
	//void AddIndexes(const float& index);
    void FlushMesh();

    vector<Vector3> m_VertexData;

    vector<VertexPosCol> m_VPNTTData;
    map<string, DWORD> m_VPNTTMap;
	vector<DWORD> m_IndexData;

    Model<VertexPosCol>* m_pCurrentModel;
    ModelMesh<VertexPosCol>* m_pCurrentMesh;

    AssetContainer<Model<VertexPosCol> >* m_pAssetContainer;

    //private
	SplineLoader(const SplineLoader& t);
	SplineLoader& operator=(const SplineLoader& t);
};
