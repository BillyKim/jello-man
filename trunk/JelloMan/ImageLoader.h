#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include "AssetContainer.h"

class Image;

//-----------------------------------------------------
// ImageLoader Class									
//-----------------------------------------------------
class ImageLoader
{
public:
	ImageLoader(IWICImagingFactory* pWICFactory);
	virtual ~ImageLoader();

	/* GENERAL */

	//* Loads image from a file location. *
	Image* LoadImage(const tstring& fileNameRef, int newWidth = 0, int newHeight = 0);

	//* Loads image from an embedded resource - only BITMAPS or JPEGS can be embedded. *
	//Image* LoadImage(unsigned int resourceID, int newWidth = 0, int newHeight = 0);

private: 
	HRESULT LoadResourceBitmap(
        PCWSTR resourceName,
        PCWSTR resourceType,
        ID2D1Bitmap **ppBitmap,
		int destinationWidth,
		int destinationHeight
        );

    HRESULT LoadBitmapFromFile(
        PCWSTR uri,
        ID2D1Bitmap **ppBitmap,
		int destinationWidth,
		int destinationHeight,
		IWICFormatConverter** pConverter
        );

	// Datamembers
	ID2D1RenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pWICFactory;

	AssetContainer<Image>* m_pAssetContainer;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ImageLoader(const ImageLoader& yRef);									
	ImageLoader& operator=(const ImageLoader& yRef);	
};

 
