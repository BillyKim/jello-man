#include "MoveGizmo.h"


// CONSTRUCTOR - DESTRUCTOR
MoveGizmo::MoveGizmo()	:	m_bLockX(false),
							m_bLockY(false),
							m_bLockZ(false),
							m_pRenderContext(0)
{
}


MoveGizmo::~MoveGizmo()
{
	m_pRenderContext = 0;
}

// GENERAL
void MoveGizmo::Show(Vector3& position, TYPE type, int id, LevelObject* pLevelObject)
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

	D3DXVECTOR3 pos;
	// POSITION
	if (type != TYPE_MODEL)
		pos = position.ToD3DVector3();
	else
	{
		pos = D3DXVECTOR3(
			pLevelObject->GetPosition().X,
			pLevelObject->GetPosition().Y,
			pLevelObject->GetPosition().Z	);
	}

	Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();

	Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - Vector3(pos);
	float l = length.Length();
	l *= 0.001f;

	D3DXVECTOR3 pos2D;
	D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

	if (vLook.Dot(length) < 0 &&
		pos2D.x < BLOX_2D->GetWindowSize().width &&
		pos2D.x > 0 &&
		pos2D.y < BLOX_2D->GetWindowSize().height &&
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
			static_cast<int>(posLineX_2D.x),
			static_cast<int>(posLineX_2D.y),
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
	
		BLOX_2D->SetColor(255, 0, 0);
		BLOX_2D->SetFont(_T("Verdana"), false, false, 14);
		BLOX_2D->DrawString(_T("X"),
			static_cast<int>(posLineX_2D.x-5),
			static_cast<int>(posLineX_2D.y) - 25);

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
			BLOX_2D->SetColor(150, 150, 255, 0.8f);
		else
			BLOX_2D->SetColor(50, 50, 255, 0.4f);

		BLOX_2D->FillPolygon(r, 4);

		BLOX_2D->SetColor(100, 100, 255, 0.8f);
		BLOX_2D->DrawLine(r[0], r[1]);
		BLOX_2D->DrawLine(r[1], r[2]);
		BLOX_2D->DrawLine(r[2], r[3]);
		BLOX_2D->DrawLine(r[3], r[0]);

		if (hitRectX.HitTest(CONTROLS->GetMousePos()))
		{
			BLOX_2D->SetColor(255, 255, 255);
			BLOX_2D->DrawEllipse(
				static_cast<int>(posLineX_2D.x),
				static_cast<int>(posLineX_2D.y),
				static_cast<int>(size/2),
				static_cast<int>(size/2),
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
			BLOX_2D->SetColor(255,255,255);
			BLOX_2D->DrawEllipse(
				static_cast<int>(posLineX_2D.x),
				static_cast<int>(posLineX_2D.y),
				static_cast<int>(size/2),
				static_cast<int>(size/2),
				2.0f);

			if (type == TYPE_POINTLIGHT)
			{
				float diff = m_OldPointLightPos[id].x - mousePosPlusZX_3D.x;

				position.X -= diff;

				m_OldPointLightPos[id].x = mousePosPlusZX_3D.x;
			}
			else if (type == TYPE_SPOTLIGHT)
			{
				float diff = m_OldSpotLightPos[id].x - mousePosPlusZX_3D.x;

				position.X -= diff;

				m_OldSpotLightPos[id].x = mousePosPlusZX_3D.x;
			}
			else if (type == TYPE_MODEL)
			{
				float diffX = m_OldModelPos[id].x - mousePosPlusZX_3D.x;

				pLevelObject->Translate(Vector3(-diffX,0,0));

				m_OldModelPos[id].x = mousePosPlusZX_3D.x;
			}
		}
		else
		{
			if (type == TYPE_POINTLIGHT)
				m_OldPointLightPos[id].x = mousePosPlusZX_3D.x;
			else if (type == TYPE_SPOTLIGHT)
				m_OldSpotLightPos[id].x = mousePosPlusZX_3D.x;
			else if (type == TYPE_MODEL)
				m_OldModelPos[id].x = mousePosPlusZX_3D.x;
		}

		// Y
		HitRegion hitRectY(
			HitRegion::TYPE_ELLIPSE,
			static_cast<int>(posLineY_2D.x),
			static_cast<int>(posLineY_2D.y),
			size,
			size);

		BLOX_2D->SetColor(0, 255, 0);
		BLOX_2D->DrawLine(static_cast<int>(posLineY2_2D.x),
			static_cast<int>(posLineY2_2D.y),
			static_cast<int>(posLineY_2D.x),
			static_cast<int>(posLineY_2D.y),
			2.0f);

		BLOX_2D->FillEllipse(
			static_cast<int>(posLineY_2D.x),
			static_cast<int>(posLineY_2D.y),
			static_cast<int>(size/2),
			static_cast<int>(size/2));

		BLOX_2D->SetColor(0, 255, 0);
		BLOX_2D->SetFont(_T("Verdana"), false, false, 14);
		BLOX_2D->DrawString(_T("Y"),
			static_cast<int>(posLineY_2D.x-5),
			static_cast<int>(posLineY_2D.y) - 25);
	
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
			BLOX_2D->SetColor(150, 150, 255, 0.8f);
		else
			BLOX_2D->SetColor(50, 50, 255, 0.4f);

		BLOX_2D->FillPolygon(r,4);

		BLOX_2D->SetColor(100, 100, 255, 0.8f);
		BLOX_2D->DrawLine(r[0],r[1]);
		BLOX_2D->DrawLine(r[1],r[2]);
		BLOX_2D->DrawLine(r[2],r[3]);
		BLOX_2D->DrawLine(r[3],r[0]);

		if (hitRectY.HitTest(CONTROLS->GetMousePos()))
		{
			BLOX_2D->SetColor(255,255,255);
			BLOX_2D->DrawEllipse(
				static_cast<int>(posLineY_2D.x),
				static_cast<int>(posLineY_2D.y),
				static_cast<int>(size/2),
				static_cast<int>(size/2),
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
			BLOX_2D->SetColor(255,255,255);
			BLOX_2D->DrawEllipse(
				static_cast<int>(posLineY_2D.x),
				static_cast<int>(posLineY_2D.y),
				static_cast<int>(size/2),
				static_cast<int>(size/2),
				2.0f);
			
			if (type == TYPE_POINTLIGHT)
			{
				float diff = m_OldPointLightPos[id].y - mousePosPlusZY_3D.y;

				position.Y -= diff;

				m_OldPointLightPos[id].y = mousePosPlusZY_3D.y;
			}
			else if (type == TYPE_SPOTLIGHT)
			{
				float diff = m_OldSpotLightPos[id].y - mousePosPlusZY_3D.y;

				position.Y -= diff;

				m_OldSpotLightPos[id].y = mousePosPlusZY_3D.y;
			}
			else if (type == TYPE_MODEL)
			{
				float diffY = m_OldModelPos[id].y - mousePosPlusZY_3D.y;

				pLevelObject->Translate(Vector3(0, -diffY, 0));

				m_OldModelPos[id].y = mousePosPlusZY_3D.y;
			}
		}
		else
		{
			if (type == TYPE_POINTLIGHT)
				m_OldPointLightPos[id].y = mousePosPlusZY_3D.y;
			else if (type == TYPE_SPOTLIGHT)
				m_OldSpotLightPos[id].y = mousePosPlusZY_3D.y;
			else if (type == TYPE_MODEL)
				m_OldModelPos[id].y = mousePosPlusZY_3D.y;
		}
	
		// Z
		HitRegion hitRectZ(
			HitRegion::TYPE_ELLIPSE,
			static_cast<int>(posLineZ_2D.x),
			static_cast<int>(posLineZ_2D.y),
			size,
			size);

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

		BLOX_2D->SetColor(255, 255, 0);
		BLOX_2D->SetFont(_T("Verdana"), false, false, 14);
		BLOX_2D->DrawString(_T("Z"),
			static_cast<int>(posLineZ_2D.x-5),
			static_cast<int>(posLineZ_2D.y) - 25);

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
			BLOX_2D->SetColor(150, 150, 255, 0.8f);
		else
			BLOX_2D->SetColor(50, 50, 255, 0.4f);

		BLOX_2D->FillPolygon(r,4);

		BLOX_2D->SetColor(100, 100, 255, 0.8f);
		BLOX_2D->DrawLine(r[0],r[1]);
		BLOX_2D->DrawLine(r[1],r[2]);
		BLOX_2D->DrawLine(r[2],r[3]);
		BLOX_2D->DrawLine(r[3],r[0]);

		if (hitRectZ.HitTest(CONTROLS->GetMousePos()))
		{
			BLOX_2D->SetColor(255,255,255);
			BLOX_2D->DrawEllipse(
				static_cast<int>(posLineZ_2D.x),
				static_cast<int>(posLineZ_2D.y),
				static_cast<int>(size/2),
				static_cast<int>(size/2),
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
			BLOX_2D->SetColor(255,255,255);
			BLOX_2D->DrawEllipse(
				static_cast<int>(posLineZ_2D.x),
				static_cast<int>(posLineZ_2D.y),
				static_cast<int>(size/2),
				static_cast<int>(size/2),
				2.0f);

			if (type == TYPE_POINTLIGHT)
			{
				float diff = m_OldPointLightPos[id].z - mousePosPlusZZ_3D.z;

				position.Z -= diff;

				m_OldPointLightPos[id].z = mousePosPlusZZ_3D.z;
			}
			else if (type == TYPE_SPOTLIGHT)
			{
				float diff = m_OldSpotLightPos[id].z - mousePosPlusZZ_3D.z;

				position.Z -= diff;

				m_OldSpotLightPos[id].z = mousePosPlusZZ_3D.z;
			}
			else if (type == TYPE_MODEL)
			{
				float diffZ = m_OldModelPos[id].z - mousePosPlusZZ_3D.z;

				pLevelObject->Translate(Vector3(0, 0, -diffZ));

				m_OldModelPos[id].z = mousePosPlusZZ_3D.z;
			}
		}
		else
		{
			if (type == TYPE_POINTLIGHT)
				m_OldPointLightPos[id].z = mousePosPlusZZ_3D.z;
			else if (type == TYPE_SPOTLIGHT)
				m_OldSpotLightPos[id].z = mousePosPlusZZ_3D.z;
			else if (type == TYPE_MODEL)
				m_OldModelPos[id].z = mousePosPlusZZ_3D.z;
		}
	}
}

void MoveGizmo::Tick(const RenderContext* pRenderContext, vector<LevelObject*> pLevelObjects)
{
	m_pRenderContext = pRenderContext;

    //TODO

	//while (m_pRenderContext->GetLightController()->GetPointLights().size() > m_OldPointLightPos.size())
	//{
	//	m_OldPointLightPos.push_back(D3DXVECTOR3(0,0,0));
	//}

	//while (m_pRenderContext->GetLightController()->GetSpotLights().size() > m_OldSpotLightPos.size())
	//{
	//	m_OldSpotLightPos.push_back(D3DXVECTOR3(0,0,0));
	//}

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