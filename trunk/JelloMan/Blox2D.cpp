//---------------------------
// Includes
//---------------------------
#include "Blox2D.h"


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



//-----------------------------------------------------------------
// HitRegion Class
//-----------------------------------------------------------------
HitRegion::HitRegion(int type, int x, int y, int width, int height)	:	m_pGeometry(0),
																		m_pTransformedGeometry(0)
{
	m_Type = type;
	m_Width = width;
	m_Height = height;

	m_CurrentPos.x = static_cast<float>(x);
	m_CurrentPos.y = static_cast<float>(y);

	if (type == TYPE_RECTANGLE)
	{
		ID2D1RectangleGeometry* RG = dynamic_cast<ID2D1RectangleGeometry*>(m_pGeometry);

		BLOX_2D->GetFactory()->CreateRectangleGeometry(
		RectF(0,0,(float)(width),(float)(height)),&RG);

		SafeRelease(m_pGeometry);
		m_pGeometry = RG;
	}
	else if (type == TYPE_ELLIPSE)
	{
		ID2D1EllipseGeometry* EG = dynamic_cast<ID2D1EllipseGeometry*>(m_pGeometry);

		BLOX_2D->GetFactory()->CreateEllipseGeometry(
		Ellipse(Point2F(0,0),(float)width,(float)height),&EG);

		SafeRelease(m_pGeometry);
		m_pGeometry = EG;
	}

	SetPosition(x,y);
}

HitRegion::HitRegion(int type, D2D1_POINT_2F* points, int nrPoints)	:	m_pGeometry(0),
																		m_pTransformedGeometry(0)
{
	if (type == TYPE_POLYGON)
	{
		ID2D1PathGeometry* PG = dynamic_cast<ID2D1PathGeometry*>(m_pGeometry);

		BLOX_2D->GetFactory()->CreatePathGeometry(&PG);

		ID2D1GeometrySink* pSink = NULL;
		PG->Open(&pSink);

		pSink->BeginFigure(points[0],D2D1_FIGURE_BEGIN_FILLED);

		pSink->AddLines(points,nrPoints);

		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	
		pSink->Close();
		SafeRelease(pSink);

		SafeDelete(m_pGeometry);
		m_pGeometry = PG;

		SetPosition(0,0);
	}
}

HitRegion::~HitRegion()
{
	SafeRelease(m_pGeometry);
	SafeRelease(m_pTransformedGeometry);
}

// default copy constructor
HitRegion::HitRegion(const HitRegion& second)
{
	/*if (second.m_Type == TYPE_RECTANGLE)
	{
		m_pHitRegionRECT = second.m_pHitRegionRECT;
		
		m_Type = TYPE_RECTANGLE;
		m_CurrentPos = second.m_CurrentPos;
		m_Width = second.m_Width;
		m_Height = second.m_Height;
	}

	else if (second.m_Type == TYPE_ELLIPSE)
	{
		m_pHitRegionELLIPSE = second.m_pHitRegionELLIPSE;

		m_Type = TYPE_ELLIPSE;
		m_CurrentPos = second.m_CurrentPos;
		m_Width = second.m_Width;
		m_Height = second.m_Height;
	}

	else if (second.m_Type == TYPE_RECTANGLE)
	{
		m_pHitRegionPOLYGON = second.m_pHitRegionPOLYGON;

		m_Type = TYPE_POLYGON;
		m_CurrentPos = second.m_CurrentPos;
		m_Width = 0;
		m_Height = 0;
	}*/
}

// dafault assignment operator
HitRegion& HitRegion::operator=(const HitRegion& second)
{
	/*if (second.m_Type == TYPE_RECTANGLE)
	{
		BLOX_2D->GetFactory()->CreateRectangleGeometry
			(RectF((float)second.m_CurrentPos.x,(float)second.m_CurrentPos.y,
			(float)(second.m_CurrentPos.x+second.m_Width),
			(float)(second.m_CurrentPos.x+second.m_Height)),&this->m_pHitRegionRECT);
		
		this->m_Type = TYPE_RECTANGLE;
		this->m_CurrentPos = second.m_CurrentPos;
		this->m_Width = second.m_Width;
		this->m_Height = second.m_Height;

		return *this;
	}

	else if (second.m_Type == TYPE_ELLIPSE)
	{
		BLOX_2D->GetFactory()->CreateEllipseGeometry
			(Ellipse(Point2F((float)second.m_CurrentPos.x,(float)second.m_CurrentPos.y),
			(float)second.m_Width,(float)second.m_Height),&this->m_pHitRegionELLIPSE);
	
		this->m_Type = TYPE_ELLIPSE;
		this->m_CurrentPos = second.m_CurrentPos;
		this->m_Width = second.m_Width;
		this->m_Height = second.m_Height;

		return *this;
	}*/

	return *this;
}

// getters
bool HitRegion::HitTest(HitRegion* hitRect, bool draw)
{
	/*if (m_Type == TYPE_RECTANGLE)
	{
		if (hitRect->m_Type == TYPE_RECTANGLE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionRECT->CompareWithGeometry(hitRect->m_pHitRegionRECT,m_matWorld,10.0f,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}

		else if (hitRect->m_Type == TYPE_ELLIPSE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionRECT->CompareWithGeometry(hitRect->m_pHitRegionELLIPSE,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}

		else if (hitRect->m_Type == TYPE_POLYGON)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionRECT->CompareWithGeometry(hitRect->m_pHitRegionPOLYGON,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}
	}

	else if (m_Type == TYPE_ELLIPSE)
	{
		if (hitRect->m_Type == TYPE_RECTANGLE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionELLIPSE->CompareWithGeometry(hitRect->m_pHitRegionRECT,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}

		else if (hitRect->m_Type == TYPE_ELLIPSE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionELLIPSE->CompareWithGeometry(hitRect->m_pHitRegionELLIPSE,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}

		else if (hitRect->m_Type == TYPE_POLYGON)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionELLIPSE->CompareWithGeometry(hitRect->m_pHitRegionPOLYGON,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}
	}

	else if (m_Type == TYPE_POLYGON)
	{
		if (hitRect->m_Type == TYPE_RECTANGLE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionPOLYGON->CompareWithGeometry(hitRect->m_pHitRegionRECT,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}

		else if (hitRect->m_Type == TYPE_ELLIPSE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionPOLYGON->CompareWithGeometry(hitRect->m_pHitRegionELLIPSE,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}

		else if (hitRect->m_Type == TYPE_POLYGON)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionPOLYGON->CompareWithGeometry(hitRect->m_pHitRegionPOLYGON,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
			else return true;
		}
	}

	return false;*/

	D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

	m_pGeometry->CompareWithGeometry(hitRect->m_pGeometry,Matrix3x2F::Identity(),0.0f,&result);

	if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return false;
	else return true;
}

bool HitRegion::HitTest(D2D1_POINT_2F pos)
{
	/*if (m_Type == TYPE_ELLIPSE)
	{
		BOOL contains;

		m_pHitRegionELLIPSE->FillContainsPoint(pos,&m_matWorld,&contains);

		if (contains == TRUE) return true;
		else return false;
	}

	else if (m_Type == TYPE_RECTANGLE)
	{
		BOOL contains;

		m_pHitRegionRECT->FillContainsPoint(pos,&m_matWorld,&contains);

		if (contains == TRUE) return true;
		else return false;
	}

	else if (m_Type == TYPE_POLYGON)
	{
		BOOL contains;

		m_pHitRegionPOLYGON->FillContainsPoint(pos,&m_matWorld,&contains);

		if (contains == TRUE) return true;
		else return false;
	}

	return false;*/

	BOOL contains;

	m_pTransformedGeometry->FillContainsPoint(pos,Matrix3x2F::Identity(),&contains);

	if (contains == TRUE) return true;
	else return false;
}

D2D1_POINT_2F HitRegion::CollisionTest(HitRegion* hitRect)
{
	ID2D1GeometrySink* pGeometrySink = NULL;
	ID2D1PathGeometry* pPathGeometryUnion = NULL;

	BLOX_2D->GetFactory()->CreatePathGeometry(&pPathGeometryUnion);

	/*if (m_Type == TYPE_RECTANGLE)
	{
		if (hitRect->m_Type == TYPE_RECTANGLE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionRECT->CompareWithGeometry(hitRect->m_pHitRegionRECT,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT ||result == D2D1_GEOMETRY_RELATION_UNKNOWN) return Point2F(-999999,-999999);
			else
			{
				pPathGeometryUnion->Open(&pGeometrySink);

				m_pHitRegionRECT->CombineWithGeometry(hitRect->m_pHitRegionRECT,D2D1_COMBINE_MODE_UNION,m_matWorld,NULL,pGeometrySink);

				pGeometrySink->Close();
				SafeRelease(pGeometrySink);

				D2D1_RECT_F rect;
				pPathGeometryUnion->GetBounds(NULL,&rect);

				D2D1_POINT_2F point;
				point.x = (rect.left + ((rect.right - rect.left)/2));
				point.y = (rect.top + ((rect.bottom - rect.top)/2));

				return point;
			}
		}

		else if (hitRect->m_Type == TYPE_ELLIPSE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionRECT->CompareWithGeometry(hitRect->m_pHitRegionELLIPSE,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT ||result == D2D1_GEOMETRY_RELATION_UNKNOWN) return Point2F(-999999,-999999);
			else
			{
				pPathGeometryUnion->Open(&pGeometrySink);

				m_pHitRegionRECT->CombineWithGeometry(hitRect->m_pHitRegionELLIPSE,D2D1_COMBINE_MODE_UNION,m_matWorld,NULL,pGeometrySink);

				pGeometrySink->Close();
				SafeRelease(pGeometrySink);

				D2D1_RECT_F rect;
				pPathGeometryUnion->GetBounds(NULL,&rect);

				D2D1_POINT_2F point;
				point.x = (rect.left + (rect.right - rect.left));
				point.y = (rect.top + (rect.bottom - rect.top));

				return point;
			}
		}
	}

	if (m_Type == TYPE_ELLIPSE)
	{
		if (hitRect->m_Type == TYPE_RECTANGLE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionELLIPSE->CompareWithGeometry(hitRect->m_pHitRegionRECT,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT ||result == D2D1_GEOMETRY_RELATION_UNKNOWN) return Point2F(-999999,-999999);
			else
			{
				pPathGeometryUnion->Open(&pGeometrySink);

				m_pHitRegionELLIPSE->CombineWithGeometry(hitRect->m_pHitRegionRECT,D2D1_COMBINE_MODE_UNION,m_matWorld,NULL,pGeometrySink);

				pGeometrySink->Close();
				SafeRelease(pGeometrySink);

				D2D1_RECT_F rect;
				pPathGeometryUnion->GetBounds(NULL,&rect);

				D2D1_POINT_2F point;
				point.x = (rect.left + (rect.right - rect.left));
				point.y = (rect.top + (rect.bottom - rect.top));

				return point;
			}
		}

		else if (hitRect->m_Type == TYPE_ELLIPSE)
		{
			D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

			m_pHitRegionELLIPSE->CompareWithGeometry(hitRect->m_pHitRegionELLIPSE,m_matWorld,&result);

			if (result == D2D1_GEOMETRY_RELATION_DISJOINT ||result == D2D1_GEOMETRY_RELATION_UNKNOWN) return Point2F(-999999,-999999);
			else
			{
				pPathGeometryUnion->Open(&pGeometrySink);

				m_pHitRegionELLIPSE->CombineWithGeometry(hitRect->m_pHitRegionELLIPSE,D2D1_COMBINE_MODE_UNION,m_matWorld,NULL,pGeometrySink);

				pGeometrySink->Close();
				SafeRelease(pGeometrySink);

				D2D1_RECT_F rect;
				pPathGeometryUnion->GetBounds(NULL,&rect);

				D2D1_POINT_2F point;
				point.x = (rect.left + (rect.right - rect.left));
				point.y = (rect.top + (rect.bottom - rect.top));

				return point;
			}
		}
	}*/

	D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;

	m_pGeometry->CompareWithGeometry(hitRect->m_pGeometry,Matrix3x2F::Identity(),&result);

	if (result == D2D1_GEOMETRY_RELATION_DISJOINT || result == D2D1_GEOMETRY_RELATION_UNKNOWN) return Point2F(-999999,-999999);
	else
	{
		pPathGeometryUnion->Open(&pGeometrySink);

		m_pGeometry->CombineWithGeometry(hitRect->m_pGeometry,D2D1_COMBINE_MODE_UNION,Matrix3x2F::Identity(),NULL,pGeometrySink);

		pGeometrySink->Close();
		SafeRelease(pGeometrySink);

		D2D1_RECT_F rect;
		pPathGeometryUnion->GetBounds(NULL, &rect);

		D2D1_POINT_2F point;
		point.x = (rect.left + (rect.right - rect.left));
		point.y = (rect.top + (rect.bottom - rect.top));

		return point;
	}

	return Point2F(-999999,-999999);
}

D2D1_RECT_F HitRegion::GetDimension()
{
	D2D1_RECT_F rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	m_pTransformedGeometry->GetBounds(Matrix3x2F::Identity(),&rect);

	return rect;
}

// setters
void HitRegion::SetPosition(int x, int y)
{
	Matrix3x2F temp = Matrix3x2F::Translation(static_cast<float>(x),static_cast<float>(y));

	SafeRelease(m_pTransformedGeometry);
	BLOX_2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

	m_CurrentPos.x = static_cast<float>(x);
	m_CurrentPos.y = static_cast<float>(y);
}

void HitRegion::SetPosition(D2D1_POINT_2F pos)
{
	Matrix3x2F temp = Matrix3x2F::Translation(pos.x,pos.y);

	SafeRelease(m_pTransformedGeometry);
	BLOX_2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

	m_CurrentPos.x = pos.x;
	m_CurrentPos.y = pos.y;
}

void HitRegion::Move(int x, int y)
{
	Matrix3x2F temp = Matrix3x2F::Translation(m_CurrentPos.x + x, m_CurrentPos.y + y);

	SafeRelease(m_pTransformedGeometry);
	BLOX_2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

	m_CurrentPos.x += x;
	m_CurrentPos.y += y;
}
void HitRegion::SetSize(int width, int height)
{
	Size2D s;
	s.width = (float)width/m_Width;
	s.height = (float)height/m_Height;

	Matrix3x2F temp = D2D1::Matrix3x2F::Translation(
                    m_CurrentPos.x,
					m_CurrentPos.y
                    ) *
                D2D1::Matrix3x2F::Scale(
					s.width,
					s.height
                    );

	SafeRelease(m_pTransformedGeometry);
	BLOX_2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

	/*if (m_Type == TYPE_RECTANGLE)
		BLOX_2D->GetFactory()->CreateRectangleGeometry(
		RectF(0.0f,0.0f,(float)(width),(float)(height)),&m_pHitRegionRECT);
	else if (m_Type == TYPE_ELLIPSE)
		BLOX_2D->GetFactory()->CreateEllipseGeometry(
		Ellipse(Point2F(0.0f,0.0f),(float)width,(float)height),&m_pHitRegionELLIPSE);

	m_matWorld.Identity();
	m_matWorld = m_matWorld.Translation(m_CurrentPos.x,m_CurrentPos.y);*/

	m_Width = width;
	m_Height = height;
}

// general
void HitRegion::Draw(bool fill)
{
	BLOX_2D->SetColor(255,0,0,0.5f);

	D2D1_RECT_F rect;
	m_pTransformedGeometry->GetBounds(Matrix3x2F::Identity(),&rect);

	if (m_Type == TYPE_RECTANGLE)
	{
		if (fill) BLOX_2D->FillRect(rect);
		else BLOX_2D->DrawRect(rect);
	}
	else if (m_Type == TYPE_ELLIPSE)
	{
		if (fill) BLOX_2D->FillEllipse(
			static_cast<int>(rect.left+((rect.right-rect.left)/2)),
			static_cast<int>(rect.top+((rect.bottom-rect.top)/2)),
			static_cast<int>((rect.right-rect.left)/2),
			static_cast<int>((rect.bottom-rect.top)/2));
		else BLOX_2D->DrawEllipse(
			static_cast<int>(rect.left+((rect.right-rect.left)/2)),
			static_cast<int>(rect.top+((rect.bottom-rect.top)/2)),
			static_cast<int>(rect.right-rect.left),
			static_cast<int>(rect.bottom-rect.top));
	}
	else if (m_Type == TYPE_POLYGON)
	{
		
		//m_pHitRegionPOLYGON->Stream(

		//BLOX_2D->FillPolygon
	}
}

//-----------------------------------------------------------------
// Bitmap Class
//-----------------------------------------------------------------
// constructors
Bitmap::Bitmap(tstring filePath)
{
	HRESULT hr;
	hr = BLOX_2D->LoadBitmapFromFile(
		filePath.c_str(),
		0,
		0,
		&m_pBitmap
		);

	if (SUCCEEDED(hr))
	{
		m_FilePath = filePath;
		m_bExists = true;
	}
	else 
	{
		tstringstream stream;
		stream << _T("Can't create bitmap: ") << filePath;

		//BLOX_2D->MsgBox(stream.str().c_str(),_T("ERROR"));

		m_FilePath = _T("");
		m_bExists = false;
	}
}

Bitmap::Bitmap(int IDBitmap)
{
	/*HRESULT hr;
	hr = BLOX_2D->LoadResourceBitmap(
		MAKEINTRESOURCE(IDBitmap),
		L"Image",
		0,
		0,
		&m_pBitmap
		);

	if (SUCCEEDED(hr))
	{
		m_FilePath = _T("EMBEDDED");
		m_bExists = true;
	}
	else 
	{
		m_FilePath = _T("");
		m_bExists = false;
	}*/
}

// destructor
Bitmap::~Bitmap()
{
	SafeRelease(m_pBitmap);
}

// getters

D2D1_SIZE_F Bitmap::GetSize()
{
	if (m_bExists)
		return m_pBitmap->GetSize();
	else
	{
		D2D1_SIZE_F temp;
		temp.width = 0;
		temp.height = 0;

		return temp;
	}
}

BITMAP_INFO Bitmap::GetInfo()
{
	BITMAP_INFO info;

	info.filepath = m_FilePath;
	info.bitmap = m_pBitmap;

	return info;
}

bool Bitmap::Exists()
{
	return m_bExists;
}

//-----------------------------------------------------------------
// Button Class
//-----------------------------------------------------------------

// constructor
Button::Button(int posX, int posY, int width, int height, bool bToggleable)	:	m_bClicked(false),
																				m_pNormalBitmap(0),
																				m_pHoverBitmap(0),
																				m_pDownBitmap(0),
																				m_pDeactivatedBitmap(0),
																				m_pDeactivatedHoverBitmap(0),
																				m_pDeactivatedDownBitmap(0),
																				m_bToggleable(bToggleable),
																				m_bActivated(true),
																				m_bHover(false),
																				m_bClick(false),
																				m_bDown(false)
{
	m_Pos.x = (float)posX;
	m_Pos.y = (float)posY;

	m_Size.width = (float)width;
	m_Size.height = (float)height;

	m_State = STATE_NORMAL;
	m_Mode = MODE_MOUSE;

	m_Opacity = 1.0f;

	m_pHitRect = new HitRegion(HitRegion::TYPE_RECTANGLE,posX,posY,width,height);
}

// destructor
Button::~Button()
{
	delete m_pHitRect;
}

// general
void Button::Tick()
{
	Point2D mousePos = CONTROLS->GetMousePos();

	if (CONTROLS->LeftMBDown())
		m_bClick = true;

	if (CONTROLS->LeftMBUp())
	{
		if (m_bClick)
		{
			if (m_pHitRect->HitTest(mousePos))
			{
				m_bClicked = true;

				if (m_bToggleable)
					m_bActivated = !m_bActivated;
			}
			else
			{
				m_bClicked = false;
			}			
		}
		else
			m_bClicked = false;

		m_bClick = false;
	}

	// setting states
	if (m_Mode == MODE_MOUSE) // if state has been set manual, don't check with mouseaction
	{
		if (m_bActivated)
		{
			if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == false)
			{
				m_State = STATE_HOVER;
				m_bHover = true;
				m_bDown = false;
			}
			else if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == true)
			{
				m_State = STATE_DOWN;
				m_bDown = true;
				m_bHover = false;
			}
			else
			{
				m_State = STATE_NORMAL;
				m_bHover = false;
				m_bDown = false;
			}
		}
		else
		{
			if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == false)
			{
				m_State = STATE_DEACTIVATED_HOVER;
				m_bHover = true;
				m_bDown = false;
			}
			else if (m_pHitRect->HitTest(mousePos) && CONTROLS->LeftMBDown() == true)
			{
				m_State = STATE_DEACTIVATED_DOWN;
				m_bDown = true;
				m_bHover = false;
			}
			else
			{
				m_State = STATE_DEACTIVATED;
				m_bHover = false;
				m_bDown = false;
			}
		}
	}
}

void Button::Show()
{
	Tick();

	if (m_State == STATE_NORMAL)
	{
		if (m_pNormalBitmap != 0 && m_pNormalBitmap->Exists())
			BLOX_2D->DrawBitmap(m_pNormalBitmap,(int)m_Pos.x, (int)m_Pos.y, m_Opacity);
	}
	else if (m_State == STATE_HOVER)
	{
		if (m_pHoverBitmap != 0 && m_pHoverBitmap->Exists())
			BLOX_2D->DrawBitmap(m_pHoverBitmap,(int)m_Pos.x, (int)m_Pos.y, m_Opacity);
	}
	else if (m_State == STATE_DOWN)
	{
		if (m_pDownBitmap != 0 && m_pDownBitmap->Exists())
			BLOX_2D->DrawBitmap(m_pDownBitmap,(int)m_Pos.x, (int)m_Pos.y, m_Opacity);
	}
	else if (m_State == STATE_DEACTIVATED)
	{
		if (m_pDeactivatedBitmap != 0 && m_pDeactivatedBitmap->Exists())
			BLOX_2D->DrawBitmap(m_pDeactivatedBitmap,(int)m_Pos.x, (int)m_Pos.y, m_Opacity);
	}
	else if (m_State == STATE_DEACTIVATED_HOVER)
	{
		if (m_pDeactivatedHoverBitmap != 0 && m_pDeactivatedHoverBitmap->Exists())
			BLOX_2D->DrawBitmap(m_pDeactivatedHoverBitmap,(int)m_Pos.x, (int)m_Pos.y, m_Opacity);
	}
	else if (m_State == STATE_DEACTIVATED_DOWN)
	{
		if (m_pDeactivatedDownBitmap != 0 && m_pDeactivatedDownBitmap->Exists())
			BLOX_2D->DrawBitmap(m_pDeactivatedDownBitmap,(int)m_Pos.x, (int)m_Pos.y, m_Opacity);
	}
}

void Button::ShowHitRegion(bool fill)
{
	if (fill)
	{
		D2D1_RECT_F rect = m_pHitRect->GetDimension();
		BLOX_2D->FillRect(rect);
	}
	else
	{
		D2D1_RECT_F rect = m_pHitRect->GetDimension();
		BLOX_2D->DrawRect(rect);
	}
}

// setters
void Button::SetNormalState(Bitmap* normalState)
{
	m_pNormalBitmap = normalState;
}
void Button::SetHoverState(Bitmap* hoverState)
{
	m_pHoverBitmap = hoverState;
}
void Button::SetDownState(Bitmap* downState)
{
	m_pDownBitmap = downState;
}
void Button::SetDeactivatedState(Bitmap* deacivatedState)
{
	m_pDeactivatedBitmap = deacivatedState;
}
void Button::SetDeactivatedStateHover(Bitmap* deactivatedHover)
{
	m_pDeactivatedHoverBitmap = deactivatedHover;
}
void Button::SetDeactivatedStateDown(Bitmap* deactivatedDown)
{
	m_pDeactivatedDownBitmap = deactivatedDown;
}
void Button::SetState(BUTTON_STATE state)
{
	m_State = state;
	if (state == STATE_DEACTIVATED)
		m_bActivated = false;
}
void Button::SetMode(BUTTON_MODE mode)
{
	m_Mode = mode;
}
void Button::SetPosition(int x, int y)
{
	m_Pos.x = (float)x;
	m_Pos.y = (float)y;

	m_pHitRect->SetPosition(Point2F((float)x,(float)y));
}
void Button::Deactivate()
{
	m_State = STATE_DEACTIVATED;
	m_bActivated = false;
}

// getters
bool Button::Clicked()
{
	return m_bClicked;
}

//-----------------------------------------------------------------
// TextBox Class
//-----------------------------------------------------------------

// constructors
TextBox::TextBox(int posX, int posY, int width, int height)	:	m_Width(width),
																m_Height(height),
																m_Pos(Point2F(static_cast<float>(posX), static_cast<float>(posY))),
																m_pHitRegion(0),
																m_bHasFocus(false),
																m_Text(_T("")),
																m_NewText(_T("")),
																m_pTextFormat(0),
																m_BackColor(ColorF(255,255,255)),
																m_bClick(false),
																m_TextColor(ColorF(0,0,0)),
																m_Time(0)
{
	m_pHitRegion = new HitRegion(HitRegion::TYPE_RECTANGLE, posX, posY, width, height);

	SetFont(_T("Verdana"), false,false, 12);

	m_Timer.Reset();
}

// destructor
TextBox::~TextBox()
{
	delete m_pHitRegion;

	SafeRelease(m_pTextFormat);
}

// general
void TextBox::Show()
{
	Tick();

	BLOX_2D->SetAntiAliasing(false);

	BLOX_2D->SetColor(m_BackColor);
	BLOX_2D->FillRect(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), m_Width, m_Height);

	BLOX_2D->SetColor(0,0,0,0.5f);
	BLOX_2D->DrawRect(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), m_Width, m_Height);

//	BLOX_2D->SetFont(m_pTextFormat);

	/*DWRITE_TRIMMING lol;
	 DWRITE_TRIMMING_GRANULARITY f*/

	if (!m_bHasFocus)
	{
		BLOX_2D->SetColor(m_TextColor);

		BLOX_2D->SetFont(_T("Verdana"), false,false, 12);
		BLOX_2D->DrawString(
		m_Text,
		RectF(m_Pos.x+2, m_Pos.y, m_Pos.x + m_Width-4, m_Pos.y + m_Height),
		Blox2D::HORIZONTAL_ALIGN_LEFT,
		Blox2D::VERTICAL_ALIGN_MIDDLE);
	}
	else
	{
		BLOX_2D->SetColor(120,120,255);
		BLOX_2D->DrawRect(static_cast<int>(m_Pos.x+1), static_cast<int>(m_Pos.y+1), m_Width-2, m_Height-2);

		BLOX_2D->SetColor(m_TextColor);

		tstringstream strm;
		strm << m_NewText;

		if (m_Time >= 0.5f)
		{
			strm << _T("|");

			if (m_Time >= 1.0f)
				m_Time = 0.0f;
		}

		BLOX_2D->SetFont(_T("Verdana"), false,false, 12);
		BLOX_2D->DrawString(
		strm.str(),
		RectF(m_Pos.x+2, m_Pos.y, m_Pos.x + m_Width-4, m_Pos.y + m_Height),
		Blox2D::HORIZONTAL_ALIGN_LEFT,
		Blox2D::VERTICAL_ALIGN_MIDDLE);
	}

	BLOX_2D->SetAntiAliasing(true);
}

void TextBox::Tick()
{
	m_Timer.Tick();

	if (CONTROLS->LeftMBDown())
		m_bClick = true;

	if (CONTROLS->LeftMBUp())
	{
		if (m_bClick)
		{
			if (m_pHitRegion->HitTest(CONTROLS->GetMousePos()))
				m_bHasFocus = true;
			else
				m_bHasFocus = false;

			m_bClick = false;
		}
	}

	if (m_bHasFocus)
	{
		m_Time += m_Timer.GetDeltaTime();

		tstringstream stream;
		stream.str(_T(""));

		if (CONTROLS->IsKeyPressed(VK_NUMPAD0))
			stream << "0";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD1))
			stream << "1";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD2))
			stream << "2";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD3))
			stream << "3";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD4))
			stream << "4";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD5))
			stream << "5";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD6))
			stream << "6";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD7))
			stream << "7";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD8))
			stream << "8";
		else if (CONTROLS->IsKeyPressed(VK_NUMPAD9))
			stream << "9";
		else if (CONTROLS->IsKeyPressed(VK_DECIMAL) || CONTROLS->IsKeyPressed('.'))
			stream << _T(".");

		else if (CONTROLS->IsKeyPressed(VK_BACK))
		{
			int lastChar = m_NewText.length();

			if (lastChar > 1)
				m_NewText.erase(lastChar-1,lastChar-1);
			else
				m_NewText = _T("");
		}

		if (stream.str() != _T(""))
		{
			tstringstream streamFinal;
			streamFinal << m_NewText << stream.str();
			m_NewText = streamFinal.str();
		}

		if (CONTROLS->IsKeyPressed(VK_RETURN))
		{
			m_Text = m_NewText;

			//m_bHasFocus = false;
		}
	}
	else
		m_NewText = m_Text;
}

// getters

// setters
void TextBox::SetBackColor(unsigned int R, unsigned int G, unsigned int B)
{
	m_BackColor = ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B));
}

void TextBox::SetTextColor(unsigned int R, unsigned int G, unsigned int B)
{
	m_TextColor = ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B));
}

void TextBox::SetFont(tstring fontName, bool bold, bool italic, float size)
{
	SafeRelease(m_pTextFormat);

	DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
	DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL;

	if (bold) weight = DWRITE_FONT_WEIGHT_BOLD;
	if (italic) style = DWRITE_FONT_STYLE_ITALIC;

	BLOX_2D->GetWriteFactory()->CreateTextFormat(
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