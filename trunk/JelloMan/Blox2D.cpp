//---------------------------
// Includes
//---------------------------
#include "Blox2D.h"
#include "HitRegion.h"
#include "Bitmap.h"
#include "Button.h"

//-----------------------------------------------------------------
// Static Variable Initialization
//-----------------------------------------------------------------
Blox2D* Blox2D::m_pSingleton = NULL;

//---------------------------
// Constructor & Destructor
//---------------------------
Blox2D::Blox2D()	:	m_pRenderTarget(0),
						m_pD2DFactory(0),
						m_pDWriteFactory(0),
						m_pColorBrush(0),
						m_pTextFormat(0),
						m_FrameCount(0),
						m_TBase(0),
						m_GameTime(0),
						m_FPS(0),
						m_DTimeS(0),
						m_pWICFactory(0)
{
	CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&m_pWICFactory
			);

	m_HorizontalAlignment = HORIZONTAL_ALIGN_CENTER;
	m_VerticalAlignment = VERTICAL_ALIGN_MIDDLE;
}

Blox2D::~Blox2D()
{
	m_pSingleton = 0;
	SafeRelease(m_pWICFactory);
}

// STATIC METHOD
Blox2D* Blox2D::GetSingleton()
{
	if (m_pSingleton == NULL) m_pSingleton = new Blox2D();
	return m_pSingleton;
}

// SETTERS
void Blox2D::SetColor(int r, int g, int b, float a)
{
	D2D_COLOR_F temp;
	temp.r = (static_cast<float>(r/255.0f));
	temp.g = (static_cast<float>(g/255.0f));
	temp.b = (static_cast<float>(b/255.0f));
	temp.a = a;

	if (m_pColorBrush->GetColor().r != temp.r ||
		m_pColorBrush->GetColor().g != temp.g || 
		m_pColorBrush->GetColor().b != temp.b ||
		m_pColorBrush->GetColor().a != temp.a)
	m_pColorBrush->SetColor(temp);
}

void Blox2D::SetColor(D2D1_COLOR_F color)
{
	m_pColorBrush->SetColor(color);
}

void Blox2D::SetFont(tstring const& fontName, bool bold, bool italic, float size)
{
	SafeRelease(m_pTextFormat);

	DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
	DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL;

	if (bold) weight = DWRITE_FONT_WEIGHT_BOLD;
	if (italic) style = DWRITE_FONT_STYLE_ITALIC;

	m_pDWriteFactory->CreateTextFormat(
		fontName.c_str(),
            NULL,
            weight,
            style,
            DWRITE_FONT_STRETCH_NORMAL,
            size,
            L"", //locale
            &m_pTextFormat
            );
}

void Blox2D::SetFont(IDWriteTextFormat* textFormat)
{
	m_pTextFormat = textFormat;
}

void Blox2D::SetTransform(D2D1_MATRIX_3X2_F transform)
{
	m_pRenderTarget->SetTransform(transform);
}

void Blox2D::ResetTransform()
{
	m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
}

void Blox2D::SetParams(	ID2D1RenderTarget* pRenderTarget,
						ID2D1Factory* pD2DFactory,
						IDWriteFactory* pDWriteFactory,
						ID2D1SolidColorBrush* pColorBrush,
						IDWriteTextFormat* pTextFormat)
{
	m_pRenderTarget = pRenderTarget;
	m_pD2DFactory = pD2DFactory;
	m_pDWriteFactory = pDWriteFactory;
	m_pColorBrush = pColorBrush;
	m_pTextFormat = pTextFormat;
}

void Blox2D::SetAntiAliasing(bool AA)
{
	if (AA)
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	else
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
}

// GETTERS
Size2D Blox2D::GetWindowSize() const
{
	return m_pRenderTarget->GetSize();
}

// -------------------------------------
// BLOX_2D DRAW METHODS	
// -------------------------------------
void Blox2D::DrawGrid(int stepsize, D2D1_RECT_F area) const
{
	for (int x = static_cast<int>(area.left); x < area.right; x += stepsize)
	{
		m_pRenderTarget->DrawLine(	Point2F(static_cast<float>(x), area.top),
									Point2F(static_cast<float>(x),area.bottom),
									m_pColorBrush);
	}

	for (int y = static_cast<int>(area.top); y < area.bottom; y += stepsize)
	{
		m_pRenderTarget->DrawLine(	Point2F(area.left, static_cast<float>(y)),
									Point2F(area.right, static_cast<float>(y)),
									m_pColorBrush);
	}
}

void Blox2D::FillBackGround() const
{
	// Retrieve the size of the render target.
    D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

	m_pRenderTarget->FillRectangle(RectF(0,0,rtSize.width,rtSize.height),m_pColorBrush);
}

void Blox2D::DrawLine(int x, int y, int x2, int y2, float strokeSize) const
{
	m_pRenderTarget->DrawLine(Point2F(static_cast<float>(x),static_cast<float>(y)),Point2F(static_cast<float>(x2),static_cast<float>(y2)),m_pColorBrush,strokeSize);
}

void  Blox2D::DrawLine(D2D1_POINT_2F start, D2D1_POINT_2F end, float strokeSize) const
{
	m_pRenderTarget->DrawLine(start,end,m_pColorBrush,strokeSize);
}
void Blox2D::DrawEllipse(int x, int y, int width, int height, float strokeSize) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = static_cast<float>(x);
	temp.point.y = static_cast<float>(y);
	temp.radiusX = static_cast<float>(width);
	temp.radiusY = static_cast<float>(height);

	m_pRenderTarget->DrawEllipse(temp,m_pColorBrush,strokeSize);
}

void Blox2D::DrawEllipse(D2D1_POINT_2F coord, int width, int height, float strokeSize) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = static_cast<float>(coord.x);
	temp.point.y = static_cast<float>(coord.y);
	temp.radiusX = static_cast<float>(width);
	temp.radiusY = static_cast<float>(height);

	m_pRenderTarget->DrawEllipse(temp,m_pColorBrush,strokeSize);
}

void Blox2D::DrawEllipse(D2D1_ELLIPSE ellipse, float strokeSize) const
{
	m_pRenderTarget->BeginDraw();

	m_pRenderTarget->DrawEllipse(ellipse,m_pColorBrush,strokeSize);

	m_pRenderTarget->EndDraw();
}

void Blox2D::FillEllipse(int x, int y, int width, int height) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = static_cast<float>(x);
	temp.point.y = static_cast<float>(y);
	temp.radiusX = static_cast<float>(width);
	temp.radiusY = static_cast<float>(height);

	m_pRenderTarget->FillEllipse(temp,m_pColorBrush);
}

void Blox2D::FillEllipse(D2D1_POINT_2F coord, int width, int height) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = static_cast<float>(coord.x);
	temp.point.y = static_cast<float>(coord.y);
	temp.radiusX = static_cast<float>(width);
	temp.radiusY = static_cast<float>(height);

	m_pRenderTarget->FillEllipse(temp,m_pColorBrush);
}

void Blox2D::FillEllipse(D2D1_ELLIPSE ellipse) const
{
	m_pRenderTarget->FillEllipse(ellipse,m_pColorBrush);
}

void Blox2D::DrawStringCentered(tstring const& text, int offSetX, int offSetY) const
{
	// Retrieve the size of the render target.
    D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
	D2D1_RECT_F rect = RectF(0,0,rtSize.width+(offSetX*2),rtSize.height+(offSetY*2));

	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	
	m_pRenderTarget->DrawTextW(text.c_str(),text.size(),m_pTextFormat,rect,m_pColorBrush);
}

void Blox2D::DrawString(tstring const& text, int x, int y) const
{
	// Retrieve the size of the render target.
    D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	m_pRenderTarget->DrawTextW(text.c_str(),text.size(),m_pTextFormat,RectF(static_cast<float>(x),static_cast<float>(y),rtSize.width,rtSize.height),m_pColorBrush);
}

void Blox2D::DrawString(tstring const& text, D2D1_RECT_F rect, HORIZONTAL_ALIGN textAlignment, VERTICAL_ALIGN paragraphAlignment)
{
	if (textAlignment == HORIZONTAL_ALIGN_CENTER) m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	else if (textAlignment == HORIZONTAL_ALIGN_LEFT) m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	else if (textAlignment == HORIZONTAL_ALIGN_RIGHT) m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

	if (paragraphAlignment == VERTICAL_ALIGN_MIDDLE) m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	else if (paragraphAlignment == VERTICAL_ALIGN_TOP) m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	else if (paragraphAlignment == VERTICAL_ALIGN_BOTTOM) m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

	m_pRenderTarget->DrawTextW(text.c_str(),text.size(),m_pTextFormat,RectF(rect.left,rect.top,rect.right,rect.bottom),m_pColorBrush);
}

/*void  Blox2D::DrawString(int nr, D2D1_RECT_F rect, TEXT_ALIGNMENT textAlignment, PARAGRAPH_ALIGNMENT paragraphAlignment)
{
	tstringstream stream;
	stream << nr;

	BLOX_2D->DrawString(stream.str(),rect,textAlignment,paragraphAlignment);
}*/

void Blox2D::ShowFPS(float dTime, bool showGraph, float delayInterval)
{
	m_FrameCount++;
	m_GameTime += dTime;

	// Compute averages over one second period.
	if( (m_GameTime - m_TBase) >= delayInterval )
	{
		float fps = static_cast<float>(m_FrameCount*(1/delayInterval)); // fps = frameCnt / 1

		m_FPS = static_cast<int>(fps);
		
		// Reset for next average.
		m_FrameCount = 0;
		m_TBase  += delayInterval;

		m_DTimeS = dTime*1000;

		if (showGraph)
		{
			if (m_FPS > 80) m_fpsHistory.push_back(80);
			else if (m_FPS < 0) m_fpsHistory.push_back(0);
			else m_fpsHistory.push_back(m_FPS);

			if (m_DTimeS > 80) m_dtimeHistory.push_back(80);
			else if (m_DTimeS < 0) m_dtimeHistory.push_back(0);
			else m_dtimeHistory.push_back(m_DTimeS);
		}
	}

	tstringstream outs;
	outs.precision(5);

	outs << L"FPS: " << m_FPS << L"\n";
	outs << L"DTime: " << m_DTimeS << L" ms\n";

	SetFont(_T("Consolas"),true,false,12);
	DrawString(outs.str(),RectF(GetWindowSize().width-110,0,GetWindowSize().width,95),HORIZONTAL_ALIGN_LEFT,VERTICAL_ALIGN_BOTTOM);

	if (showGraph && m_GameTime > 1)
	{
		SetColor(255,255,255,0.4f);
		FillRect(static_cast<int>(GetWindowSize().width) - 105, 5, 100, 40);

		if (m_fpsHistory.size() > 26) m_fpsHistory.erase(m_fpsHistory.begin());
		if (m_dtimeHistory.size() > 26) m_dtimeHistory.erase(m_dtimeHistory.begin());

		SetColor(255,0,0,0.5f);
		for (unsigned int i = 0; i < m_fpsHistory.size()-1; ++i)
		{
			DrawLine(
				static_cast<int>((GetWindowSize().width-5)-(i*4)),
				static_cast<int>(45-(m_fpsHistory.at(i)/2)),
				static_cast<int>((GetWindowSize().width-5)-((i+1)*4)),
				static_cast<int>(45-(m_fpsHistory.at(i+1)/2)),
				2.0f);
		}
		DrawString(_T("fps"),static_cast<int>(GetWindowSize().width-128),10);

		SetColor(255,255,0,0.5f);
		for (unsigned int i = 0; i < m_dtimeHistory.size()-1; ++i)
		{
			DrawLine(
				static_cast<int>((GetWindowSize().width-5)-(i*4)),
				static_cast<int>((45-(m_dtimeHistory.at(i)/2))),
				static_cast<int>(((GetWindowSize().width-5)-(i+1)*4)),
				static_cast<int>((45-(m_dtimeHistory.at(i+1)/2))),
				2.0f);
		}
		DrawString(_T("dt"),static_cast<int>(GetWindowSize().width-122),25);	

		SetColor(43,43,43,0.5f);
		DrawRect(static_cast<int>(GetWindowSize().width-105),5,100,40);
	}
}

void Blox2D::DrawRect(int x, int y, int width, int height, float strokeSize) const
{
	m_pRenderTarget->DrawRectangle(RectF(static_cast<float>(x),static_cast<float>(y),static_cast<float>(x+width),static_cast<float>(y+height)),m_pColorBrush,strokeSize);
}

void Blox2D::DrawRect(D2D1_RECT_F rect, float strokeSize) const
{
	m_pRenderTarget->DrawRectangle(rect,m_pColorBrush, strokeSize);
}

void Blox2D::FillRect(int x, int y, int width, int height) const
{
	m_pRenderTarget->FillRectangle(RectF(static_cast<float>(x),static_cast<float>(y),static_cast<float>(x+width),static_cast<float>(y+height)),m_pColorBrush);
}

void Blox2D::FillRect(D2D1_RECT_F rect) const
{
	m_pRenderTarget->FillRectangle(rect,m_pColorBrush);
}

void Blox2D::FillBlock(int x, int y, int size)
{
	SetColor(64,64,64);
	for(int i = 0; i < (size/4); ++i)
	{
		FillRect(x-i,y-i,size,size);
	}

	SetColor(225,225,225);
	FillRect(x,y,size,size);
}

void Blox2D::FillBlock(D2D1_POINT_2F coord, int size,D2D1_COLOR_F color1, D2D1_COLOR_F color2)
{
	SetColor(static_cast<int>(color2.r),static_cast<int>(color2.g),static_cast<int>(color2.b,color2.a));
	for(int i = 0; i < (size/4); ++i)
	{
		FillRect(static_cast<int>(coord.x-i),static_cast<int>(coord.y-i),size,size);
	}

	SetColor(static_cast<int>(color1.r),static_cast<int>(color1.g),static_cast<int>(color1.b,color1.a));
	FillRect(static_cast<int>(coord.x),static_cast<int>(coord.y),size,size);
}

void Blox2D::DrawPolygon(D2D1_POINT_2F pArr[], int nrPoints, bool close, float strokeSize) const
{
	ID2D1PathGeometry* pPathGeometry = NULL;
	m_pD2DFactory->CreatePathGeometry(&pPathGeometry);

	ID2D1GeometrySink* pSink = NULL;
	pPathGeometry->Open(&pSink);

	pSink->BeginFigure(pArr[0],D2D1_FIGURE_BEGIN_HOLLOW);

	pSink->AddLines(pArr,nrPoints);
	
	if (close) pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	else pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	
	pSink->Close();
	SafeRelease(pSink);

	m_pRenderTarget->DrawGeometry(pPathGeometry,m_pColorBrush,strokeSize);
}

void Blox2D::FillPolygon(D2D1_POINT_2F pArr[], int nrPoints) const
{
	ID2D1PathGeometry* pPathGeometry = NULL;
	m_pD2DFactory->CreatePathGeometry(&pPathGeometry);

	ID2D1GeometrySink* pSink = NULL;
	pPathGeometry->Open(&pSink);

	pSink->BeginFigure(pArr[0],D2D1_FIGURE_BEGIN_FILLED);

	pSink->AddLines(pArr,nrPoints);

	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	
	pSink->Close();
	SafeRelease(pSink);

	m_pRenderTarget->FillGeometry(pPathGeometry,m_pColorBrush);

	SafeRelease(pPathGeometry);
}

void Blox2D::DrawBitmap(Bitmap* bitmap, int x, int y, float opacity, int width, int height)
{
	D2D1_RECT_F rect;

	rect.left = static_cast<float>(x);
	rect.top = static_cast<float>(y);

	if (width == 0 && height == 0)
	{
		rect.right = static_cast<float>(x + bitmap->GetSize().width);
		rect.bottom = static_cast<float>(y + bitmap->GetSize().height);
	}
	else if (width == 0 && height != 0)
	{
		rect.right = static_cast<float>(x + bitmap->GetSize().width);
		rect.bottom = static_cast<float>(y + height);
	}
	else if (width != 0 && height == 0)
	{
		rect.right = static_cast<float>(x + width);
		rect.bottom = static_cast<float>(y + bitmap->GetSize().height);
	}
	else
	{
		rect.right = static_cast<float>(x + width);
		rect.bottom = static_cast<float>(y + height);
	}

	BITMAP_INFO info = bitmap->GetInfo();

	if (!bitmap->Exists())
	{
		tstringstream stream;
		stream << _T("Can't draw bitmap: ") << info.filepath;

		//BLOX_2D->MsgBox(stream.str().c_str(),_T("ERROR"));
		return;
	}
	else 
		m_pRenderTarget->DrawBitmap(info.bitmap,rect,opacity);
}

void Blox2D::DrawRoundRect(int x, int y, int width, int height, int radius, float strokeSize) const
{
	D2D1_ROUNDED_RECT roundRect;
	D2D1_RECT_F rect;

	rect.left = static_cast<float>(x);
	rect.right = static_cast<float>(x + width);
	rect.top = static_cast<float>(y);
	rect.bottom = static_cast<float>(y + height);

	roundRect.radiusX = static_cast<float>(radius);
	roundRect.radiusY = static_cast<float>(radius);
	roundRect.rect = rect;

	m_pRenderTarget->DrawRoundedRectangle(roundRect, m_pColorBrush, strokeSize);
}

void Blox2D::DrawRoundRect(D2D1_ROUNDED_RECT roundRect, float strokeSize) const
{
	m_pRenderTarget->DrawRoundedRectangle(roundRect, m_pColorBrush, strokeSize);
}

void Blox2D::FillRoundRect(int x, int y, int width, int height, int radius) const
{
	D2D1_ROUNDED_RECT roundRect;
	D2D1_RECT_F rect;

	rect.left = static_cast<float>(x);
	rect.right = static_cast<float>(x + width);
	rect.top = static_cast<float>(y);
	rect.bottom = static_cast<float>(y + height);

	roundRect.radiusX = static_cast<float>(radius);
	roundRect.radiusY = static_cast<float>(radius);
	roundRect.rect = rect;

	m_pRenderTarget->FillRoundedRectangle(roundRect, m_pColorBrush);
}

void Blox2D::FillRoundRect(D2D1_ROUNDED_RECT roundRect) const
{
	m_pRenderTarget->FillRoundedRectangle(roundRect, m_pColorBrush);
}

HRESULT Blox2D::LoadBitmapFromFile(
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap **ppBitmap
    )
{
    IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pSource = NULL;
    IWICStream *pStream = NULL;
    IWICFormatConverter *pConverter = NULL;
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
	else
	{
		tstringstream stream;
		stream << _T("Can't create bitmap: ") << uri;
		
		//MsgBox(stream.str(),_T("ERROR"));
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
                }
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
        else // Don't scale the image.
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
    
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            ppBitmap
            );
    }

    SafeRelease(pDecoder);
    SafeRelease(pSource);
    SafeRelease(pStream);
    SafeRelease(pConverter);
    SafeRelease(pScaler);

    return hr;
}