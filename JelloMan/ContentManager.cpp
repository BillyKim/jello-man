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
{
}

ContentManager::~ContentManager()
{
	delete m_pTextureLoader;
	delete m_pEffectLoader;
	delete m_pModelLoader;
	m_pSingleton = 0;
}

void ContentManager::Init(ID3D10Device* pDXDevice)
{
	m_pDevice = pDXDevice;
	m_pTextureLoader = new TextureLoader();
	m_pEffectLoader = new EffectLoader();
	m_pModelLoader = new ModelLoader();
}

Texture2D* ContentManager::LoadTexture2D(const tstring& assetName)
{
    return m_pTextureLoader->Load(m_pDevice, assetName);
}

Model<VertexPosNormTex>* ContentManager::LoadModel(const tstring& assetName)
{
    return m_pModelLoader->Load(m_pDevice, assetName);
}

//Static Method
ContentManager* ContentManager::GetSingleton()
{
	if (m_pSingleton == NULL) m_pSingleton = new ContentManager();
	return m_pSingleton;
}



