#pragma once

#include "D3DUtil.h"
#include <vector>
#include "GameTimer.h"

#define BX2D (Blox2DEngine::GetSingleton())

class HitRegion;
class Button;
class Image;
class TextFormat;

class Blox2DEngine
{
private:
	
	/* SINGLETON - CONSTRUCTOR*/

	Blox2DEngine();
	static Blox2DEngine* m_pSingleton;

public:
	
	/* SINGLETON - DESTRUCTOR*/

	virtual ~Blox2DEngine();
	static Blox2DEngine* GetSingleton();

	/* SETTERS */

	//* Sets the color of the brush. *
	void SetColor(int r, int g, int b, float a = 1.0f);
	//* Sets the color of the brush. *
	void SetColor(const D2D1_COLOR_F& color);
	//* Sets the textformat. *
	void SetFont(TextFormat* pTextFormat);
	//* Sets the transformation matrix of the world. *
	void SetTransform(D2D1_MATRIX_3X2_F transform);
	//* Resets the transformation matrix of the world. *
	void ResetTransform();
	//* Initializes Blox2DEngine - only used by engine. *
	void Initialize(ID2D1RenderTarget* pRenderTarget, ID2D1Factory* pD2DFactory	);
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
	//* Draws an ellipse with the given centerpoint (x,y), given width / height and given strokesize. *
	void DrawEllipse(float x, float y, float width, float height, float strokeSize = 1.0f) const;
	//* Draws an ellipse with the given centercoordinate coord, given width / height and given strokesize. *
	void DrawEllipse(Point2D coord, float width, float height, float strokeSize = 1.0f) const;
	//* Draws an ellipse with the given ellipse description and given strokesize. *
	void DrawEllipse(D2D1_ELLIPSE ellipse, float strokeSize = 1.0f) const;
	//* Draws a filled ellipse with the given centerpoint (x,y), given width / height. *
	void FillEllipse(float x, float y, float width, float height) const;
	//* Draws a filled ellipse with the given centercoordinate coord, given width / height. *
	void FillEllipse(Point2D coord, float width, float height)const;
	//* Draws a filled ellipse with the given ellipse description. *
	void FillEllipse(D2D1_ELLIPSE ellipse) const;
	//* Draws a string at the given coordinates - use left,top font alignment *
	void DrawString(tstring const& text, float x, float y) const;
	//* Draws a string centered in the window with the given x & y offset. *
	void DrawStringCentered(tstring const& text, float offSetX = 0, float offSetY = 0) const;
	//* Draws a string in the given rectangular area. *
	void DrawString(tstring const& text, D2D1_RECT_F rect);
	//* Draws a polygon with the given point array and given strokesize. *
	void DrawPolygon(Point2D pArr[], unsigned int nrPoints, bool close, float strokeSize = 1.0f) const;
	//* Draws a filled polygon with the given point array. *
	void FillPolygon(Point2D pArr[], unsigned int nrPoints) const;
	//* Draws an image at the given left top point x,y with or without anti-aliasing (pixelperfect) and given opacity - ability to resize image with new width / height. *
	void DrawImage(Image* bitmap, float x, float y, bool pixelPerfect = false, float opacity = 1.0f, float width = 0.0f, float height = 0.0f);
	//* Draws an image at the given left top point x,y with or without anti-aliasing (pixelperfect) and given opacity - ability to show only area of image. *
	void DrawImage(Image* bitmap, float x, float y, D2D1_RECT_F areaToDraw, bool pixelPerfect = false, float opacity = 1.0f);
	//* Draws a round rectangle at the given point x,y with the given width / height , radius and strokesize. *
	void DrawRoundRect(float x, float y, float width, float height, float radius, float strokeSize = 1.0f) const;
	//* Draws a round rectangle with the given description and strokesize. *
	void DrawRoundRect(D2D1_ROUNDED_RECT roundRect, float strokeSize = 1.0f) const;
	//* Draws a round rectangle at the given point x,y with the given width / height and radius. *
	void FillRoundRect(float x, float y, float width, float height, float radius) const;
	//* Draws a filled round rectangle with the given description. *
	void FillRoundRect(D2D1_ROUNDED_RECT roundRect) const;
	void DrawGeometry(ID2D1Geometry* pGeometry);

	//* Shows the current fps & delta time in top right corner with the given interval and with or without history graph. *
	void ShowFPS(float dTime, bool showGraph = false, float delayInterval = 1.0f);

private:

	/* DATAMEMBERS */

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

	/* DEFAULT COPY & ASSIGNMENT OPERATOR */

	Blox2DEngine(const Blox2DEngine& yRef);									
	Blox2DEngine& operator=(const Blox2DEngine& yRef);	
};