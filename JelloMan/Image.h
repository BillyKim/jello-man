#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"

//-----------------------------------------------------
// Image Class									
//-----------------------------------------------------
class Image
{
public:
	Image(const tstring& refFileName, ID2D1Bitmap* pBitmap, IWICFormatConverter* pConverter);
	Image(int resourceID, ID2D1Bitmap* pBitmap);
	virtual ~Image();

	/* GETTERS */

	//* Get the dimensions of the bitmap. *
	Size2D GetDimensions() const;

	//* Get the opacity of the bitmap - 0 meaning transparant, 1 opaque. *
	float GetOpacity() const {return m_Opacity;}

	//* Returns the D2D1ResourceBitmap pointer - only used for drawing images. *
	ID2D1Bitmap* GetD2D1BitmapResource() const
	{ return m_pBitmap; }

	//* Returns the file location of the image. *
	tstring GetFileLocation() const
	{ return m_FileName; }

	IWICFormatConverter* GetFormatConverter() const
	{ return m_pConverter; }

	/* SETTERS */

	//* Sets the overall opacity of the image. *
	void SetOpacity(float opacity) {m_Opacity = opacity;}

private:
	ID2D1Bitmap* m_pBitmap;
	IWICFormatConverter* m_pConverter;
	tstring m_FileName;
	int m_ResourceID;
	float m_Opacity;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Image(const Image& yRef);									
	Image& operator=(const Image& yRef);	
};

 
