#include "Blox2DEngine.h"
#include "HitRegion.h"
#include "Image.h"
#include "Button.h"
#include "TextFormat.h"
#include "ContentManager.h"

/* SINGLETON INIT */
Blox2DEngine* Blox2DEngine::m_pSingleton = NULL;

/* CONSTRUCTOR - DESTRUCTOR */
Blox2DEngine::Blox2DEngine()	:	m_pRenderTarget(0),
						m_pD2DFactory(0),
						m_pColorBrush(0),
						m_pTextFormat(0),
						m_TBase(0),
						m_GameTime(0),
						m_FPS(0),
						m_DTimeS(0),
						m_pFPSFont(0)
{
	
}

Blox2DEngine::~Blox2DEngine()
{
	m_pRenderTarget = 0;
	m_pD2DFactory = 0;
	m_pTextFormat = 0;
	m_pFPSFont = 0;
	m_pSingleton = 0;

	SafeRelease(m_pColorBrush);
}

/* SINGLETON */
Blox2DEngine* Blox2DEngine::GetSingleton()
{
	if (m_pSingleton == NULL) m_pSingleton = new Blox2DEngine();
	return m_pSingleton;
}

/* SETTERS */
void Blox2DEngine::SetColor(int r, int g, int b, float a)
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
	{
		m_pColorBrush->SetColor(temp);
	}
}

void Blox2DEngine::SetColor(D2D1_COLOR_F color)
{
	m_pColorBrush->SetColor(color);
}

void Blox2DEngine::SetFont(TextFormat* pTextFormat)
{
	m_pTextFormat = pTextFormat->GetDWriteTextFormat();
}

void Blox2DEngine::SetTransform(D2D1_MATRIX_3X2_F transform)
{
	m_pRenderTarget->SetTransform(transform);
}

void Blox2DEngine::ResetTransform()
{
	m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
}

void Blox2DEngine::Initialize(ID2D1RenderTarget* pRenderTarget, ID2D1Factory* pD2DFactory)
{
	m_pRenderTarget = pRenderTarget;
	m_pD2DFactory = pD2DFactory;

	// Create a brush.
	m_pRenderTarget->CreateSolidColorBrush(
	D2D1::ColorF(D2D1::ColorF::Black),
	&m_pColorBrush
	);
}

void Blox2DEngine::SetAntiAliasing(bool AA)
{
	if (AA)
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	else
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
}

/* GETTERS */
Size2D Blox2DEngine::GetWindowSize() const
{
	return m_pRenderTarget->GetSize();
}

/* BX2D DRAW METHODS */
void Blox2DEngine::DrawGrid(float stepsize, D2D1_RECT_F area) const
{
	for (float x = area.left; x < area.right; x += stepsize)
	{
		DrawLine(	Point2F(x, area.top),
					Point2F(x,area.bottom));
	}

	for (float y = area.top; y < area.bottom; y += stepsize)
	{
		DrawLine(	Point2F(area.left, y),
					Point2F(area.right, y));
	}
}

void Blox2DEngine::FillBackGround() const
{
	m_pRenderTarget->Clear(m_pColorBrush->GetColor());
}

void Blox2DEngine::DrawLine(float x, float y, float x2, float y2, float strokeSize) const
{
	m_pRenderTarget->DrawLine(Point2F(x, y),Point2F(x2, y2), m_pColorBrush, strokeSize);
}

void  Blox2DEngine::DrawLine(D2D1_POINT_2F start, D2D1_POINT_2F end, float strokeSize) const
{
	m_pRenderTarget->DrawLine(start, end, m_pColorBrush, strokeSize);
}
void Blox2DEngine::DrawEllipse(float x, float y, float width, float height, float strokeSize) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = x;
	temp.point.y = y;
	temp.radiusX = width;
	temp.radiusY = height;

	m_pRenderTarget->DrawEllipse(temp, m_pColorBrush, strokeSize);
}

void Blox2DEngine::DrawEllipse(D2D1_POINT_2F coord, float width, float height, float strokeSize) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = coord.x;
	temp.point.y = coord.y;
	temp.radiusX = width;
	temp.radiusY = height;

	m_pRenderTarget->DrawEllipse(temp, m_pColorBrush, strokeSize);
}

void Blox2DEngine::DrawEllipse(D2D1_ELLIPSE ellipse, float strokeSize) const
{
	m_pRenderTarget->DrawEllipse(ellipse, m_pColorBrush, strokeSize);
}

void Blox2DEngine::FillEllipse(float x, float y, float width, float height) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = x;
	temp.point.y = y;
	temp.radiusX = width;
	temp.radiusY = height;

	m_pRenderTarget->FillEllipse(temp, m_pColorBrush);
}

void Blox2DEngine::FillEllipse(D2D1_POINT_2F coord, float width, float height) const
{
	D2D1_ELLIPSE temp;

	temp.point.x = coord.x;
	temp.point.y = coord.y;
	temp.radiusX = width;
	temp.radiusY = height;

	m_pRenderTarget->FillEllipse(temp, m_pColorBrush);
}

void Blox2DEngine::FillEllipse(D2D1_ELLIPSE ellipse) const
{
	m_pRenderTarget->FillEllipse(ellipse, m_pColorBrush);
}

void Blox2DEngine::DrawStringCentered(tstring const& text, float offSetX, float offSetY) const
{
    D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
	D2D1_RECT_F rect = RectF(0, 0, rtSize.width + (offSetX * 2), rtSize.height + (offSetY * 2));

	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	
	m_pRenderTarget->DrawTextW(text.c_str(), text.size(), m_pTextFormat, rect, m_pColorBrush);
}

void Blox2DEngine::DrawString(tstring const& text, float x, float y) const
{
    D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	m_pRenderTarget->DrawTextW(text.c_str(), text.size(), m_pTextFormat, RectF(x, y, rtSize.width, rtSize.height), m_pColorBrush);
}

void Blox2DEngine::DrawString(tstring const& text, D2D1_RECT_F rect)
{
	m_pRenderTarget->DrawTextW(text.c_str(),text.size(),m_pTextFormat,RectF(rect.left,rect.top,rect.right,rect.bottom),m_pColorBrush);
}

void Blox2DEngine::ShowFPS(float dTime, bool showGraph, float delayInterval)
{
	if (!m_pFPSFont)
		m_pFPSFont = Content->LoadTextFormat(_T("Consolas"), 12, true, false);

	m_GameTime += dTime;

	if( (m_GameTime - m_TBase) >= delayInterval )
	{
		float fps = static_cast<float>(1 / dTime);

		m_FPS = static_cast<int>(fps);
		
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

	m_pFPSFont->SetVerticalAlignment(PARAGRAPH_ALIGNMENT_BOTTOM);
	m_pFPSFont->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);

	SetFont(m_pFPSFont);

	DrawString(outs.str(),RectF(GetWindowSize().width-110,0,GetWindowSize().width,95));

	if (showGraph && m_GameTime > 1)
	{
		SetColor(255,255,255,0.4f);
		FillRect(GetWindowSize().width - 105, 5, 100, 40);

		if (m_fpsHistory.size() > 26) m_fpsHistory.erase(m_fpsHistory.begin());
		if (m_dtimeHistory.size() > 26) m_dtimeHistory.erase(m_dtimeHistory.begin());

		SetColor(255,0,0,0.5f);
		for (unsigned int i = 0; i < m_fpsHistory.size() - 1; ++i)
		{
			DrawLine(
				static_cast<float>((GetWindowSize().width - 5) - (i * 4)),
				static_cast<float>(45 - (m_fpsHistory.at(i) / 2)),
				static_cast<float>((GetWindowSize().width - 5) - ((i + 1) * 4)),
				static_cast<float>(45 - (m_fpsHistory.at(i + 1) / 2)),
				2.0f);
		}
		DrawString(_T("fps"),static_cast<float>(GetWindowSize().width-128),10);

		SetColor(255,255,0,0.5f);
		for (unsigned int i = 0; i < m_dtimeHistory.size() - 1; ++i)
		{
			DrawLine(
				static_cast<float>((GetWindowSize().width - 5) - (i * 4)),
				static_cast<float>((45 - (m_dtimeHistory.at(i) / 2))),
				static_cast<float>(((GetWindowSize().width - 5) - (i + 1) * 4)),
				static_cast<float>((45 - (m_dtimeHistory.at(i + 1) / 2))),
				2.0f);
		}
		DrawString(_T("dt"),GetWindowSize().width - 122, 25);	

		SetColor(43,43,43,0.5f);
		DrawRect(GetWindowSize().width - 105,5,100,40);
	}
}

void Blox2DEngine::DrawRect(float x, float y, float width, float height, float strokeSize) const
{
	m_pRenderTarget->DrawRectangle(RectF(x , y, x + width, y + height), m_pColorBrush, strokeSize);
}

void Blox2DEngine::DrawRect(D2D1_RECT_F rect, float strokeSize) const
{
	m_pRenderTarget->DrawRectangle(rect, m_pColorBrush, strokeSize);
}

void Blox2DEngine::FillRect(float x, float y, float width, float height) const
{
	m_pRenderTarget->FillRectangle(RectF(x, y, x + width, y + height), m_pColorBrush);
}

void Blox2DEngine::FillRect(D2D1_RECT_F rect) const
{
	m_pRenderTarget->FillRectangle(rect, m_pColorBrush);
}

void Blox2DEngine::DrawPolygon(Point2D pArr[], unsigned int nrPoints, bool close, float strokeSize) const
{
	ID2D1PathGeometry* pPathGeometry = NULL;
	m_pD2DFactory->CreatePathGeometry(&pPathGeometry);

	ID2D1GeometrySink* pSink = NULL;
	pPathGeometry->Open(&pSink);

	pSink->BeginFigure(pArr[0], D2D1_FIGURE_BEGIN_HOLLOW);

	pSink->AddLines(pArr, nrPoints);
	
	if (close) pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	else pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	
	pSink->Close();
	SafeRelease(pSink);

	m_pRenderTarget->DrawGeometry(pPathGeometry, m_pColorBrush, strokeSize);

	SafeRelease(pPathGeometry);
}

void Blox2DEngine::FillPolygon(Point2D pArr[], unsigned int nrPoints) const
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

	m_pRenderTarget->FillGeometry(pPathGeometry, m_pColorBrush);

	SafeRelease(pPathGeometry);
}

void Blox2DEngine::DrawImage(Image* bitmap, float x, float y, bool pixelPerfect, float opacity, float width, float height)
{
	D2D1_RECT_F rect;

	rect.left = x;
	rect.top = y;

	if (width == 0 && height == 0)
	{
		rect.right = x + bitmap->GetDimensions().width;
		rect.bottom = y + bitmap->GetDimensions().height;
	}
	else if (width == 0 && height != 0)
	{
		rect.right = x + bitmap->GetDimensions().width;
		rect.bottom = y + height;
	}
	else if (width != 0 && height == 0)
	{
		rect.right = x + width;
		rect.bottom = y + bitmap->GetDimensions().height;
	}
	else
	{
		rect.right = x + width;
		rect.bottom = y + height;
	}

	float opc;

	if (opacity == 0)
		opc = bitmap->GetOpacity();
	else
		opc = opacity;

	D2D1_BITMAP_INTERPOLATION_MODE inpolMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

	if (pixelPerfect)
		inpolMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

	m_pRenderTarget->DrawBitmap(bitmap->GetD2D1BitmapResource(), rect, opc, inpolMode);
}

void Blox2DEngine::DrawImage(Image* bitmap, float x, float y, D2D1_RECT_F areaToDraw, bool pixelPerfect, float opacity)
{
	D2D1_BITMAP_INTERPOLATION_MODE inpolMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

	if (pixelPerfect)
		inpolMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

	D2D1_RECT_F rect;

	rect.left = x;
	rect.top = y;
	rect.right = x + (areaToDraw.right - areaToDraw.left);
	rect.bottom = y + (areaToDraw.bottom - areaToDraw.top);

	float opc;

	if (opacity == 0)
		opc = bitmap->GetOpacity();
	else
		opc = opacity;

	m_pRenderTarget->DrawBitmap(bitmap->GetD2D1BitmapResource(), rect, opc, inpolMode, areaToDraw);
}

void Blox2DEngine::DrawRoundRect(float x, float y, float width, float height, float radius, float strokeSize) const
{
	D2D1_ROUNDED_RECT roundRect;
	D2D1_RECT_F rect;

	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;

	roundRect.radiusX = radius;
	roundRect.radiusY = radius;
	roundRect.rect = rect;

	m_pRenderTarget->DrawRoundedRectangle(roundRect, m_pColorBrush, strokeSize);
}

void Blox2DEngine::DrawRoundRect(D2D1_ROUNDED_RECT roundRect, float strokeSize) const
{
	m_pRenderTarget->DrawRoundedRectangle(roundRect, m_pColorBrush, strokeSize);
}

void Blox2DEngine::FillRoundRect(float x, float y, float width, float height, float radius) const
{
	D2D1_ROUNDED_RECT roundRect;
	D2D1_RECT_F rect;

	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;

	roundRect.radiusX = radius;
	roundRect.radiusY = radius;
	roundRect.rect = rect;

	m_pRenderTarget->FillRoundedRectangle(roundRect, m_pColorBrush);
}

void Blox2DEngine::FillRoundRect(D2D1_ROUNDED_RECT roundRect) const
{
	m_pRenderTarget->FillRoundedRectangle(roundRect, m_pColorBrush);
}

void Blox2DEngine::DrawGeometry(ID2D1Geometry* pGeometry)
{
	m_pRenderTarget->DrawGeometry(pGeometry, m_pColorBrush);
}