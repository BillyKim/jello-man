#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include <vector>
#include "GameTimer.h"

#define BX2D (Blox2D::GetSingleton())

class HitRegion;
class Button;
class Image;
class TextFormat;

//-----------------------------------------------------
// Blox2D Class									
//-----------------------------------------------------
class Blox2D
{
private:
	// singleton
	Blox2D();
	static Blox2D* m_pSingleton;

public:
	
	virtual ~Blox2D();
	static Blox2D* GetSingleton();

	/* SETTERS */

	//* Sets the color of the brush. *
	void SetColor(int r, int g, int b, float a = 1.0f);
	//* Sets the color of the brush. *
	void SetColor(D2D1_COLOR_F color);
	//* Sets the textformat. *
	void SetFont(TextFormat* pTextFormat);
	//* Sets the transformation matrix of the world. *
	void SetTransform(D2D1_MATRIX_3X2_F transform);
	//* Resets the transformation matrix of the world. *
	void ResetTransform();
	//* Initializes Blox2D - only used by engine. *
	void Initialize(	ID2D1RenderTarget* pRenderTarget,
						ID2D1Factory* pD2DFactory	);
	//* Called when backbuffer is resized - only used by engine. *
	void OnResize(ID2D1RenderTarget* pRenderTarget) { m_pRenderTarget = pRenderTarget; }
	//* Turns on / off anti-aliasing. *
	void SetAntiAliasing(bool AA);
	//* Sets the windowhandel - only used by engine. *
	void SetWindowHandle(HWND hwnd) { m_Hwnd = hwnd; }
	//* Sets the window instance - only used by engine. *
	void SetWindowInstance(HINSTANCE instance) { m_hInstance = instance; }

	/* GETTERS */

	//* Returns the size of the window / backbuffer. *
	Size2D GetWindowSize() const;
	//* Returns the D2D factory - only used by engine. *
	ID2D1Factory* GetFactory() const { return m_pD2DFactory; }
	//* Returns the render target - only used by engine. *
	ID2D1RenderTarget* GetBackBuffer() const { return m_pRenderTarget; }
	//* Gets the window handle - only used by engine. *
	HWND GetWindowHandle() const { return m_Hwnd; }
	//* Gets the window instance - only used by engine. *
	HINSTANCE GetWindowInstance() const { return m_hInstance; }
	
	/* DRAW METHODS */

	//* Draws a square grid in the given area with the given stepsize. *
	void DrawGrid(float stepsize, D2D1_RECT_F area) const;
	//* Fills background with the current color. *
	void FillBackGround() const;
	//* Draws a line from x,y to x2,y2 with the given strokesize. *
	void DrawLine(float x, float y, float x2, float y2, float strokeSize = 1.0f) const;
	//* Draws a line from start point to end point with the given strokesize. *
	void DrawLine(Point2D start, Point2D end, float strokeSize = 1.0f) const;
	//* Draws the outline of a rect at position x,y with the given width / height and given strokesize. *
	void DrawRect(float x, float y, float width, float height, float strokeSize = 1.0f) const;
	//* Draws the outline of a rect description with the given strokesize. *
	void DrawRect(D2D1_RECT_F rect, float strokeSize = 1.0f) const;
	//* Draws a filled rect at position x,y with the given width / height. *
	void FillRect(float x, float y, float width, float height) const;
	//* Draws a filled rect description. *
	void FillRect(D2D1_RECT_F rect) const;
	void DrawEllipse(float x, float y, float width, float height, float strokeSize = 1.0f) const;
	void DrawEllipse(Point2D coord, float width, float height, float strokeSize = 1.0f) const;
	void DrawEllipse(D2D1_ELLIPSE ellipse, float strokeSize = 1.0f) const;
	void FillEllipse(float x, float y, float width, float height) const;
	void FillEllipse(Point2D coord, float width, float height)const;
	void FillEllipse(D2D1_ELLIPSE ellipse) const;
	void DrawString(tstring const& text, float x, float y) const;
	void DrawStringCentered(tstring const& text, float offSetX = 0, float offSetY = 0) const;
	void DrawString(tstring const& text, D2D1_RECT_F rect);
	void ShowFPS(float dTime, bool showGraph = false, float delayInterval = 1.0f);
	void FillBlock(float x, float y, float size);
	void FillBlock(D2D1_POINT_2F coord, float size, D2D1_COLOR_F color1, D2D1_COLOR_F color2);
	void DrawPolygon(D2D1_POINT_2F pArr[], int nrPoints, bool close, float strokeSize = 1.0f) const;
	void FillPolygon(D2D1_POINT_2F pArr[], int nrPoints) const;
	void DrawBitmap(Image* bitmap, float x, float y, bool pixelPerfect = false, float opacity = 1.0f, float width = 0.0f, float height = 0.0f);
	void DrawBitmap(Image* bitmap, float x, float y, D2D1_RECT_F areaToDraw, bool pixelPerfect = false);
	void DrawRoundRect(float x, float y, float width, float height, float radius, float strokeSize = 1.0f) const;
	void DrawRoundRect(D2D1_ROUNDED_RECT roundRect, float strokeSize = 1.0f) const;
	void FillRoundRect(float x, float y, float width, float height, float radius) const;
	void FillRoundRect(D2D1_ROUNDED_RECT roundRect) const;
	void DrawGeometry(ID2D1Geometry* pGeometry);

private:

	ID2D1RenderTarget* m_pRenderTarget;
	ID2D1Factory* m_pD2DFactory;
	ID2D1SolidColorBrush* m_pColorBrush;
	IDWriteTextFormat* m_pTextFormat;

	TextFormat* m_pFPSFont;

	vector<int> m_fpsHistory;
	vector<float> m_dtimeHistory;

	float m_TBase;
	float m_GameTime;
	int m_FPS;
	float m_DTimeS;

	HWND m_Hwnd;
	HINSTANCE m_hInstance;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Blox2D(const Blox2D& yRef);									
	Blox2D& operator=(const Blox2D& yRef);	
};