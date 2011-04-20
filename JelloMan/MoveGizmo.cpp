#include "MoveGizmo.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
MoveGizmo::MoveGizmo()	:	m_bLockX(false),
							m_bLockY(false),
							m_bLockZ(false),
							m_pRenderContext(0),
							m_pAxisFont(0)
{
	m_pAxisFont = Content->LoadTextFormat(_T("Verdana"),14, false, false);
}


MoveGizmo::~MoveGizmo()
{
	m_pRenderContext = 0;
}

// GENERAL
void MoveGizmo::Show(Light* pLight, int id)
{
	/*if (pLight->GetType() == LightType_Point)
	{

	}*/

	// MATRIX
	D3DXMATRIX matProj = m_pRenderContext->GetCamera()->GetProjection();
	D3DXMATRIX matView = m_pRenderContext->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	// VIEWPORT
	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BX2D->GetWindowSize().width;
	viewP.Height = (UINT)BX2D->GetWindowSize().height;
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	// POSITION
	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(	pLight->GetPosition().X,
						pLight->GetPosition().Y,
						pLight->GetPosition().Z	);

	Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();

	Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - Vector3(pos);
	float l = length.Length();
	l *= 0.001f;

	D3DXVECTOR3 pos2D;
	D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

	if (vLook.Dot(length) < 0 &&
		pos2D.x < BX2D->GetWindowSize().width &&
		pos2D.x > 0 &&
		pos2D.y < BX2D->GetWindowSize().height &&
		pos2D.y > 0)
	{
		// VIEWPORT PROJECTION
		D3DXVECTOR3 posLineX_2D;
		D3DXVECTOR3 posLineY_2D;
		D3DXVECTOR3 posLineZ_2D;

		D3DXVECTOR3 posLineX2_2D;
		D3DXVECTOR3 posLineY2_2D;
		D3DXVECTOR3 posLineZ2_2D;

		D3DXVECTOR3 posLineXY_2D;
		D3DXVECTOR3 posLineYZ_2D;
		D3DXVECTOR3 posLineZX_2D;
	
		D3DXVECTOR3 posLineX = pos;
		posLineX.x += l*100;
		D3DXVECTOR3 posLineY = pos;
		posLineY.y += l*100;
		D3DXVECTOR3 posLineZ = pos;
		posLineZ.z += l*100;

		D3DXVECTOR3 posLineX2 = pos;
		posLineX2.x += l*50;
		D3DXVECTOR3 posLineY2 = pos;
		posLineY2.y += l*50;
		D3DXVECTOR3 posLineZ2 = pos;
		posLineZ2.z += l*50;

		D3DXVECTOR3 posLineXY = pos;
		posLineXY.x += l*50;
		posLineXY.y += l*50;
		D3DXVECTOR3 posLineYZ = pos;
		posLineYZ.y += l*50;
		posLineYZ.z += l*50;
		D3DXVECTOR3 posLineZX = pos;
		posLineZX.z += l*50;
		posLineZX.x += l*50;

		D3DXVec3Project(&posLineX_2D, &posLineX, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineY_2D, &posLineY, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineZ_2D, &posLineZ, &viewP, &matProj, &matView, &matIdent);

		D3DXVec3Project(&posLineX2_2D, &posLineX2, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineY2_2D, &posLineY2, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineZ2_2D, &posLineZ2, &viewP, &matProj, &matView, &matIdent);

		D3DXVec3Project(&posLineXY_2D, &posLineXY, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineYZ_2D, &posLineYZ, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineZX_2D, &posLineZX, &viewP, &matProj, &matView, &matIdent);

		int size = 10;

		// X
		HitRegion hitRectX(
			HitRegion::TYPE_ELLIPSE,
			posLineX_2D.x,
			posLineX_2D.y,
			(float)size,
			(float)size);

		BX2D->SetColor(255, 0, 0);
		BX2D->DrawLine(
			posLineX2_2D.x,
			posLineX2_2D.y,
			posLineX_2D.x,
			posLineX_2D.y,
			2.0f);

		BX2D->FillEllipse(
			posLineX_2D.x,
			posLineX_2D.y,
			size/2.0f,
			size/2.0f);
	
		BX2D->SetColor(255, 0, 0);
		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(_T("X"),
			posLineX_2D.x - 5,
			posLineX_2D.y - 25);

		// XY
		D2D1_POINT_2F r[5];
		r[0].x = posLineXY_2D.x;
		r[0].y = posLineXY_2D.y;
		r[1].x = posLineY2_2D.x;
		r[1].y = posLineY2_2D.y;
		r[2].x = pos2D.x;
		r[2].y = pos2D.y;
		r[3].x = posLineX2_2D.x;
		r[3].y = posLineX2_2D.y;
		r[4].x = posLineXY_2D.x;
		r[4].y = posLineXY_2D.y;

		HitRegion hitRectXY(HitRegion::TYPE_POLYGON, &r[0], 5);
						
		if (hitRectXY.HitTest(CONTROLS->GetMousePos()))
		{		
			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockX == false && m_bLockZ == false)
				m_bLockX = m_bLockY = true;
		}

		if (hitRectXY.HitTest(CONTROLS->GetMousePos()) || (m_bLockX == true && m_bLockY == true))
			BX2D->SetColor(150, 150, 255, 0.8f);
		else
			BX2D->SetColor(50, 50, 255, 0.4f);

		BX2D->FillPolygon(r, 4);

		BX2D->SetColor(100, 100, 255, 0.8f);
		BX2D->DrawLine(r[0], r[1]);
		BX2D->DrawLine(r[1], r[2]);
		BX2D->DrawLine(r[2], r[3]);
		BX2D->DrawLine(r[3], r[0]);

		if (hitRectX.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255, 255, 255);
			BX2D->DrawEllipse(
				posLineX_2D.x,
				posLineX_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockZ == false)
				m_bLockX = true;
		}

		if (!CONTROLS->LeftMBDown())
			m_bLockX = m_bLockY = m_bLockZ = false;

		D3DXVECTOR3 mousePosPlusZX(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineX_2D.z);
		D3DXVECTOR3 mousePosPlusZX_3D;
		D3DXVec3Unproject(&mousePosPlusZX_3D, &mousePosPlusZX, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZX_3D.x -= l*100;

		if (m_bLockX)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineX_2D.x,
				posLineX_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			float diff = m_OldLightPos[id].x - mousePosPlusZX_3D.x;

			pLight->Translate(Vector3(-diff, 0, 0));

			m_OldLightPos[id].x = mousePosPlusZX_3D.x;
		}
		else
		{
			m_OldLightPos[id].x = mousePosPlusZX_3D.x;
		}

		// Y
		HitRegion hitRectY(
			HitRegion::TYPE_ELLIPSE,
			posLineY_2D.x,
			posLineY_2D.y,
			(float)size,
			(float)size);

		BX2D->SetColor(0, 255, 0);
		BX2D->DrawLine(
			posLineY2_2D.x,
			posLineY2_2D.y,
			posLineY_2D.x,
			posLineY_2D.y,
			2.0f);

		BX2D->FillEllipse(
			posLineY_2D.x,
			posLineY_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->SetColor(0, 255, 0);
		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(
			_T("Y"),
			posLineY_2D.x - 5,
			posLineY_2D.y - 25);
	
		//D2D1_POINT_2F r[4];
		r[0].x = posLineY2_2D.x;
		r[0].y = posLineY2_2D.y;
		r[1].x = posLineYZ_2D.x;
		r[1].y = posLineYZ_2D.y;
		r[2].x = posLineZ2_2D.x;
		r[2].y = posLineZ2_2D.y;
		r[3].x = pos2D.x;
		r[3].y = pos2D.y;
		r[4].x = posLineY2_2D.x;
		r[4].y = posLineY2_2D.y;

		HitRegion hitRectYZ(HitRegion::TYPE_POLYGON, &r[0], 5);
						
		if (hitRectYZ.HitTest(CONTROLS->GetMousePos()))
		{
			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockX == false && m_bLockZ == false)
				m_bLockY = m_bLockZ = true;
		}

		if (hitRectYZ.HitTest(CONTROLS->GetMousePos()) || (m_bLockY == true && m_bLockZ == true))
			BX2D->SetColor(150, 150, 255, 0.8f);
		else
			BX2D->SetColor(50, 50, 255, 0.4f);

		BX2D->FillPolygon(r,4);

		BX2D->SetColor(100, 100, 255, 0.8f);
		BX2D->DrawLine(r[0],r[1]);
		BX2D->DrawLine(r[1],r[2]);
		BX2D->DrawLine(r[2],r[3]);
		BX2D->DrawLine(r[3],r[0]);

		if (hitRectY.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineY_2D.x,
				posLineY_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockX == false && m_bLockZ == false)
			{
				m_bLockY = true;
			}
		}

		D3DXVECTOR3 mousePosPlusZY(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineY_2D.z);
		D3DXVECTOR3 mousePosPlusZY_3D;
		D3DXVec3Unproject(&mousePosPlusZY_3D, &mousePosPlusZX, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZY_3D.y -= l*100;

		if (m_bLockY)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineY_2D.x,
				posLineY_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);
			
			float diff = m_OldLightPos[id].y - mousePosPlusZY_3D.y;

			pLight->Translate(Vector3(0, -diff, 0));

			m_OldLightPos[id].y = mousePosPlusZY_3D.y;
		}
		else
		{
			m_OldLightPos[id].y = mousePosPlusZY_3D.y;
		}
	
		// Z
		HitRegion hitRectZ(
			HitRegion::TYPE_ELLIPSE,
			posLineZ_2D.x,
			posLineZ_2D.y,
			(float)size,
			(float)size);

		BX2D->SetColor(255, 255, 0);
		BX2D->DrawLine(
			posLineZ2_2D.x,
			posLineZ2_2D.y,
			posLineZ_2D.x,
			posLineZ_2D.y,
			2.0f);

		BX2D->FillEllipse(
			posLineZ_2D.x,
			posLineZ_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->SetColor(255, 255, 0);
		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(
			_T("Z"),
			posLineZ_2D.x - 5,
			posLineZ_2D.y - 25);

		//D2D1_POINT_2F r[4];
		r[0].x = posLineZX_2D.x;
		r[0].y = posLineZX_2D.y;
		r[1].x = posLineX2_2D.x;
		r[1].y = posLineX2_2D.y;
		r[2].x = pos2D.x;
		r[2].y = pos2D.y;
		r[3].x = posLineZ2_2D.x;
		r[3].y = posLineZ2_2D.y;
		r[4].x = posLineZX_2D.x;
		r[4].y = posLineZX_2D.y;

		HitRegion hitRectZX(HitRegion::TYPE_POLYGON, &r[0], 5);
						
		if (hitRectZX.HitTest(CONTROLS->GetMousePos()))
		{
			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockX == false && m_bLockZ == false)
				m_bLockZ = m_bLockX = true;
		}

		if (hitRectZX.HitTest(CONTROLS->GetMousePos()) || (m_bLockZ == true && m_bLockX == true))
			BX2D->SetColor(150, 150, 255, 0.8f);
		else
			BX2D->SetColor(50, 50, 255, 0.4f);

		BX2D->FillPolygon(r,4);

		BX2D->SetColor(100, 100, 255, 0.8f);
		BX2D->DrawLine(r[0],r[1]);
		BX2D->DrawLine(r[1],r[2]);
		BX2D->DrawLine(r[2],r[3]);
		BX2D->DrawLine(r[3],r[0]);

		if (hitRectZ.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineZ_2D.x,
				posLineZ_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockX == false && m_bLockY == false)
			{
				m_bLockZ = true;
			}
		}

		D3DXVECTOR3 mousePosPlusZZ(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineZ_2D.z);
		D3DXVECTOR3 mousePosPlusZZ_3D;
		D3DXVec3Unproject(&mousePosPlusZZ_3D, &mousePosPlusZZ, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZZ_3D.z -= l*100;

		if (m_bLockZ)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineZ_2D.x,
				posLineZ_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			float diff = m_OldLightPos[id].z - mousePosPlusZZ_3D.z;

			pLight->Translate(Vector3(0, 0, -diff));

			m_OldLightPos[id].z = mousePosPlusZZ_3D.z;
		}
		else
		{
			m_OldLightPos[id].z = mousePosPlusZZ_3D.z;
		}
	}
}

void MoveGizmo::Show(ILevelObject* pLevelObject, int id)
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
	viewP.Width = (UINT)BX2D->GetWindowSize().width;
	viewP.Height = (UINT)BX2D->GetWindowSize().height;
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	
	// POSITION
	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(
		pLevelObject->GetPosition().X,
		pLevelObject->GetPosition().Y,
		pLevelObject->GetPosition().Z	);

	Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();

	Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - Vector3(pos);
	float l = length.Length();
	l *= 0.001f;

	D3DXVECTOR3 pos2D;
	D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

	if (vLook.Dot(length) < 0 &&
		pos2D.x < BX2D->GetWindowSize().width &&
		pos2D.x > 0 &&
		pos2D.y < BX2D->GetWindowSize().height &&
		pos2D.y > 0)
	{
		// VIEWPORT PROJECTION
		D3DXVECTOR3 posLineX_2D;
		D3DXVECTOR3 posLineY_2D;
		D3DXVECTOR3 posLineZ_2D;

		D3DXVECTOR3 posLineX2_2D;
		D3DXVECTOR3 posLineY2_2D;
		D3DXVECTOR3 posLineZ2_2D;

		D3DXVECTOR3 posLineXY_2D;
		D3DXVECTOR3 posLineYZ_2D;
		D3DXVECTOR3 posLineZX_2D;
	
		D3DXVECTOR3 posLineX = pos;
		posLineX.x += l*100;
		D3DXVECTOR3 posLineY = pos;
		posLineY.y += l*100;
		D3DXVECTOR3 posLineZ = pos;
		posLineZ.z += l*100;

		D3DXVECTOR3 posLineX2 = pos;
		posLineX2.x += l*50;
		D3DXVECTOR3 posLineY2 = pos;
		posLineY2.y += l*50;
		D3DXVECTOR3 posLineZ2 = pos;
		posLineZ2.z += l*50;

		D3DXVECTOR3 posLineXY = pos;
		posLineXY.x += l*50;
		posLineXY.y += l*50;
		D3DXVECTOR3 posLineYZ = pos;
		posLineYZ.y += l*50;
		posLineYZ.z += l*50;
		D3DXVECTOR3 posLineZX = pos;
		posLineZX.z += l*50;
		posLineZX.x += l*50;

		D3DXVec3Project(&posLineX_2D, &posLineX, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineY_2D, &posLineY, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineZ_2D, &posLineZ, &viewP, &matProj, &matView, &matIdent);

		D3DXVec3Project(&posLineX2_2D, &posLineX2, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineY2_2D, &posLineY2, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineZ2_2D, &posLineZ2, &viewP, &matProj, &matView, &matIdent);

		D3DXVec3Project(&posLineXY_2D, &posLineXY, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineYZ_2D, &posLineYZ, &viewP, &matProj, &matView, &matIdent);
		D3DXVec3Project(&posLineZX_2D, &posLineZX, &viewP, &matProj, &matView, &matIdent);

		int size = 10;

		// X
		HitRegion hitRectX(
			HitRegion::TYPE_ELLIPSE,
			posLineX_2D.x,
			posLineX_2D.y,
			(float)size,
			(float)size);

		BX2D->SetColor(255, 0, 0);
		BX2D->DrawLine(
			posLineX2_2D.x,
			posLineX2_2D.y,
			posLineX_2D.x,
			posLineX_2D.y,
			2.0f);

		BX2D->FillEllipse(
			posLineX_2D.x,
			posLineX_2D.y,
			size/2.0f,
			size/2.0f);
	
		BX2D->SetColor(255, 0, 0);
		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(_T("X"),
			posLineX_2D.x - 5,
			posLineX_2D.y - 25);

		// XY
		D2D1_POINT_2F r[5];
		r[0].x = posLineXY_2D.x;
		r[0].y = posLineXY_2D.y;
		r[1].x = posLineY2_2D.x;
		r[1].y = posLineY2_2D.y;
		r[2].x = pos2D.x;
		r[2].y = pos2D.y;
		r[3].x = posLineX2_2D.x;
		r[3].y = posLineX2_2D.y;
		r[4].x = posLineXY_2D.x;
		r[4].y = posLineXY_2D.y;

		HitRegion hitRectXY(HitRegion::TYPE_POLYGON, &r[0], 5);
						
		if (hitRectXY.HitTest(CONTROLS->GetMousePos()))
		{		
			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockX == false && m_bLockZ == false)
				m_bLockX = m_bLockY = true;
		}

		if (hitRectXY.HitTest(CONTROLS->GetMousePos()) || (m_bLockX == true && m_bLockY == true))
			BX2D->SetColor(150, 150, 255, 0.8f);
		else
			BX2D->SetColor(50, 50, 255, 0.4f);

		BX2D->FillPolygon(r, 4);

		BX2D->SetColor(100, 100, 255, 0.8f);
		BX2D->DrawLine(r[0], r[1]);
		BX2D->DrawLine(r[1], r[2]);
		BX2D->DrawLine(r[2], r[3]);
		BX2D->DrawLine(r[3], r[0]);

		if (hitRectX.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255, 255, 255);
			BX2D->DrawEllipse(
				posLineX_2D.x,
				posLineX_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockZ == false)
				m_bLockX = true;
		}

		if (!CONTROLS->LeftMBDown())
			m_bLockX = m_bLockY = m_bLockZ = false;

		D3DXVECTOR3 mousePosPlusZX(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineX_2D.z);
		D3DXVECTOR3 mousePosPlusZX_3D;
		D3DXVec3Unproject(&mousePosPlusZX_3D, &mousePosPlusZX, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZX_3D.x -= l*100;

		if (m_bLockX)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineX_2D.x,
				posLineX_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			float diffX = m_OldModelPos[id].x - mousePosPlusZX_3D.x;

			pLevelObject->Translate(Vector3(-diffX,0,0));

			m_OldModelPos[id].x = mousePosPlusZX_3D.x;
		}
		else
		{
			m_OldModelPos[id].x = mousePosPlusZX_3D.x;
		}

		// Y
		HitRegion hitRectY(
			HitRegion::TYPE_ELLIPSE,
			posLineY_2D.x,
			posLineY_2D.y,
			(float)size,
			(float)size);

		BX2D->SetColor(0, 255, 0);
		BX2D->DrawLine(
			posLineY2_2D.x,
			posLineY2_2D.y,
			posLineY_2D.x,
			posLineY_2D.y,
			2.0f);

		BX2D->FillEllipse(
			posLineY_2D.x,
			posLineY_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->SetColor(0, 255, 0);
		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(
			_T("Y"),
			posLineY_2D.x - 5,
			posLineY_2D.y - 25);
	
		//D2D1_POINT_2F r[4];
		r[0].x = posLineY2_2D.x;
		r[0].y = posLineY2_2D.y;
		r[1].x = posLineYZ_2D.x;
		r[1].y = posLineYZ_2D.y;
		r[2].x = posLineZ2_2D.x;
		r[2].y = posLineZ2_2D.y;
		r[3].x = pos2D.x;
		r[3].y = pos2D.y;
		r[4].x = posLineY2_2D.x;
		r[4].y = posLineY2_2D.y;

		HitRegion hitRectYZ(HitRegion::TYPE_POLYGON, &r[0], 5);
						
		if (hitRectYZ.HitTest(CONTROLS->GetMousePos()))
		{
			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockX == false && m_bLockZ == false)
				m_bLockY = m_bLockZ = true;
		}

		if (hitRectYZ.HitTest(CONTROLS->GetMousePos()) || (m_bLockY == true && m_bLockZ == true))
			BX2D->SetColor(150, 150, 255, 0.8f);
		else
			BX2D->SetColor(50, 50, 255, 0.4f);

		BX2D->FillPolygon(r,4);

		BX2D->SetColor(100, 100, 255, 0.8f);
		BX2D->DrawLine(r[0],r[1]);
		BX2D->DrawLine(r[1],r[2]);
		BX2D->DrawLine(r[2],r[3]);
		BX2D->DrawLine(r[3],r[0]);

		if (hitRectY.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineY_2D.x,
				posLineY_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockX == false && m_bLockZ == false)
			{
				m_bLockY = true;
			}
		}

		D3DXVECTOR3 mousePosPlusZY(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineY_2D.z);
		D3DXVECTOR3 mousePosPlusZY_3D;
		D3DXVec3Unproject(&mousePosPlusZY_3D, &mousePosPlusZX, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZY_3D.y -= l*100;

		if (m_bLockY)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineY_2D.x,
				posLineY_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);
			
			float diffY = m_OldModelPos[id].y - mousePosPlusZY_3D.y;

			pLevelObject->Translate(Vector3(0, -diffY, 0));

			m_OldModelPos[id].y = mousePosPlusZY_3D.y;
		}
		else
		{
			m_OldModelPos[id].y = mousePosPlusZY_3D.y;
		}
	
		// Z
		HitRegion hitRectZ(
			HitRegion::TYPE_ELLIPSE,
			posLineZ_2D.x,
			posLineZ_2D.y,
			(float)size,
			(float)size);

		BX2D->SetColor(255, 255, 0);
		BX2D->DrawLine(
			posLineZ2_2D.x,
			posLineZ2_2D.y,
			posLineZ_2D.x,
			posLineZ_2D.y,
			2.0f);

		BX2D->FillEllipse(
			posLineZ_2D.x,
			posLineZ_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->SetColor(255, 255, 0);
		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(
			_T("Z"),
			posLineZ_2D.x - 5,
			posLineZ_2D.y - 25);

		//D2D1_POINT_2F r[4];
		r[0].x = posLineZX_2D.x;
		r[0].y = posLineZX_2D.y;
		r[1].x = posLineX2_2D.x;
		r[1].y = posLineX2_2D.y;
		r[2].x = pos2D.x;
		r[2].y = pos2D.y;
		r[3].x = posLineZ2_2D.x;
		r[3].y = posLineZ2_2D.y;
		r[4].x = posLineZX_2D.x;
		r[4].y = posLineZX_2D.y;

		HitRegion hitRectZX(HitRegion::TYPE_POLYGON, &r[0], 5);
						
		if (hitRectZX.HitTest(CONTROLS->GetMousePos()))
		{
			if (CONTROLS->LeftMBDown() && m_bLockY == false && m_bLockX == false && m_bLockZ == false)
				m_bLockZ = m_bLockX = true;
		}

		if (hitRectZX.HitTest(CONTROLS->GetMousePos()) || (m_bLockZ == true && m_bLockX == true))
			BX2D->SetColor(150, 150, 255, 0.8f);
		else
			BX2D->SetColor(50, 50, 255, 0.4f);

		BX2D->FillPolygon(r,4);

		BX2D->SetColor(100, 100, 255, 0.8f);
		BX2D->DrawLine(r[0],r[1]);
		BX2D->DrawLine(r[1],r[2]);
		BX2D->DrawLine(r[2],r[3]);
		BX2D->DrawLine(r[3],r[0]);

		if (hitRectZ.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineZ_2D.x,
				posLineZ_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockX == false && m_bLockY == false)
			{
				m_bLockZ = true;
			}
		}

		D3DXVECTOR3 mousePosPlusZZ(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineZ_2D.z);
		D3DXVECTOR3 mousePosPlusZZ_3D;
		D3DXVec3Unproject(&mousePosPlusZZ_3D, &mousePosPlusZZ, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZZ_3D.z -= l*100;

		if (m_bLockZ)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineZ_2D.x,
				posLineZ_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			float diffZ = m_OldModelPos[id].z - mousePosPlusZZ_3D.z;

			pLevelObject->Translate(Vector3(0, 0, -diffZ));

			m_OldModelPos[id].z = mousePosPlusZZ_3D.z;
		}
		else
		{
			m_OldModelPos[id].z = mousePosPlusZZ_3D.z;
		}
	}
}

void MoveGizmo::Tick(const RenderContext* pRenderContext, vector<ILevelObject*> pLevelObjects)
{
	m_pRenderContext = pRenderContext;

    //TODO

	while (m_pRenderContext->GetLightController()->GetLights().size() > m_OldLightPos.size())
	{
		m_OldLightPos.push_back(D3DXVECTOR3(0,0,0));
	}

	while (pLevelObjects.size() > m_OldModelPos.size())
	{
		m_OldModelPos.push_back(D3DXVECTOR3(0,0,0));
	}
}

// GETTERS
bool MoveGizmo::IsMoving() const
{
	if (m_bLockX || m_bLockY || m_bLockZ)
		return true;
	else
		return false;
}