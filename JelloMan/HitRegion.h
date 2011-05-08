#pragma once

#include "D3DUtil.h"
#include <vector>
#include "Controls.h"
#include "GameTimer.h"
#include "Blox2DEngine.h"

//-----------------------------------------------------------------
// HitRegion Class
//-----------------------------------------------------------------
class HitRegion
{
public:
	// constructors
	HitRegion(int type, float x, float y, float width, float height);
	HitRegion(int type, D2D1_POINT_2F* points, int nrPoints);
	HitRegion(Image* pImage, int r, int g, int b, IWICImagingFactory* pWICFactory);

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

	ID2D1TransformedGeometry* GetGeometry() const
	{ return m_pTransformedGeometry; }

	// setters
	void SetPosition(float x, float y);
	void SetPosition(D2D1_POINT_2F pos);
	void SetSize(int width, int height);

	void Move(int x, int y);
	
	// general
	void Draw(bool fill = false);

	static const int TYPE_RECTANGLE = 0;
	static const int TYPE_ELLIPSE = 1;
	static const int TYPE_POLYGON = 2;

private:

	/*Converts pixels into Geometry (internal use)*/
	HRESULT GeometryFromPixels(IWICBitmapLock *lockPtr, ID2D1Geometry**GeometryPtrPtr, int r, int g, int b);

	ID2D1Geometry* m_pGeometry;
	ID2D1TransformedGeometry* m_pTransformedGeometry;

	int m_Type;
	float m_Width;
	float m_Height;

	D2D1_POINT_2F m_CurrentPos;
	Matrix3x2F m_matWorld;
};