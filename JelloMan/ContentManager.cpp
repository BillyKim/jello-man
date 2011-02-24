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
ContentManager::ContentManager()
	:m_pTextureLoader(0)
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

//pass through methods
ID3D10Effect* ContentManager::GetEffect(ID3D10Device *pD3DDevice,const tstring& key)
{
	return m_pEffectLoader->GetEffect(pD3DDevice, key);
}
ID3D10ShaderResourceView* ContentManager::GetTexture(ID3D10Device *pD3DDevice, const tstring& Filename)
{
	return m_pTextureLoader->GetTexture(pD3DDevice, Filename);
}



