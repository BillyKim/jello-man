#include "TextFormatLoader.h"
#include "TextFormat.h"

// CONSTRUCTOR - DESTRUCTOR
TextFormatLoader::TextFormatLoader()	:	m_pDWriteFactory(0),
											m_pAssetContainer(new AssetContainer<TextFormat>())
{
	if (FAILED(CreateWriteFactory()))
	{
		MessageBox(0,_T("Error creating DWriteFactory!"),_T("Error!"),MB_OK);
		exit(-1);
	}
}

TextFormatLoader::~TextFormatLoader()
{
	SafeRelease(m_pDWriteFactory);

	delete m_pAssetContainer;
}

HRESULT TextFormatLoader::CreateWriteFactory()
{
	HRESULT hr;
    // Create a DirectWrite factory.
    hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_pDWriteFactory),
            reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
            );
	return hr;
}

// GENERAL
TextFormat* TextFormatLoader::LoadTextFormat(const tstring& fontName, float size, bool bold, bool italic)
{
    tstringstream streamId;
    streamId << fontName << size << bold << italic;

	if (m_pAssetContainer->IsAssetPresent(streamId.str()))
	{
		TextFormat* pTempFormat = m_pAssetContainer->GetAsset(streamId.str());		
		return pTempFormat;
	}
    else
    {
	    IDWriteTextFormat* pDWriteTextformat = 0;

	    DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
	    DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL;

	    if (bold) weight = DWRITE_FONT_WEIGHT_BOLD;
	    if (italic) style = DWRITE_FONT_STYLE_ITALIC;

	    HRESULT hr = m_pDWriteFactory->CreateTextFormat(
		    fontName.c_str(),
            NULL,
            weight,
            style,
            DWRITE_FONT_STRETCH_NORMAL,
            size,
            L"", //locale
            &pDWriteTextformat
            );

	    TextFormat* pTextFormat = 0;

	    if (SUCCEEDED(hr))
	    {
		    pTextFormat = new TextFormat(pDWriteTextformat);
		    pTextFormat->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		    pTextFormat->SetVerticalAlignment(PARAGRAPH_ALIGNMENT_TOP);

		    m_pAssetContainer->AddAsset(streamId.str(), pTextFormat);
	    }
	    else
	    {
		    MessageBox(0, _T("Error creating new textformat!"), _T("Error!"), MB_OK);
		    exit(-1);
	    }

	    return pTextFormat;
    }
}