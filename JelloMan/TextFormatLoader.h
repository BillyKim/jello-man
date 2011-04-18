#pragma once

#include "D3DUtil.h"
#include "AssetContainer.h"

class TextFormat;

class TextFormatLoader
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	TextFormatLoader();
	virtual ~TextFormatLoader();

	// GENERAL
	TextFormat* LoadTextFormat(const tstring& fontName, float size, bool bold = false, bool italic = false);

private:

	HRESULT CreateWriteFactory();

	// DATAMEMBERS
	IDWriteFactory* m_pDWriteFactory;

	AssetContainer<TextFormat>* m_pAssetContainer;

	// DISABLE DEFAULT COPY & ASSIGNMENT OPERATOR
	TextFormatLoader(const TextFormatLoader& yRef);
	TextFormatLoader& operator=(const TextFormatLoader& yRef);
};

