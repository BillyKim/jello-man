//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "ContentManager.h"

ContentManager* ContentManager::m_pSingleton = NULL;

//---------------------------
// Constructor & Destructor
//---------------------------
ContentManager::ContentManager():
     m_pDevice(0)
	,m_pTextureLoader(0)
	,m_pEffectLoader(0)
	,m_pModelLoader(0)
	,m_pImageLoader(0)
	,m_pTextFormatLoader(0)
	,m_pSplineLoader(0)
    ,m_pTerrainLoader(0)
{
	CreateWICFactory();
}

ContentManager::~ContentManager()
{
	delete m_pTextureLoader;
	delete m_pEffectLoader;
	delete m_pModelLoader;
	delete m_pImageLoader;
	delete m_pTextFormatLoader;
	delete m_pSplineLoader;
    delete m_pTerrainLoader;
	m_pSingleton = 0;
}

void ContentManager::Init(ID3D10Device* pDXDevice)
{
	m_pDevice = pDXDevice;
	m_pTextureLoader = new TextureLoader();
	m_pEffectLoader = new EffectLoader();
	m_pModelLoader = new ModelLoader();
	m_pTextFormatLoader = new TextFormatLoader();
	m_pImageLoader = new ImageLoader(m_pWICFactory);
	m_pSplineLoader = new SplineLoader();
    m_pTerrainLoader = new TerrainLoader();
}

Texture2D* ContentManager::LoadTexture2D(const tstring& assetName, bool cpuReadAccess)
{
    return m_pTextureLoader->Load(m_pDevice, assetName, cpuReadAccess);
}
Texture2D* ContentManager::LoadTexture2D(DefaultTextureType type)
{
    switch (type)
    {
        case DefaultTextureType_Black: return m_pTextureLoader->LoadDefaultBlack(m_pDevice);
        case DefaultTextureType_White: return m_pTextureLoader->LoadDefaultWhite(m_pDevice);
        case DefaultTextureType_Gray: return m_pTextureLoader->LoadDefaultGray(m_pDevice);
        case DefaultTextureType_Normal: return m_pTextureLoader->LoadDefaultNormal(m_pDevice);
        default: PANIC("Unrecognized DefaultTextureType"); return m_pTextureLoader->LoadDefaultGray(m_pDevice);
    }
}

Model<VertexPosNormTanTex>* ContentManager::LoadModel(const tstring& assetName)
{
    return m_pModelLoader->Load(m_pDevice, assetName);
}
SoftbodyMesh* ContentManager::LoadSoftbodyMesh(const tstring& assetName)
{
    return m_pModelLoader->LoadSoftbodyMesh(m_pDevice, assetName);
}

Model<VertexPosNormTanTex>* ContentManager::LoadTerrain(const tstring& assetName)
{
    return m_pTerrainLoader->Load(m_pDevice, assetName);
}

Model<VertexPos>* ContentManager::LoadSpline(const tstring& assetName)
{
	return m_pSplineLoader->Load(m_pDevice, assetName);
}

Image* ContentManager::LoadImage(const tstring& fileNameRef, int newWidth, int newHeight)
{
	if (!m_pImageLoader)
	{
		MessageBox(0,_T("ContentManager needs to be initialized before you can load images!"),_T("Error"),MB_OK);
		exit(-1);
	}

	return m_pImageLoader->LoadImage(fileNameRef, newWidth, newHeight);
}

TextFormat* ContentManager::LoadTextFormat(const tstring& fontName, float size, bool bold, bool italic)
{
	if (!m_pTextFormatLoader)
	{
		MessageBox(0,_T("ContentManager needs to be initialized before you can load text formats!"),_T("Error"),MB_OK);
		exit(-1);
	}

	return m_pTextFormatLoader->LoadTextFormat(fontName, size, bold, italic);
}

//Static Method
ContentManager* ContentManager::GetSingleton()
{
	if (m_pSingleton == NULL) m_pSingleton = new ContentManager();
	return m_pSingleton;
}

void ContentManager::CreateWICFactory()
{
	CoCreateInstance(	CLSID_WICImagingFactory,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_IWICImagingFactory,
						(LPVOID*)&m_pWICFactory	);
}



