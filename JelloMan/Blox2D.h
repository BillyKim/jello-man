#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include <vector>
#include "Controls.h"
#include "GameTimer.h"

#define BLOX_2D (Blox2D::GetSingleton())

class HitRegion;
class Bitmap;
class Button;

//-----------------------------------------------------
// Blox2D Class									
//-----------------------------------------------------
class Blox2D
{
public:
	enum HORIZONTAL_ALIGN
	{
		HORIZONTAL_ALIGN_LEFT = 0,
		HORIZONTAL_ALIGN_CENTER = 1,
		HORIZONTAL_ALIGN_RIGHT = 2
	};

	enum VERTICAL_ALIGN
	{
		VERTICAL_ALIGN_TOP = 0,
		VERTICAL_ALIGN_MIDDLE = 1,
		VERTICAL_ALIGN_BOTTOM = 2
	};

private:
	// singleton
	Blox2D();
	static Blox2D* m_pSingleton;

public:
	
	virtual ~Blox2D();
	static Blox2D* GetSingleton();

	HRESULT Blox2D::LoadBitmapFromFile(
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap **ppBitmap
    );

	// SETTERS
	void SetColor(int r, int g, int b, float a = 1.0f);
	void SetColor(D2D1_COLOR_F color);
	void SetFont(tstring const& fontName, bool bold, bool italic, float size);
	void SetFont(IDWriteTextFormat* textFormat);
	void SetTransform(D2D1_MATRIX_3X2_F transform);
	void ResetTransform();

	void SetParams(	ID2D1RenderTarget* pRenderTarget,
					ID2D1Factory* pD2DFactory,
					IDWriteFactory* pDWriteFactory,
					ID2D1SolidColorBrush* pColorBrush,
					IDWriteTextFormat* pTextFormat);

	void OnResize(ID2D1RenderTarget* pRenderTarget) { m_pRenderTarget = pRenderTarget; }
	void SetAntiAliasing(bool AA);

	// GETTERS
	Size2D GetWindowSize() const;
	ID2D1Factory* GetFactory() const { return m_pD2DFactory; }
	IDWriteFactory* GetWriteFactory() const { return m_pDWriteFactory; }
	
	// DRAW METHODS
	void DrawGrid(int stepsize, D2D1_RECT_F area) const;
	void FillBackGround() const;
	void DrawLine(int x, int y, int x2, int y2, float strokeSize = 1.0f) const;
	void DrawLine(D2D1_POINT_2F start, D2D1_POINT_2F end, float strokeSize = 1.0f) const;
	void DrawRect(int x, int y, int width, int height, float strokeSize = 1.0f) const;
	void DrawRect(D2D1_RECT_F rect, float strokeSize = 1.0f) const;
	void FillRect(int x, int y, int width, int height) const;
	void FillRect(D2D1_RECT_F rect) const;
	void DrawEllipse(int x, int y, int width, int height, float strokeSize= 1.0f) const;
	void DrawEllipse(D2D1_POINT_2F coord, int width, int height, float strokeSize= 1.0f) const;
	void DrawEllipse(D2D1_ELLIPSE ellipse, float strokeSize= 1.0f) const;
	void FillEllipse(int x, int y, int width, int height) const;
	void FillEllipse(D2D1_POINT_2F coord, int width, int height)const;
	void FillEllipse(D2D1_ELLIPSE ellipse) const;
	void DrawString(tstring const& text, int x, int y) const;
	void DrawStringCentered(tstring const& text, int offSetX = 0, int offSetY = 0) const;
	void DrawString(tstring const& text, D2D1_RECT_F rect, HORIZONTAL_ALIGN textAlignment, VERTICAL_ALIGN paragraphAlignment);
	void ShowFPS(float dTime, bool showGraph = false, float delayInterval = 1.0f);
	void FillBlock(int x, int y, int size);
	void FillBlock(D2D1_POINT_2F coord, int size, D2D1_COLOR_F color1, D2D1_COLOR_F color2);
	void DrawPolygon(D2D1_POINT_2F pArr[], int nrPoints, bool close, float strokeSize) const;
	void FillPolygon(D2D1_POINT_2F pArr[], int nrPoints) const;
	void DrawBitmap(Bitmap* bitmap, int x, int y, float opacity = 1.0f, int width = 0, int height = 0);
	void DrawRoundRect(int x, int y, int width, int height, int radius, float strokeSize = 1.0f) const;
	void DrawRoundRect(D2D1_ROUNDED_RECT roundRect, float strokeSize = 1.0f) const;
	void FillRoundRect(int x, int y, int width, int height, int radius) const;
	void FillRoundRect(D2D1_ROUNDED_RECT roundRect) const;

private:

	ID2D1RenderTarget* m_pRenderTarget;
	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* m_pDWriteFactory;
	ID2D1SolidColorBrush* m_pColorBrush;
	IDWriteTextFormat* m_pTextFormat;
	IWICImagingFactory *m_pWICFactory;

	HORIZONTAL_ALIGN m_HorizontalAlignment;
	VERTICAL_ALIGN m_VerticalAlignment;

	vector<int> m_fpsHistory;
	vector<float> m_dtimeHistory;

	int m_FrameCount;
	float m_TBase;
	float m_GameTime;
	int m_FPS;
	float m_DTimeS;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Blox2D(const Blox2D& yRef);									
	Blox2D& operator=(const Blox2D& yRef);	
};

//-----------------------------------------------------------------
// TextBox Class
//-----------------------------------------------------------------

class TextBox
{
public:

	// constructors
	TextBox(int posX, int posY, int width, int height);

	// destructor
	virtual ~TextBox();

	// general
	void Show();

	// getters
	tstring GetText() const
	{ return m_Text; }
	bool HasFocus() const
	{ return m_bHasFocus; }

	// setters
	void SetText(tstring text)
	{ m_Text = text; }

	void SetBackColor(unsigned int R, unsigned int G, unsigned int B);
	void SetTextColor(unsigned int R, unsigned int G, unsigned int B);
	void SetFont(tstring fontName, bool bold, bool italic, float size);

	void SetFocus(bool hasFocus)
	{ m_bHasFocus = hasFocus; }

private:

	// general
	void Tick();
	
	// datamembers
	bool m_bHasFocus;
	bool m_bClick;

	GameTimer m_Timer;
	float m_Time;

	tstring m_Text;
	tstring m_NewText;

	HitRegion* m_pHitRegion;

	Point2D m_Pos;

	int m_Width;
	int m_Height;

	IDWriteTextFormat* m_pTextFormat;

	ColorF m_BackColor;
	ColorF m_TextColor;
};