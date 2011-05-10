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
	return Load(pDXDevice, assetName, false);
}
Texture2D* TextureLoader::Load(ID3D10Device *pDXDevice, const tstring& assetName, bool skipLoadDefault) 
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
            PANIC("Loading texture '" + string(assetName.begin(), assetName.end()) + "' Failed!");
            if (skipLoadDefault == true)
                return 0;
            else
                return LoadDefaultBlack(pDXDevice);
		}

        Texture2D* tex = new Texture2D(pTextureRV);
        m_pAssets->AddAsset(assetName, tex);
	}

    return m_pAssets->GetAsset(assetName);
}
Texture2D* TextureLoader::LoadDefaultWhite(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_White.png"), true);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
Texture2D* TextureLoader::LoadDefaultGray(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_Gray.png"), true);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
Texture2D* TextureLoader::LoadDefaultBlack(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_Black.png"), true);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
Texture2D* TextureLoader::LoadDefaultNormal(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_PlainNormal.png"), true);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
