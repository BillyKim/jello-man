#include "HitRegion.h"

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