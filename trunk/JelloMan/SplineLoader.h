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
	
	Model<VertexPos>* Load(ID3D10Device *pDXDevice, const tstring& assetName);

private:
    void ReadBinObj(const tstring& assetName);
    void ReadASCIIObj(const tstring& assetName);

	void AddVertex(const Vector3& v);
    void AddMesh(const tstring& name);
    void FlushMesh();

    vector<Vector3> m_VertexData;

    vector<VertexPos> m_VPNTTData;
	vector<DWORD> m_IndexData;

    Model<VertexPos>* m_pCurrentModel;
    ModelMesh<VertexPos>* m_pCurrentMesh;

    AssetContainer<Model<VertexPos> >* m_pAssetContainer;

    //private
	SplineLoader(const SplineLoader& t);
	SplineLoader& operator=(const SplineLoader& t);
};
