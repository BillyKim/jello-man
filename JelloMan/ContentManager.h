#pragma once
#include "d3dUtil.h"
#include "EffectLoader.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "vertex.h"
#include "ImageLoader.h"
#include "TextFormatLoader.h"
#include "SplineLoader.h"

class TextureLoader;
class EffectLoader;

#define Content ContentManager::GetSingleton()

class ContentManager
{
public:
	virtual ~ContentManager();	

	void Init(ID3D10Device* pDXDevice);
	
	template <typename T>
    T* LoadEffect(const tstring& assetName)
    {
        return m_pEffectLoader->Load<T>(m_pDevice, assetName); 
    }

	Texture2D* LoadTexture2D(const tstring& assetName);

	Model<VertexPosNormTanTex>* LoadModel(const tstring& assetName);
	Model<VertexPosCol>* LoadSpline(const tstring& assetName, Color col);

	SoftbodyMesh* LoadSoftbodyMesh(const tstring& assetName);

	//* Loads image from a file location. *
	Image* LoadImage(const tstring& fileNameRef, int newWidth = 0, int newHeight = 0);
	//* Loads new text format. *
	TextFormat* LoadTextFormat(const tstring& fontName, float size, bool bold = false, bool italic = false);

    ID3D10Device* GetDxDevice() { return m_pDevice; }

	static ContentManager* GetSingleton();

private:
	ContentManager();

	// initialize wic imaging factory
	void CreateWICFactory();

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	ID3D10Device* m_pDevice;
	IWICImagingFactory* m_pWICFactory;

	ImageLoader* m_pImageLoader;
	TextFormatLoader* m_pTextFormatLoader;
	
	TextureLoader* m_pTextureLoader;
	EffectLoader* m_pEffectLoader;
	ModelLoader* m_pModelLoader;
	SplineLoader* m_pSplineLoader;
	
	static ContentManager* m_pSingleton;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ContentManager(const ContentManager& yRef);									
	ContentManager& operator=(const ContentManager& yRef);	
};

 
