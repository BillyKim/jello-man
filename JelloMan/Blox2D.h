#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include <vector>

//-----------------------------------------------------
// Blox2D Class									
//-----------------------------------------------------
class Blox2D
{
public:
	Blox2D(	ID2D1RenderTarget* pRenderTarget,
			ID2D1Factory* pD2DFactory,
			IDWriteFactory* pDWriteFactory,
			ID2D1SolidColorBrush* pColorBrush,
			IDWriteTextFormat* pTextFormat);

	virtual ~Blox2D();

	// SETTERS
	void SetColor(int r, int g, int b, float a = 1.0f);
	void SetColor(D2D1_COLOR_F color);
	void SetFont(tstring const& fontName, bool bold, bool italic, float size);
	
	// DRAW METHODS
	void DrawGrid(int stepsize);
	void FillBackGround();
	void DrawLine(int x, int y, int x2, int y2, float strokeSize = 1.0f);
	void DrawLine(D2D1_POINT_2F start, D2D1_POINT_2F end, float strokeSize = 1.0f);
	void DrawRect(int x, int y, int width, int height, float strokeSize = 1.0f);
	void DrawRect(D2D1_RECT_F rect, float strokeSize = 1.0f);
	void FillRect(int x, int y, int width, int height);
	void FillRect(D2D1_RECT_F rect);
	void DrawEllipse(int x, int y, int width, int height, float strokeSize= 1.0f);
	void DrawEllipse(D2D1_POINT_2F coord, int width, int height, float strokeSize= 1.0f);
	void DrawEllipse(D2D1_ELLIPSE ellipse, float strokeSize= 1.0f);
	void FillEllipse(int x, int y, int width, int height);
	void FillEllipse(D2D1_POINT_2F coord, int width, int height);
	void FillEllipse(D2D1_ELLIPSE ellipse);
	void DrawString(tstring const& text, int x, int y);
	void DrawStringCentered(tstring const& text, int offSetX = 0, int offSetY = 0);
	//void DrawString(tstring const& text, D2D1_RECT_F rect, TEXT_ALIGNMENT textAlignment, PARAGRAPH_ALIGNMENT paragraphAlignment);
	//void DrawString(int nr, D2D1_RECT_F rect, TEXT_ALIGNMENT textAlignment, PARAGRAPH_ALIGNMENT paragraphAlignment);
	void ShowFPS(float dTime, bool showGraph);
	void FillBlock(int x, int y, int size);
	void FillBlock(D2D1_POINT_2F coord, int size, D2D1_COLOR_F color1, D2D1_COLOR_F color2);
	void DrawPolygon(D2D1_POINT_2F pArr[], int nrPoints, bool close, float strokeSize);
	void FillPolygon(D2D1_POINT_2F pArr[], int nrPoints);
	//void DrawBitmap(Bitmap* bitmap, int x, int y, float opacity = 1.0f, int width = 0, int height = 0);

private: 

	ID2D1RenderTarget* m_pRenderTarget;
	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* m_pDWriteFactory;
	ID2D1SolidColorBrush* m_pColorBrush;
	IDWriteTextFormat* m_pTextFormat;

	vector<int> m_fpsHistory;
	vector<float> m_dtimeHistory;
	float m_GameTime;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Blox2D(const Blox2D& yRef);									
	Blox2D& operator=(const Blox2D& yRef);	
};

 
