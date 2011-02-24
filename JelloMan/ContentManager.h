#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"

//-----------------------------------------------------
// ContentManager Class									
//-----------------------------------------------------
class TextureLoader;
class EffectLoader;

class ContentManager
{
public:
	ContentManager();				// Constructor
	virtual ~ContentManager();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
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

 
