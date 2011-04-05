#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"
#include "TextureLoader.h"
#include "AssetContainer.h"
#include "Texture2D.h"

class TextureLoader
{
public:
    //------Constructor-Destructor------->
	TextureLoader(void);
	virtual ~TextureLoader(void);
    //<-----------------------------------

	Texture2D* Load(ID3D10Device* pDXDevice, const tstring& key);

private:
	TextureLoader(TextureLoader& t);
	TextureLoader& operator=(TextureLoader& t);

    AssetContainer<Texture2D>* m_pAssets;
};
