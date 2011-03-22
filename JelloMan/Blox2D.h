#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include <vector>
#include "Controls.h"

#define BLOX_2D (Blox2D::GetSingleton())

class Bitmap;

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
	//void DrawString(int nr, D2D1_RECT_F rect, TEXT_ALIGNMENT textAlignment, PARAGRAPH_ALIGNMENT paragraphAlignment);
	void ShowFPS(float dTime, bool showGraph = false, float delayInterval = 1.0f);
	void FillBlock(int x, int y, int size);
	void FillBlock(D2D1_POINT_2F coord, int size, D2D1_COLOR_F color1, D2D1_COLOR_F color2);
	void DrawPolygon(D2D1_POINT_2F pArr[], int nrPoints, bool close, float strokeSize) const;
	void FillPolygon(D2D1_POINT_2F pArr[], int nrPoints) const;
	void DrawBitmap(Bitmap* bitmap, int x, int y, float opacity = 1.0f, int width = 0, int height = 0);

private:

	ID2D1RenderTarget* m_pRenderTarget;
	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* m_pDWriteFactory;
	ID2D1SolidColorBrush* m_pColorBrush;
	IDWriteTextFormat* m_pTextFormat;
	IWICImagingFactory *m_pWICFactory;

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
// HitRegion Class
//-----------------------------------------------------------------
class HitRegion
{
public:
	// constructors
	HitRegion(); // default
	HitRegion(int type, int x, int y, int width, int height);
	HitRegion(int type, D2D1_POINT_2F* points, int nrPoints);

	// destructor
	virtual  ~HitRegion();

	// default copy constructor
	HitRegion(const HitRegion& second);

	// default assignment operator
	HitRegion& operator=(const HitRegion& second);

	// getters
	bool HitTest(HitRegion* hitRect, bool draw = false); // returns true if 2 hitregions hit
	bool HitTest(D2D1_POINT_2F pos); // returns centerpoint of 2 intersecting hitregions - else returns (-999999,-999999)

	D2D1_POINT_2F CollisionTest(HitRegion* hitRect);
	D2D1_RECT_F GetDimension();

	// setters
	void SetPosition(int x, int y);
	void SetPosition(D2D1_POINT_2F pos);
	void SetSize(int width, int height);

	void Move(int x, int y);
	
	// general
	void Draw(bool fill = false);

	static const int TYPE_RECTANGLE = 0;
	static const int TYPE_ELLIPSE = 1;
	static const int TYPE_POLYGON = 2;

private:
	ID2D1RectangleGeometry* m_pHitRegionRECT;
	ID2D1EllipseGeometry* m_pHitRegionELLIPSE;
	ID2D1PathGeometry* m_pHitRegionPOLYGON;

	int m_Type;
	int m_Width;
	int m_Height;

	D2D1_POINT_2F m_CurrentPos;
	D2D1_MATRIX_3X2_F m_matWorld;
};

//-----------------------------------------------------------------
// Bitmap Class
//-----------------------------------------------------------------

struct BITMAP_INFO
{
	tstring filepath;
	ID2D1Bitmap* bitmap;
};

class Bitmap
{
public:
	// constructors
	Bitmap(tstring filepath);
	Bitmap(int IDBitmap);

	// destructor
	virtual  ~Bitmap();

	// getters
	D2D1_SIZE_F GetSize();
	BITMAP_INFO GetInfo();
	bool Exists();

private:
	tstring m_FilePath;
	bool m_bExists;

	ID2D1Bitmap* m_pBitmap;
};
 
//-----------------------------------------------------------------
// Button Class
//-----------------------------------------------------------------

class Button
{
public:

	enum BUTTON_STATE
	{
		STATE_NORMAL = 0,
		STATE_HOVER = 1,
		STATE_DOWN = 2,
		STATE_DEACTIVATED = 3,
		STATE_DEACTIVATED_HOVER = 4,
		STATE_DEACTIVATED_DOWN = 5
	};

	enum BUTTON_MODE
	{
		MODE_MOUSE = 0,
		MODE_MANUAL = 1
	};

	// constructor
	Button();
	Button(int posX, int posY, int width, int height, bool bToggleable = false);

	// destructor
	virtual  ~Button();

	// general
	void Tick();
	void ShowHitRegion(bool fill = false);
	void Show();

	// setters
	void SetNormalState(Bitmap* normalState);
	void SetHoverState(Bitmap* hoverState);
	void SetDownState(Bitmap* downState);
	void SetDeactivatedState(Bitmap* deacivatedState);
	void SetDeactivatedStateHover(Bitmap* deactivatedHover);
	void SetDeactivatedStateDown(Bitmap* deactivatedDown);
	void SetState(BUTTON_STATE state);
	void SetMode(BUTTON_MODE mode);
	void SetPosition(int x, int y);
	void SetTransparency(float alpha)
	{m_Opacity = alpha;}

	// getters
	bool Clicked();
	bool Hover() const
	{ return m_bHover; }
	bool Down() const
	{ return m_bDown; }
	int GetWith()
	{return (int)m_Size.width;}
	int GetHeight()
	{return (int)m_Size.height;}
	bool IsActive() const
	{ return m_bActivated; }

private:
	Bitmap* m_pNormalBitmap;
	Bitmap* m_pHoverBitmap;
	Bitmap* m_pDownBitmap;
	Bitmap* m_pDeactivatedBitmap;
	Bitmap* m_pDeactivatedHoverBitmap;
	Bitmap* m_pDeactivatedDownBitmap;

	D2D1_SIZE_F m_Size;
	D2D1_POINT_2F m_Pos;

	HitRegion* m_pHitRect;

	BUTTON_STATE m_State;
	BUTTON_MODE m_Mode;

	bool m_bClicked;
	bool m_bToggleable;
	bool m_bActivated;
	bool m_bHover;
	bool m_bClick;
	bool m_bDown;

	float m_Opacity;
};