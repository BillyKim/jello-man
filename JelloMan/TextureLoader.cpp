#include "StdAfx.h"
#include "TextureLoader.h" 
#include "d3dUtil.h"


TextureLoader::TextureLoader(void)
{

}

TextureLoader::~TextureLoader(void)
{
	AssetContainer::~AssetContainer();
}

Texture2D* TextureLoader::Load(ID3D10Device *pD3DDevice, const tstring& assetName) 
{
	if ( IsAssetPresent(assetName))
	{
        //#if defined DEBUG || _DEBUG
		//cout << "Using Existing Texture.\n";
        //#endif
	}
	else
	{
		ID3D10ShaderResourceView *pTextureRV;

		HRESULT hr = D3DX10CreateShaderResourceViewFromFile(pD3DDevice, assetName.c_str(), NULL, NULL, &pTextureRV, NULL);
		if(hr != S_OK)
		{
			wcout << "Loading texture " << assetName << "Failed!";
			return 0;
		}

        Texture2D* tex = new Texture2D(pTextureRV);
        AddAsset(assetName, tex);
	}

    return GetAsset(assetName);
}
