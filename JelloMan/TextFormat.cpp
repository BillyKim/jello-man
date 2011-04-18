#include "TextFormat.h"

// CONSTRUCTOR - DESTRUCTOR
TextFormat::TextFormat(IDWriteTextFormat* pDWriteTextFormat)	:	m_pTextFormat(pDWriteTextFormat)
{
}

TextFormat::~TextFormat()
{
}

// DEFAULT COPY & ASSIGNMENT OPERATOR
TextFormat::TextFormat(const TextFormat& yRef)
{
	m_pTextFormat = yRef.m_pTextFormat;
}

TextFormat& TextFormat::operator=(const TextFormat& yRef)
{
	m_pTextFormat = yRef.m_pTextFormat;

	return *this;
}

// SETTERS
void TextFormat::SetHorizontalAlignment(TEXT_ALIGNMENT horizontalAlignment)
{
	m_pTextFormat->SetTextAlignment(horizontalAlignment);
}

void TextFormat::SetVerticalAlignment(PARAGRAPH_ALIGNMENT verticalAlignment)
{
	m_pTextFormat->SetParagraphAlignment(verticalAlignment);
}

// GETTERS
TextFormatInfo TextFormat::GetTextFormatInfo() const
{
	TextFormatInfo info;

	if (m_pTextFormat->GetFontWeight() == DWRITE_FONT_WEIGHT_BOLD)
		info.Bold = true;
	else
		info.Bold = false;

	if (m_pTextFormat->GetFontStyle() == DWRITE_FONT_STYLE_ITALIC)
		info.Italic = true;
	else
		info.Italic = false;

	info.FontSize = m_pTextFormat->GetFontSize();

	/*wchar_t* wcName = 0;
	m_pTextFormat->GetFontFamilyName(wcName, m_pTextFormat->GetFontFamilyNameLength());

	info.FontName = wcName;*/

	return info;
}