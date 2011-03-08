#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"
#include "TextureLoader.h"
#include "AssetContainer.h"
#include "Texture2D.h"

class TextureLoader : public AssetContainer<Texture2D>
{
public:
    //------Constructor-Destructor------->
	TextureLoader(void);
	virtual ~TextureLoader(void);
    //<-----------------------------------

	Texture2D* Load(ID3D10Device* pD3DDevice, const tstring& key);

private:
	TextureLoader(TextureLoader& t);
	TextureLoader& operator=(TextureLoader& t);

};
