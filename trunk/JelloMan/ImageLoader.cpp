//---------------------------
// Includes
//---------------------------
#include "ImageLoader.h"
#include "Image.h"
#include "Blox2D.h"

//---------------------------
// Constructor & Destructor
//---------------------------
ImageLoader::ImageLoader(	IWICImagingFactory* pWICFactory)
							:							
							m_pRenderTarget(0),
							m_pWICFactory(pWICFactory),
							m_pAssetContainer(new AssetContainer<Image>())
{
	// nothing to create
}

ImageLoader::~ImageLoader()
{
	delete m_pAssetContainer;

	// safe releasing members in contentmanager
}

Image* ImageLoader::LoadImage(const tstring& fileNameRef, int newWidth, int newHeight)
{
	if (m_pAssetContainer->IsAssetPresent(fileNameRef))
	{
		cout << "Image re-used: " << string(fileNameRef.begin(),fileNameRef.end()) << "\n";
		return m_pAssetContainer->GetAsset(fileNameRef);
	}

	ID2D1Bitmap* pBitmap = 0;
	IWICFormatConverter* pConverter = 0;

	HRESULT hr = LoadBitmapFromFile(
		fileNameRef.c_str(),
		&pBitmap,
		newWidth,
		newHeight,
		&pConverter);
		
	if (FAILED(hr))
	{
		tstringstream stream;
		stream << _T("Error loading file: ") << fileNameRef;
		
		MessageBox(0,stream.str().c_str(),_T("Error!"),MB_OK);
		exit(-1);
	}

	Image* pImage = new Image(fileNameRef, pBitmap, pConverter);

	#if defined DEBUG || _DEBUG
	cout << "Image loaded: " << string(fileNameRef.begin(),fileNameRef.end()) << "\n";
	#endif

	m_pAssetContainer->AddAsset(fileNameRef, pImage);

	return pImage;
}

//Image* ImageLoader::LoadImage(unsigned int resourceID, int newWidth, int newHeight)
//{
//	ID2D1Bitmap* pBitmap;
//
//	HRESULT hr = LoadResourceBitmap(
//		MAKEINTRESOURCE(resourceID),
//		L"Image",
//		&pBitmap,
//		newWidth,
//		newHeight);
//		
//	if (FAILED(hr))
//	{
//		tstringstream stream;
//		stream << _T("Error loading file with ID: ") << resourceID;
//		
//		MessageBox(stream.str());
//		exit(-1);
//	}
//
//	return Content->LoadImage(resourceID, pBitmap);
//}


//
// Creates a Direct2D bitmap from a resource in the
// application resource file.
//
/*
HRESULT ImageLoader::LoadResourceBitmap(
    PCWSTR resourceName,
    PCWSTR resourceType,
    ID2D1Bitmap **ppBitmap,
	int destinationWidth,
	int destinationHeight
    )
{
    IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pSource = NULL;
    IWICStream *pStream = NULL;
    IWICFormatConverter *pConverter = NULL;
    IWICBitmapScaler *pScaler = NULL;

    HRSRC imageResHandle = NULL;
    HGLOBAL imageResDataHandle = NULL;
    void *pImageFile = NULL;
    DWORD imageFileSize = 0;

    // Locate the resource.
    imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);
    HRESULT hr = imageResHandle ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        // Load the resource.
        imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

        hr = imageResDataHandle ? S_OK : E_FAIL;
    }
    if (SUCCEEDED(hr))
    {
        // Lock it to get a system memory pointer.
        pImageFile = LockResource(imageResDataHandle);

        hr = pImageFile ? S_OK : E_FAIL;
    }
    if (SUCCEEDED(hr))
    {
        // Calculate the size.
        imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

        hr = imageFileSize ? S_OK : E_FAIL;
        
    }
    if (SUCCEEDED(hr))
    {
          // Create a WIC stream to map onto the memory.
        hr = m_pWICFactory->CreateStream(&pStream);
    }
    if (SUCCEEDED(hr))
    {
        // Initialize the stream with the memory pointer and size.
        hr = pStream->InitializeFromMemory(
            reinterpret_cast<BYTE*>(pImageFile),
            imageFileSize
            );
    }
    if (SUCCEEDED(hr))
    {
        // Create a decoder for the stream.
        hr = m_pWICFactory->CreateDecoderFromStream(
            pStream,
            NULL,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder
            );
    }
    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }
    if (SUCCEEDED(hr))
    {
        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = m_pWICFactory->CreateFormatConverter(&pConverter);
    }
    if (SUCCEEDED(hr))
    {
        // If a new width or height was specified, create an
        // IWICBitmapScaler and use it to resize the image.
        if (destinationWidth != 0 || destinationHeight != 0)
        {
            UINT originalWidth, originalHeight;
            hr = pSource->GetSize(&originalWidth, &originalHeight);
            if (SUCCEEDED(hr))
            {
                if (destinationWidth == 0)
                {
                    FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                    destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
                }
                else if (destinationHeight == 0)
                {
                    FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                    destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
                }

                hr = m_pWICFactory->CreateBitmapScaler(&pScaler);
                if (SUCCEEDED(hr))
                {
                    hr = pScaler->Initialize(
                            pSource,
                            destinationWidth,
                            destinationHeight,
                            WICBitmapInterpolationModeCubic
                            );
                    if (SUCCEEDED(hr))
                    {
                        hr = pConverter->Initialize(
                            pScaler,
                            GUID_WICPixelFormat32bppPBGRA,
                            WICBitmapDitherTypeNone,
                            NULL,
                            0.f,
                            WICBitmapPaletteTypeMedianCut
                            );
                    }
                }
            }
        }
        else
        {
                    
            hr = pConverter->Initialize(
                pSource,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
        }
    }
    if (SUCCEEDED(hr))
    {
        //create a Direct2D bitmap from the WIC bitmap.
        hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            ppBitmap
            );
    
    }

    SafeRelease(&&pDecoder);
    SafeRelease(&&pSource);
    SafeRelease(&&pStream);
    SafeRelease(&&pConverter);
    SafeRelease(&&pScaler);

    return hr;
}*/

//
// Creates a Direct2D bitmap from the specified
// file name.
//
HRESULT ImageLoader::LoadBitmapFromFile(
    PCWSTR uri,
    ID2D1Bitmap **ppBitmap,
	int destinationWidth,
	int destinationHeight,
	IWICFormatConverter** pConverter
    )
{
    IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pSource = NULL;
    IWICStream *pStream = NULL;
    IWICBitmapScaler *pScaler = NULL;

    HRESULT hr = m_pWICFactory->CreateDecoderFromFilename(
        uri,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
        );
        
    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }
    if (SUCCEEDED(hr))
    {

        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = m_pWICFactory->CreateFormatConverter(pConverter);

    }
 
 
    if (SUCCEEDED(hr))
    {
        // If a new width or height was specified, create an
        // IWICBitmapScaler and use it to resize the image.
        if (destinationWidth != 0 || destinationHeight != 0)
        {
            UINT originalWidth, originalHeight;
            hr = pSource->GetSize(&originalWidth, &originalHeight);
            if (SUCCEEDED(hr))
            {
                if (destinationWidth == 0)
                {
                    FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                    destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
                }
                else if (destinationHeight == 0)
                {
                    FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                    destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
                }

                hr = m_pWICFactory->CreateBitmapScaler(&pScaler);
                if (SUCCEEDED(hr))
                {
                    hr = pScaler->Initialize(
                            pSource,
                            destinationWidth,
                            destinationHeight,
                            WICBitmapInterpolationModeCubic
                            );
                }
                if (SUCCEEDED(hr))
                {
                    hr = (*pConverter)->Initialize(
                        pScaler,
                        GUID_WICPixelFormat32bppPBGRA,
                        WICBitmapDitherTypeNone,
                        NULL,
                        0.f,
                        WICBitmapPaletteTypeMedianCut
                        );
                }
            }
        }
        else // Don't scale the image.
        {
            hr = (*pConverter)->Initialize(
                pSource,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
        }
    }
    if (SUCCEEDED(hr))
    {
    
        // Create a Direct2D bitmap from the WIC bitmap.
		hr = BX2D->GetBackBuffer()->CreateBitmapFromWicBitmap(
            (*pConverter),
            ppBitmap
            );
    }

    SafeRelease(pDecoder);
    SafeRelease(pSource);
    SafeRelease(pStream);
    SafeRelease(pScaler);

    return hr;
}