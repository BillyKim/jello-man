#pragma once
#include "d3dUtil.h"
#include "Texture2D.h"
#include "Effect.h"

class TextureLoader;
class EffectLoader;

#define Content ContentManager::GetSingleton();

class ContentManager
{
public:
	ContentManager(ID3D10Device* pD3DDevice);
	virtual ~ContentManager();	

	Effect*	LoadEffect(const tstring& assetName);
	Texture2D* LoadTexture2D(const tstring& assetName);

 

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	ID3D10Device* m_pDevice;
	
	TextureLoader *m_pTextureLoader;
	EffectLoader *m_pEffectLoader;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ContentManager(const ContentManager& yRef);									
	ContentManager& operator=(const ContentManager& yRef);	
};

 
