#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"
#include "AssetContainer.h"
#include "Texture2D.h"

class TextureLoader
{
public:
    //------Constructor-Destructor------->
	TextureLoader(void);
	virtual ~TextureLoader(void);
    //<-----------------------------------

	Texture2D* Load(ID3D10Device* pDXDevice, const tstring& key, bool cpuReadAccess = false);

    Texture2D* LoadDefaultWhite(ID3D10Device* pDXDevice);
    Texture2D* LoadDefaultGray(ID3D10Device* pDXDevice);
    Texture2D* LoadDefaultBlack(ID3D10Device* pDXDevice);
    Texture2D* LoadDefaultNormal(ID3D10Device* pDXDevice);

private:
	Texture2D* Load(ID3D10Device* pDXDevice, const tstring& key, bool skipLoadDefault, bool cpuReadAccess);

	//Disable default copy constructor and assignment operator
	TextureLoader(const TextureLoader& t);
	TextureLoader& operator=(const TextureLoader& t);

    AssetContainer<Texture2D>* m_pAssets;
};
