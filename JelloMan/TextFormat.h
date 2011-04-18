#pragma once

#include "D3DUtil.h"

// DEFINES
#define PARAGRAPH_ALIGNMENT DWRITE_PARAGRAPH_ALIGNMENT
#define PARAGRAPH_ALIGNMENT_TOP DWRITE_PARAGRAPH_ALIGNMENT_NEAR
#define PARAGRAPH_ALIGNMENT_MIDDLE DWRITE_PARAGRAPH_ALIGNMENT_CENTER
#define PARAGRAPH_ALIGNMENT_BOTTOM DWRITE_PARAGRAPH_ALIGNMENT_FAR

#define TEXT_ALIGNMENT DWRITE_TEXT_ALIGNMENT
#define TEXT_ALIGNMENT_LEFT DWRITE_TEXT_ALIGNMENT_LEADING
#define TEXT_ALIGNMENT_CENTER DWRITE_TEXT_ALIGNMENT_CENTER
#define TEXT_ALIGNMENT_RIGHT DWRITE_TEXT_ALIGNMENT_TRAILING

struct TextFormatInfo
{
	tstring FontName;
	float FontSize;
	bool Bold;
	bool Italic;
};

class TextFormat
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	TextFormat(IDWriteTextFormat* pDWriteTextFormat);
	virtual ~TextFormat();

	// DEFAULT COPY & ASSIGNMENT OPERATOR
	TextFormat(const TextFormat& yRef);
	TextFormat& operator=(const TextFormat& yRef);

	// SETTERS
	void SetHorizontalAlignment(TEXT_ALIGNMENT horizontalAlignment);
	void SetVerticalAlignment(PARAGRAPH_ALIGNMENT verticalAlignment);

	// GETTERS
	TextFormatInfo GetTextFormatInfo() const;
	IDWriteTextFormat* GetDWriteTextFormat() const
	{ return m_pTextFormat; }

private:

	// DATAMEMBERS
	IDWriteTextFormat* m_pTextFormat;

};

