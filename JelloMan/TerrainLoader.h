#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"
#include "AssetContainer.h"
#include "Model.h"
#include <vector>

class TerrainLoader
{
public:
    //------Constructor-Destructor------->
	TerrainLoader(void);
	virtual ~TerrainLoader(void);
    //<-----------------------------------

    Model<VertexPosNormTanTex>* Load(ID3D10Device* pDXDevice, const tstring& key);

private:
	TerrainLoader(TerrainLoader& t);
	TerrainLoader& operator=(TerrainLoader& t);

    AssetContainer<Model<VertexPosNormTanTex>>* m_pAssets;
};

void CalculateNormals(vector<VertexPosNormTanTex>& vecVPNTData, const vector<DWORD>& vecIndexData);

void CalculateTangents(vector<VertexPosNormTanTex>& vecVPNTData, const vector<DWORD>& vecIndexData);