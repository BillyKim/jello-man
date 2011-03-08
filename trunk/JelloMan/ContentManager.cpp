//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "ContentManager.h"
#include "EffectLoader.h"
#include "TextureLoader.h"

//---------------------------
// Constructor & Destructor
//---------------------------
ContentManager::ContentManager(ID3D10Device* pD3DDevice):
     m_pDevice(pD3DDevice)
	,m_pTextureLoader(0)
	,m_pEffectLoader(0)
{
	m_pTextureLoader = new TextureLoader();
	m_pEffectLoader = new EffectLoader();
}

ContentManager::~ContentManager()
{
	delete m_pTextureLoader;
	delete m_pEffectLoader;
}


Effect*	ContentManager::LoadEffect(const tstring& assetName)
{
    return m_pEffectLoader->Load(m_pDevice, assetName); 
}
Texture2D* ContentManager::LoadTexture2D(const tstring& assetName)
{
    return m_pTextureLoader->Load(m_pDevice, assetName);
}




