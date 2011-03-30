#include "RotateGizmo.h"


RotateGizmo::RotateGizmo()	:	 m_pRenderContext(0)
{
}


RotateGizmo::~RotateGizmo()
{
	m_pRenderContext = 0;
}

// GENERAL
void RotateGizmo::Show(Vector3& position, TYPE type, int id)
{
	// MATRIX
	D3DXMATRIX matProj = m_pRenderContext->GetCamera()->GetProjection();
	D3DXMATRIX matView = m_pRenderContext->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	// VIEWPORT
	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	// POSITION
	D3DXVECTOR3 pos = position.ToD3DVector3();

	Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - Vector3(pos);
	float l = length.Length();
	l *= 0.001f;

	// VIEWPORT PROJECTION
	D3DXVECTOR3 pos2D;
	D3DXVECTOR3 posLineX_2D;
	D3DXVECTOR3 posLineY_2D;
	D3DXVECTOR3 posLineZ_2D;

	D3DXVECTOR3 posLineX2_2D;
	D3DXVECTOR3 posLineY2_2D;
	D3DXVECTOR3 posLineZ2_2D;
	
	D3DXVECTOR3 posLineX = pos;
	posLineX.x += l*100;
	D3DXVECTOR3 posLineY = pos;
	posLineY.y += l*100;
	D3DXVECTOR3 posLineZ = pos;
	posLineZ.z += l*100;

	D3DXVECTOR3 posLineX2 = pos;
	posLineX2.x -= l*100;
	D3DXVECTOR3 posLineY2 = pos;
	posLineY2.y -= l*100;
	D3DXVECTOR3 posLineZ2 = pos;
	posLineZ2.z -= l*100;

	D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

	D3DXVec3Project(&posLineX_2D, &posLineX, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineY_2D, &posLineY, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineZ_2D, &posLineZ, &viewP, &matProj, &matView, &matIdent);

	D3DXVec3Project(&posLineX2_2D, &posLineX2, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineY2_2D, &posLineY2, &viewP, &matProj, &matView, &matIdent);
	D3DXVec3Project(&posLineZ2_2D, &posLineZ2, &viewP, &matProj, &matView, &matIdent);

	int size = 10;

	// X
	HitRegion hitRectX(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineX_2D.x),
		static_cast<int>(posLineX_2D.y),
		size,size);

	HitRegion hitRectX2(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineX2_2D.x),
		static_cast<int>(posLineX2_2D.y),
		size,size);

	BLOX_2D->SetColor(255, 0, 0);
	BLOX_2D->DrawLine(
		static_cast<int>(posLineX2_2D.x),
		static_cast<int>(posLineX2_2D.y),
		static_cast<int>(posLineX_2D.x),
		static_cast<int>(posLineX_2D.y),
		2.0f);

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineX_2D.x),
		static_cast<int>(posLineX_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineX2_2D.x),
		static_cast<int>(posLineX2_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	if (hitRectX.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineX_2D.x),
			static_cast<int>(posLineX_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);
	}

	else if (hitRectX2.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineX2_2D.x),
			static_cast<int>(posLineX2_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);
	}

	// Y
	HitRegion hitRectY(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineY_2D.x),
		static_cast<int>(posLineY_2D.y),
		size,size);

	HitRegion hitRectY2(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineY2_2D.x),
		static_cast<int>(posLineY2_2D.y),
		size,size);

	BLOX_2D->SetColor(0, 255, 0);
	BLOX_2D->DrawLine(
		static_cast<int>(posLineY2_2D.x),
		static_cast<int>(posLineY2_2D.y),
		static_cast<int>(posLineY_2D.x),
		static_cast<int>(posLineY_2D.y),
		2.0f);

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineY_2D.x),
		static_cast<int>(posLineY_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineY2_2D.x),
		static_cast<int>(posLineY2_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	if (hitRectY.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineY_2D.x),
			static_cast<int>(posLineY_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);
	}

	else if (hitRectY2.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineY2_2D.x),
			static_cast<int>(posLineY2_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);
	}

	// Z
	HitRegion hitRectZ(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineZ_2D.x),
		static_cast<int>(posLineZ_2D.y),
		size,size);

	HitRegion hitRectZ2(
		HitRegion::TYPE_ELLIPSE,
		static_cast<int>(posLineZ2_2D.x),
		static_cast<int>(posLineZ2_2D.y),
		size,size);

	BLOX_2D->SetColor(255, 255, 0);
	BLOX_2D->DrawLine(
		static_cast<int>(posLineZ2_2D.x),
		static_cast<int>(posLineZ2_2D.y),
		static_cast<int>(posLineZ_2D.x),
		static_cast<int>(posLineZ_2D.y),
		2.0f);

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineZ_2D.x),
		static_cast<int>(posLineZ_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	BLOX_2D->FillEllipse(
		static_cast<int>(posLineZ2_2D.x),
		static_cast<int>(posLineZ2_2D.y),
		static_cast<int>(size/2),
		static_cast<int>(size/2));

	if (hitRectZ.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineZ_2D.x),
			static_cast<int>(posLineZ_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);
	}

	else if (hitRectZ2.HitTest(CONTROLS->GetMousePos()))
	{
		BLOX_2D->SetColor(255,255,255);
		BLOX_2D->DrawEllipse(
			static_cast<int>(posLineZ2_2D.x),
			static_cast<int>(posLineZ2_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2),
			2.0f);
	}

	BLOX_2D->SetColor(50, 50, 255, 0.4f);
	BLOX_2D->FillEllipse(
		static_cast<int>(pos2D.x),
		static_cast<int>(pos2D.y),
		static_cast<int>(size*4),
		static_cast<int>(size*4));

	BLOX_2D->SetColor(100, 100, 255, 0.8f);
	BLOX_2D->DrawEllipse(
		static_cast<int>(pos2D.x),
		static_cast<int>(pos2D.y),
		static_cast<int>(size*4),
		static_cast<int>(size*4));
}

void RotateGizmo::Tick(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;
}

