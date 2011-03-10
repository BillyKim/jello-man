#pragma once
#include "d3dUtil.h"
#include "EffectLoader.h"
#include "TextureLoader.h"
#include "ModelLoader.h"

class TextureLoader;
class EffectLoader;

#define Content ContentManager::GetSingleton();

class ContentManager
{
public:
	virtual ~ContentManager();	

	void Init(ID3D10Device* pDXDevice);

	Effect* LoadEffect(const tstring& assetName);
	Texture2D* LoadTexture2D(const tstring& assetName);
	Model* LoadModel(const tstring& assetName);

	static ContentManager* GetSingleton();

 

private:
	ContentManager();
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	ID3D10Device* m_pDevice;
	
	TextureLoader* m_pTextureLoader;
	EffectLoader* m_pEffectLoader;
	ModelLoader* m_pModelLoader;
	
	static ContentManager* m_pSingleton;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ContentManager(const ContentManager& yRef);									
	ContentManager& operator=(const ContentManager& yRef);	
};

 
