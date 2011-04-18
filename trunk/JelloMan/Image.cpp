//---------------------------
// Includes
//---------------------------
#include "Image.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Image::Image(const tstring& refFileName, ID2D1Bitmap* pBitmap, IWICFormatConverter* pConverter)	:	m_pBitmap(pBitmap),
																									m_FileName(refFileName),
																									m_ResourceID(-1),
																									m_Opacity(100),
																									m_pConverter(pConverter)
{
}

Image::Image(int resourceID, ID2D1Bitmap* pBitmap)	:	m_pBitmap(pBitmap),
														m_FileName(_T("EMBEDDED")),
														m_ResourceID(resourceID),
														m_Opacity(100)
{
}

Image::~Image()
{
	SafeRelease(m_pBitmap);
}

// GETTERS
Size2D Image::GetDimensions() const
{
	Size2D size = m_pBitmap->GetSize();

	return size;
}