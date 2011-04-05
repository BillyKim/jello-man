#include "StdAfx.h"
#include "TextureLoader.h" 
#include "d3dUtil.h"


TextureLoader::TextureLoader(void): m_pAssets(new AssetContainer<Texture2D>)
{

}

TextureLoader::~TextureLoader(void)
{
    delete m_pAssets;
}

Texture2D* TextureLoader::Load(ID3D10Device *pDXDevice, const tstring& assetName) 
{
	if ( m_pAssets->IsAssetPresent(assetName))
	{
        //#if defined DEBUG || _DEBUG
		//cout << "Using Existing Texture.\n";
        //#endif
	}
	else
	{
		ID3D10ShaderResourceView *pTextureRV;

		HRESULT hr = D3DX10CreateShaderResourceViewFromFile(pDXDevice, assetName.c_str(), NULL, NULL, &pTextureRV, NULL);
		if(hr != S_OK)
		{
			wcout << "Loading texture '" << assetName << "' Failed!\n";
			return 0;
		}

        Texture2D* tex = new Texture2D(pTextureRV);
        m_pAssets->AddAsset(assetName, tex);
	}

    return m_pAssets->GetAsset(assetName);
}
