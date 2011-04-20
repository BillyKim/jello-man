#include "RotateGizmo.h"
#include "ContentManager.h"

RotateGizmo::RotateGizmo()	:	m_pRenderContext(0),
								m_bLockXY(false),
								m_bLockYZ(false),
								m_bLockZX(false),
								m_pAxisFont(0)
{
	m_pAxisFont = Content->LoadTextFormat(_T("Verdana"),14, false, false);
}


RotateGizmo::~RotateGizmo()
{
	m_pRenderContext = 0;
}

// GENERAL
void RotateGizmo::Show(Vector3& position, Vector3& direction, TYPE type, int id, ILevelObject* pLevelObject)
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

	if (type != TYPE_MODEL)
	{
		pos = position.ToD3DVector3();
	}
	else
	{
		pos = pLevelObject->GetPosition().ToD3DVector3();
	}

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
	
		D3DXVECTOR3 posLineX = pos;
		posLineX.x += l*100;
		D3DXVECTOR3 posLineY = pos;
		posLineY.y += l*100;
		D3DXVECTOR3 posLineZ = pos;
		posLineZ.z += l*100;

		D3DXVECTOR3 posLineX2 = posLineX;
		posLineX2.y += l*20;
		D3DXVECTOR3 posLineY2 = posLineY;
		posLineY2.z += l*20;
		D3DXVECTOR3 posLineZ2 = posLineZ;
		posLineZ2.x += l*20;

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
			posLineX_2D.x,
			posLineX_2D.y,
			(float)size,
			(float)size);

		BX2D->SetColor(255, 0, 0);

		BX2D->DrawLine(
			pos2D.x,
			pos2D.y,
			posLineX_2D.x,
			posLineX_2D.y,
			2.0f);

		BX2D->DrawLine(
			posLineX2_2D.x,
			posLineX2_2D.y,
			posLineX_2D.x,
			posLineX_2D.y,
			1.0f);

		BX2D->FillEllipse(
			posLineX_2D.x,
			posLineX_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->FillEllipse(
			posLineX2_2D.x,
			posLineX2_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(_T("X"),
			posLineX_2D.x - 20,
			posLineX_2D.y - 10);

		if (hitRectX.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineX_2D.x,
				posLineX_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockYZ == false && m_bLockZX == false)
				m_bLockXY = true;
		}

		if (!CONTROLS->LeftMBDown())
				m_bLockXY = m_bLockYZ = m_bLockZX = false;

		D3DXVECTOR3 mousePosPlusZY(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineY_2D.z);
		D3DXVECTOR3 mousePosPlusZY_3D;
		D3DXVec3Unproject(&mousePosPlusZY_3D, &mousePosPlusZY, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZY_3D.y -= l*100;

		if (m_bLockXY)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineX_2D.x,
				posLineX_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			BX2D->DrawEllipse(
				posLineX2_2D.x,
				posLineX2_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (type == TYPE_SPOTLIGHT)
			{
				D3DXVECTOR3 dir(direction.X, direction.Y, direction.Z);
				D3DXMATRIX matRot;

				float diff = m_OldLightPos[id].y - mousePosPlusZY_3D.y;

				D3DXMatrixRotationZ(&matRot, -diff/10);

				D3DXVECTOR4 tran;
				D3DXVec3Transform(&tran, &dir, &matRot);

				direction = Vector3(tran.x, tran.y, tran.z);

				m_OldLightPos[id].y = mousePosPlusZY_3D.y;
			}
			/*else if (type == TYPE_MODEL)
			{			
				NxMat33 orientation = pLevelObject->GetActor()->getGlobalOrientation();

				float diff = m_OldModelPos[id].y - mousePosPlusZY_3D.y;

				m_OldModelRot[id].Z += -diff/10;

				pLevelObject->SetOrientation(m_OldModelRot[id]);

				m_OldModelPos[id].y = mousePosPlusZY_3D.y;
			}*/
		}
		else
		{
			if (type == TYPE_SPOTLIGHT)
				m_OldLightPos[id].y = mousePosPlusZY_3D.y;
			else if (type == TYPE_MODEL)
				m_OldModelPos[id].y = mousePosPlusZY_3D.y;
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
			pos2D.x,
			pos2D.y,
			posLineY_2D.x,
			posLineY_2D.y,
			2.0f);

		BX2D->DrawLine(
			posLineY2_2D.x,
			posLineY2_2D.y,
			posLineY_2D.x,
			posLineY_2D.y,
			1.0f);

		BX2D->FillEllipse(
			posLineY_2D.x,
			posLineY_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->FillEllipse(
			posLineY2_2D.x,
			posLineY2_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(_T("Y"),
			posLineY_2D.x - 5,
			posLineY_2D.y - 25);

		if (hitRectY.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineY_2D.x,
				posLineY_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockXY == false && m_bLockZX == false)
				m_bLockYZ = true;
		}

		D3DXVECTOR3 mousePosPlusZZ(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineZ_2D.z);
		D3DXVECTOR3 mousePosPlusZZ_3D;
		D3DXVec3Unproject(&mousePosPlusZZ_3D, &mousePosPlusZZ, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZZ_3D.z -= l*100;

		if (m_bLockYZ)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineY_2D.x,
				posLineY_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			BX2D->DrawEllipse(
				posLineY2_2D.x,
				posLineY2_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (type == TYPE_SPOTLIGHT)
			{
				D3DXVECTOR3 dir(direction.X, direction.Y, direction.Z);
				D3DXMATRIX matRot;

				float diff = m_OldLightPos[id].z - mousePosPlusZZ_3D.z;

				D3DXMatrixRotationX(&matRot, -diff/10);

				D3DXVECTOR4 tran;
				D3DXVec3Transform(&tran, &dir, &matRot);

				direction = Vector3(tran.x, tran.y, tran.z);

				m_OldLightPos[id].z = mousePosPlusZZ_3D.z;
			}
			/*else if (type == TYPE_MODEL)
			{			
				NxMat33 orientation = pLevelObject->GetActor()->getGlobalOrientation();

				float diff = m_OldModelPos[id].z - mousePosPlusZZ_3D.z;

				m_OldModelRot[id].X += -diff/10;

				pLevelObject->SetOrientation(m_OldModelRot[id]);

				m_OldModelPos[id].z = mousePosPlusZZ_3D.z;
			}*/
		}
		else
		{
			if (type == TYPE_SPOTLIGHT)
				m_OldLightPos[id].z = mousePosPlusZZ_3D.z;
			else if (type == TYPE_MODEL)
				m_OldModelPos[id].z = mousePosPlusZZ_3D.z;
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
			pos2D.x,
			pos2D.y,
			posLineZ_2D.x,
			posLineZ_2D.y,
			2.0f);

		BX2D->DrawLine(
			posLineZ2_2D.x,
			posLineZ2_2D.y,
			posLineZ_2D.x,
			posLineZ_2D.y,
			1.0f);

		BX2D->FillEllipse(
			posLineZ_2D.x,
			posLineZ_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->FillEllipse(
			posLineZ2_2D.x,
			posLineZ2_2D.y,
			size/2.0f,
			size/2.0f);

		BX2D->SetFont(m_pAxisFont);
		BX2D->DrawString(_T("Z"),
			posLineZ_2D.x - 5,
			posLineZ_2D.y - 25);

		if (hitRectZ.HitTest(CONTROLS->GetMousePos()))
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineZ_2D.x,
				posLineZ_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (CONTROLS->LeftMBDown() && m_bLockXY == false && m_bLockYZ == false)
				m_bLockZX = true;
		}

		D3DXVECTOR3 mousePosPlusZX(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineX_2D.z);
		D3DXVECTOR3 mousePosPlusZX_3D;
		D3DXVec3Unproject(&mousePosPlusZX_3D, &mousePosPlusZX, &viewP, &matProj, &matView, &matIdent);
		mousePosPlusZX_3D.x -= l*100;

		if (m_bLockZX)
		{
			BX2D->SetColor(255,255,255);
			BX2D->DrawEllipse(
				posLineZ_2D.x,
				posLineZ_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			BX2D->DrawEllipse(
				posLineZ2_2D.x,
				posLineZ2_2D.y,
				size/2.0f,
				size/2.0f,
				2.0f);

			if (type == TYPE_SPOTLIGHT)
			{
				D3DXVECTOR3 dir(direction.X, direction.Y, direction.Z);
				D3DXMATRIX matRot;

				float diff = m_OldLightPos[id].x - mousePosPlusZX_3D.x;

				D3DXMatrixRotationY(&matRot, -diff/10);

				D3DXVECTOR4 tran;
				D3DXVec3Transform(&tran, &dir, &matRot);

				direction = Vector3(tran.x, tran.y, tran.z);

				m_OldLightPos[id].x = mousePosPlusZX_3D.x;
			}
			/*else if (type == TYPE_MODEL)
			{
				NxMat33 orientation = pLevelObject->GetActor()->getGlobalOrientation();

				float diff = m_OldModelPos[id].x - mousePosPlusZX_3D.x;

				m_OldModelRot[id].Y += -diff/10;

				pLevelObject->SetOrientation(m_OldModelRot[id]);

				m_OldModelPos[id].x = mousePosPlusZX_3D.x;
			}*/
		}
		else
		{
			if (type == TYPE_SPOTLIGHT)
				m_OldLightPos[id].x = mousePosPlusZX_3D.x;
			else if (type == TYPE_MODEL)
				m_OldModelPos[id].x = mousePosPlusZX_3D.x;
		}

		BX2D->SetColor(50, 50, 255, 0.4f);
		BX2D->FillEllipse(
			pos2D.x,
			pos2D.y,
			size*5.0f,
			size*5.0f);

		BX2D->SetColor(100, 100, 255, 0.8f);
		BX2D->DrawEllipse(
			pos2D.x,
			pos2D.y,
			size*5.0f,
			size*5.0f);
	}
}

void RotateGizmo::Tick(const RenderContext* pRenderContext, vector<ILevelObject*> pLevelObjects)
{
	m_pRenderContext = pRenderContext;

	while (m_pRenderContext->GetLightController()->GetLights().size() > m_OldLightPos.size())
	{
		m_OldLightPos.push_back(D3DXVECTOR3(0,0,0));
	}

	while (pLevelObjects.size() > m_OldModelPos.size())
	{
		m_OldModelPos.push_back(D3DXVECTOR3(0,0,0));
		m_OldModelRot.push_back(Vector3(0,0,0));
	}
}

