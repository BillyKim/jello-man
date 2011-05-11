#include "HitRegion.h"
#include "Image.h"

//-----------------------------------------------------------------
// HitRegion Class
//-----------------------------------------------------------------
HitRegion::HitRegion(int type, float x, float y, float width, float height)	:	m_pGeometry(0),
																			m_pTransformedGeometry(0)
{
	m_Type = type;
	m_Width = width;
	m_Height = height;

	m_CurrentPos.x = x;
	m_CurrentPos.y = y;

	if (type == TYPE_RECTANGLE)
	{
		ID2D1RectangleGeometry* RG = dynamic_cast<ID2D1RectangleGeometry*>(m_pGeometry);

		BX2D->GetFactory()->CreateRectangleGeometry(
		RectF(0,0,width,height),&RG);

		SafeRelease(m_pGeometry);
		m_pGeometry = RG;
	}
	else if (type == TYPE_ELLIPSE)
	{
		ID2D1EllipseGeometry* EG = dynamic_cast<ID2D1EllipseGeometry*>(m_pGeometry);

		BX2D->GetFactory()->CreateEllipseGeometry(
		Ellipse(Point2F(0,0),width,height),&EG);

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

		BX2D->GetFactory()->CreatePathGeometry(&PG);

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

HitRegion::HitRegion(Image* pImage, int r, int g, int b, IWICImagingFactory* pWICFactory) :	m_pGeometry(0),
																							m_pTransformedGeometry(0)
{
	SafeRelease(m_pGeometry);

	IWICBitmap *pIBitmap = 0;
	HRESULT hr = pWICFactory->CreateBitmapFromSource(
				pImage->GetFormatConverter(),
				WICBitmapNoCache,
				&pIBitmap);	

	if (SUCCEEDED(hr))
	{
		UINT originalWidth, originalHeight;
		hr = pImage->GetFormatConverter()->GetSize(&originalWidth, &originalHeight);
		WICRect rcLock = { 0, 0, originalWidth, originalHeight };
		IWICBitmapLock *pLock = NULL;
		hr = pIBitmap->Lock(&rcLock, WICBitmapLockWrite, &pLock);
		if (SUCCEEDED(hr))
		{
			SafeRelease(m_pGeometry);
			hr = GeometryFromPixels(pLock, &m_pGeometry, r,g,b);
		}

		// Release the bitmap lock.
		pLock->Release();
	}
	SafeRelease(pIBitmap);
	SetPosition(0,0);//creates TransformedGeometry
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
		BX2D->GetFactory()->CreateRectangleGeometry
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
		BX2D->GetFactory()->CreateEllipseGeometry
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

	BX2D->GetFactory()->CreatePathGeometry(&pPathGeometryUnion);

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
				SafeRelease(&pGeometrySink);

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
				SafeRelease(&pGeometrySink);

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
				SafeRelease(&pGeometrySink);

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
				SafeRelease(&pGeometrySink);

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
void HitRegion::SetPosition(float x, float y)
{
	Matrix3x2F temp = Matrix3x2F::Translation(x,y);

	SafeRelease(m_pTransformedGeometry);
	BX2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

	m_CurrentPos.x = x;
	m_CurrentPos.y = y;
}

void HitRegion::SetPosition(D2D1_POINT_2F pos)
{
	Matrix3x2F temp = Matrix3x2F::Translation(pos.x,pos.y);

	SafeRelease(m_pTransformedGeometry);
	BX2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

	m_CurrentPos.x = pos.x;
	m_CurrentPos.y = pos.y;
}

void HitRegion::Move(int x, int y)
{
	Matrix3x2F temp = Matrix3x2F::Translation(m_CurrentPos.x + x, m_CurrentPos.y + y);

	SafeRelease(m_pTransformedGeometry);
	BX2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

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
	BX2D->GetFactory()->CreateTransformedGeometry(m_pGeometry,temp,&m_pTransformedGeometry);

	/*if (m_Type == TYPE_RECTANGLE)
		BX2D->GetFactory()->CreateRectangleGeometry(
		RectF(0.0f,0.0f,(float)(width),(float)(height)),&m_pHitRegionRECT);
	else if (m_Type == TYPE_ELLIPSE)
		BX2D->GetFactory()->CreateEllipseGeometry(
		Ellipse(Point2F(0.0f,0.0f),(float)width,(float)height),&m_pHitRegionELLIPSE);

	m_matWorld.Identity();
	m_matWorld = m_matWorld.Translation(m_CurrentPos.x,m_CurrentPos.y);*/

	m_Width = (float)width;
	m_Height = (float)height;
}

// general
void HitRegion::Draw(bool fill)
{
	BX2D->SetColor(255,0,0,0.5f);

	D2D1_RECT_F rect;
	m_pTransformedGeometry->GetBounds(Matrix3x2F::Identity(),&rect);

	/*if (m_Type == TYPE_RECTANGLE)
	{*/
		if (fill) BX2D->FillRect(rect);
		else BX2D->DrawRect(rect);
	//}
	 if (m_Type == TYPE_ELLIPSE)
	{
		if (fill) BX2D->FillEllipse(
			rect.left + ((rect.right-rect.left)/2),
			rect.top + ((rect.bottom-rect.top)/2),
			(rect.right-rect.left)/2,
			(rect.bottom-rect.top)/2);
		else BX2D->DrawEllipse(
			rect.left+((rect.right-rect.left)/2),
			rect.top+((rect.bottom-rect.top)/2),
			rect.right-rect.left,
			rect.bottom-rect.top);
	}
	else if (m_Type == TYPE_POLYGON)
	{
		
		//m_pHitRegionPOLYGON->Stream(

		//BX2D->FillPolygon
	}
}

HRESULT HitRegion::GeometryFromPixels(IWICBitmapLock *lockPtr, ID2D1Geometry**GeometryPtrPtr, int r, int g, int b)
{
	HRESULT hr;

	//DWORD start = timeGetTime();
	// colordata is 32bppRGBA
	BYTE transp_R = static_cast<BYTE>(min(255,max(0,r)));
	BYTE transp_G = static_cast<BYTE>(min(255,max(0,g)));
	BYTE transp_B = static_cast<BYTE>(min(255,max(0,b)));
	UINT32 transparentPixel = (((BYTE)(transp_B)|((WORD)((BYTE)(transp_G))<<8))|(((DWORD)(BYTE)(transp_R))<<16));
	//prepare Geometry
	ID2D1GeometrySink *pSink = NULL;
	ID2D1PathGeometry *pathGeometryPtr = NULL;
	hr = BX2D->GetFactory()->CreatePathGeometry(&pathGeometryPtr);
	if (SUCCEEDED(hr))
	{
		// Write to the path geometry using the geometry sink.
		hr = pathGeometryPtr->Open(&pSink);
	}
	if (SUCCEEDED(hr))
	{

		//prepare pixel data
		UINT cbBufferSize = 0;

		WICInProcPointer pv = NULL;//is byte*
		hr = lockPtr->GetDataPointer(&cbBufferSize, &pv);

		WICPixelFormatGUID pixelFormat;
		hr = lockPtr->GetPixelFormat(&pixelFormat);
		UINT bytesPerPixel;
		if(pixelFormat == GUID_WICPixelFormat32bppPBGRA)bytesPerPixel=4;
		UINT originalWidth, originalHeight;
		hr = lockPtr->GetSize(&originalWidth, &originalHeight);
		// Scan each bitmap row from top to bottom
		BYTE *p32 = pv;
		for (UINT y = 0; y < originalHeight; y++)
		{
			// Scan each bitmap pixel from left to right
			for (UINT x = 0; x < originalWidth; x++)
			{
				// Search for a continuous range of "non transparent pixels"
				UINT x0 = x;
				UINT32 *pixel = (UINT32 *)p32 + x;
				while (x < originalWidth)
				{
					UINT32 maskedPixel = (*pixel) &  0x00ffffff;
					if(transparentPixel != maskedPixel) break;
					pixel++;
					x++;
				}

				if (x > x0)
				{
					//if(x==originalWidth)--x;
					// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the Geometry
					pSink->BeginFigure(D2D1::Point2F(static_cast<FLOAT>(x0), static_cast<FLOAT>(y)), D2D1_FIGURE_BEGIN_FILLED);
					pSink->AddLine(D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y)));
					pSink->AddLine(D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y+1)));
					pSink->AddLine(D2D1::Point2F(static_cast<FLOAT>(x0), static_cast<FLOAT>(y+1)));
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

				}
			}
			p32 += originalWidth * bytesPerPixel;
		}
	}
	
	UINT count;
	UINT count2;
	pathGeometryPtr->GetSegmentCount(&count);


	//pathGeometryPtr->Outline(0,50.0f,pSink);

	hr = pSink->Close();
	SafeRelease(pSink);

	pathGeometryPtr->GetSegmentCount(&count);
	pathGeometryPtr->GetFigureCount(&count2);
	
	m_pGeometry = dynamic_cast<ID2D1Geometry*>(pathGeometryPtr);


	////Simplify haalt niks uit
	//ID2D1PathGeometry *simplefiedPathGeometryPtr = NULL;
	//    hr = GameEngine::GetSingleton()->GetFactory()->CreatePathGeometry(&simplefiedPathGeometryPtr);

	//   if (SUCCEEDED(hr))
	//   {
	//       // Write to the path geometry using the geometry sink.
	//       hr = simplefiedPathGeometryPtr->Open(&pSink);
	//}
	//   if (SUCCEEDED(hr))
	//   {
	//	//hr = pathGeometryPtr->Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION_LINES, NULL,10 , pSink);
	//	hr = pathGeometryPtr->Outline( NULL, 44, pSink);
	//}
	//   if (SUCCEEDED(hr))
	//   {
	//	hr = pSink->Close();
	//	SafeRelease(&pSink);
	//	SafeRelease(&pathGeometryPtr);
	//	hr = simplefiedPathGeometryPtr->GetSegmentCount(&count);
	//}
	//   if (SUCCEEDED(hr))
	//   {
	//	m_GeometryPtr = dynamic_cast<ID2D1Geometry*>(simplefiedPathGeometryPtr);
	//}

	//Maak een Mesh van de Geometry, dit om sneller te kunnen tekenen: dit duurt een hele tijd: 390 msec met dae logo
	/*hr = GameEngine::GetSingleton()->GetRenderTarget()->CreateMesh(&m_MeshPtr);
	if (SUCCEEDED(hr))
	{
	ID2D1TessellationSink *pSink = NULL;
	hr = m_MeshPtr->Open(&pSink);
	if (SUCCEEDED(hr))
	{
	hr = pathGeometryPtr->Tessellate(NULL, pSink);
	if (SUCCEEDED(hr))
	{
	hr = pSink->Close();
	}
	pSink->Release();
	}
	}*/
	//DWORD end = timeGetTime();
	//DWORD dTime = end-start;
	//OutputDebugString(String("Image to region: ")+(int)dTime );

	return hr;
}