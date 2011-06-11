#include "StdAfx.h"
#include "TextureLoader.h" 


TextureLoader::TextureLoader(void): m_pAssets(new AssetContainer<Texture2D>)
{

}

TextureLoader::~TextureLoader(void)
{
    delete m_pAssets;
}

Texture2D* TextureLoader::Load(ID3D10Device *pDXDevice, const tstring& assetName, bool cpuReadAccess) 
{
	return Load(pDXDevice, assetName, cpuReadAccess, cpuReadAccess);
}
Texture2D* TextureLoader::Load(ID3D10Device *pDXDevice, const tstring& assetName, bool skipLoadDefault, bool cpuReadAccess) 
{
	if ( m_pAssets->IsAssetPresent(assetName + (cpuReadAccess?_T("_READ"):_T(""))))
	{
        //#if defined DEBUG || _DEBUG
		//cout << "Using Existing Texture.\n";
        //#endif
	}
	else
	{
        D3DX10_IMAGE_LOAD_INFO info;
        info.Width = D3DX10_DEFAULT;
        info.Height = D3DX10_DEFAULT;
        info.Depth = D3DX10_DEFAULT;
        info.FirstMipLevel = cpuReadAccess?0:D3DX10_DEFAULT;
        info.MipLevels = cpuReadAccess?0:D3DX10_DEFAULT;
        info.Usage = cpuReadAccess?D3D10_USAGE_STAGING:D3D10_USAGE_IMMUTABLE;
        info.BindFlags = cpuReadAccess?0:D3DX10_DEFAULT;
        info.CpuAccessFlags = cpuReadAccess?D3D10_CPU_ACCESS_READ:0;
        info.MiscFlags = D3DX10_DEFAULT;
        info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        info.Filter = D3DX10_DEFAULT;
        info.MipFilter = D3DX10_DEFAULT;

        ID3D10Resource* pTexture;
        HRESULT hr = D3DX10CreateTextureFromFile(pDXDevice, assetName.c_str(), &info, NULL, &pTexture, NULL);
		if(hr != S_OK)
		{
            PANIC("Loading texture '" + string(assetName.begin(), assetName.end()) + "' Failed!");
            if (skipLoadDefault == true)
                return 0;
            else
                return LoadDefaultBlack(pDXDevice);
		}

        Texture2D* tex = new Texture2D(pDXDevice, pTexture);
        m_pAssets->AddAsset(assetName + (cpuReadAccess?_T("_READ"):_T("")), tex);
	}

    return m_pAssets->GetAsset(assetName + (cpuReadAccess?_T("_READ"):_T("")));
}
Texture2D* TextureLoader::LoadDefaultWhite(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_White.png"), true, false);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
Texture2D* TextureLoader::LoadDefaultGray(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_Gray.png"), true, false);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
Texture2D* TextureLoader::LoadDefaultBlack(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_Black.png"), true, false);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
Texture2D* TextureLoader::LoadDefaultNormal(ID3D10Device* pDXDevice)
{
    Texture2D* temp = Load(pDXDevice, _T("../Content/Textures/default/tex_PlainNormal.png"), true, false);
    ASSERT(temp != 0, "Error loading default texture");
    return temp;
}
