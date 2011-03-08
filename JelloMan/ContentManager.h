#pragma once
#include "d3dUtil.h"

class TextureLoader;
class EffectLoader;

class ContentManager
{
public:
	ContentManager();	
	virtual ~ContentManager();	

	ID3D10Effect* GetEffect(ID3D10Device *pD3DDevice, const tstring& key);
	ID3D10ShaderResourceView* GetTexture(ID3D10Device *pD3DDevice, const tstring& Filename);


private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	TextureLoader *m_pTextureLoader;
	EffectLoader *m_pEffectLoader;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ContentManager(const ContentManager& yRef);									
	ContentManager& operator=(const ContentManager& yRef);	
};

 
