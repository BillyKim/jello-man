#include "RotateGizmo.h"
#include "ContentManager.h"

RotateGizmo::RotateGizmo()	:	m_pRenderContext(0),
								m_pAxisFont(0),
                                m_bLockX(false),
                                m_bLockY(false),
                                m_bLockZ(false)
{
	m_pAxisFont = Content->LoadTextFormat(_T("Verdana"),14, false, false);

    ZeroMemory(&m_ViewPort, sizeof(m_ViewPort));
    m_ViewPort.MinDepth = 0;
    m_ViewPort.MaxDepth = 1;
    m_ViewPort.TopLeftX = 0;
    m_ViewPort.TopLeftY = 0;
}


RotateGizmo::~RotateGizmo()
{
}

void RotateGizmo::Draw()
{
    if (m_vCenterPos != Vector3::Infinity)
        DrawGizmo(m_vCenterPos);
}

void RotateGizmo::DrawAxis(const Vector3& pos, Axis axis,
                           const Matrix& world, const Matrix& view, const Matrix& projection)
{
	BX2D->SetFont(m_pAxisFont);
    tstring label = _T("");
    Vector3 vAxis;
    bool isSelected = false;
    D2D1_COLOR_F color;
    color.a = 1.0f;
    switch (axis)
    {
        case Axis_X: 
            label = _T("X"); 
            vAxis = Vector3::Right; 
            if (m_bLockX) isSelected = true;
            color.r = 1.0f;
            break;
        case Axis_Y: 
            label = _T("Y"); 
            vAxis = Vector3::Up;  
            if (m_bLockY) isSelected = true;
            color.g = 1.0f;
            break;
        case Axis_Z: 
            label = _T("Z"); 
            vAxis = -Vector3::Forward; 
            if (m_bLockZ) isSelected = true; 
            color.r = 1.0f; color.g = 1.0f;
            break;
    }

    BX2D->SetColor(color);

    Vector3 pointStart;
    Vector3 pointEnd;

    Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - pos;
    float l = length.Length() * 0.001f;

    pointStart = pos;
    pointEnd = pos + vAxis * AXIS_LENGTH * l;

    pointStart = Vector3::Project(pointStart, &m_ViewPort, projection, view, world);
    pointEnd = Vector3::Project(pointEnd, &m_ViewPort, projection, view, world);

    BX2D->DrawLine(
		pointStart.X,
		pointStart.Y,
		pointEnd.X,
		pointEnd.Y,
		2.0f);

    if (isSelected)
    {
        BX2D->SetColor(255, 255, 255);
        BX2D->FillEllipse(
		pointEnd.X,
		pointEnd.Y,
		static_cast<float>(END_ELLIPSE_RADIUS*2.5f),
		static_cast<float>(END_ELLIPSE_RADIUS*2.5f));
        BX2D->SetColor(color);
    }

	BX2D->FillEllipse(
		pointEnd.X,
		pointEnd.Y,
		static_cast<float>(END_ELLIPSE_RADIUS*2),
		static_cast<float>(END_ELLIPSE_RADIUS*2));

	BX2D->DrawString(label,
		pointStart.X - 20,
		pointStart.Y - 10);
    
    
}
void RotateGizmo::DrawGizmo(const Vector3& pos)
{
	// MATRIX
	Matrix proj = m_pRenderContext->GetCamera()->GetProjection();
	Matrix view = m_pRenderContext->GetCamera()->GetView();
	Matrix world = Matrix::Identity;
    
    Vector3 objLook(pos - m_pRenderContext->GetCamera()->GetPosition());
    objLook.Normalize();

    if (objLook.Dot(m_pRenderContext->GetCamera()->GetLook()) > 0.5f)
    {
        DrawAxis(pos, Axis_X, world, view, proj);
        DrawAxis(pos, Axis_Y, world, view, proj);
        DrawAxis(pos, Axis_Z, world, view, proj);
    }
}

// GENERAL
void RotateGizmo::Tick(const RenderContext* pRenderContext, ObjectSelecter* pObjectSelecter)
{
    m_vCenterPos = pObjectSelecter->GetCenterPos();
    if (m_vCenterPos != Vector3::Infinity)
    {
        // VIEWPORT
	    m_ViewPort.Width = (UINT)BX2D->GetWindowSize().width;
	    m_ViewPort.Height = (UINT)BX2D->GetWindowSize().height;

	    m_pRenderContext = pRenderContext;

        if (CONTROLS->LeftMBUp() == true)
	        m_bLockX = m_bLockY = m_bLockZ = false;

        if (m_bLockX || m_bLockY || m_bLockZ)
            pObjectSelecter->AbortControls();

        CheckControls(pObjectSelecter);
    }
}

void RotateGizmo::CheckControls(ObjectSelecter* pObjectSelecter)
{ 
	// MATRIX
	Matrix matProj = m_pRenderContext->GetCamera()->GetProjection();
	Matrix matView = m_pRenderContext->GetCamera()->GetView();
	Matrix matWorld = Matrix::Identity;

	// VIEWPORT
	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BX2D->GetWindowSize().width;
	viewP.Height = (UINT)BX2D->GetWindowSize().height;
	viewP.MinDepth = 0;
	viewP.MaxDepth = 1;

	Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();

	Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - m_vCenterPos;
    float l = length.Length() * 0.001f;

    //project position on screen
	Vector3 pos2D(Vector3::Project(m_vCenterPos, &m_ViewPort, matProj, matView, matWorld));

	if (vLook.Dot(length) < 0 &&
		pos2D.X < BX2D->GetWindowSize().width &&
		pos2D.X > 0 &&
		pos2D.Y < BX2D->GetWindowSize().height &&
		pos2D.Y > 0)
	{
		// VIEWPORT PROJECTION --->
		Vector3 posLineX(m_vCenterPos);
        posLineX.X += AXIS_LENGTH * l;
		Vector3 posLineY(m_vCenterPos);
		posLineY.Y += AXIS_LENGTH * l;
		Vector3 posLineZ(m_vCenterPos);
		posLineZ.Z += AXIS_LENGTH * l;

        Vector3 posLineX_2D(Vector3::Project(posLineX, &m_ViewPort, matProj, matView, matWorld));
		Vector3 posLineY_2D(Vector3::Project(posLineY, &m_ViewPort, matProj, matView, matWorld));
		Vector3 posLineZ_2D(Vector3::Project(posLineZ, &m_ViewPort, matProj, matView, matWorld));
        //<-------

		// Lock Checks ----->
        if (CONTROLS->LeftMBDown() == true)
        {
            #pragma region
            //X
                if (m_bLockY == false && m_bLockZ == false)
                {
		            HitRegion hitRectX(
			                            HitRegion::TYPE_ELLIPSE,
			                            posLineX_2D.X,
			                            posLineX_2D.Y,
                                        (float)END_ELLIPSE_RADIUS * 2,
			                            (float)END_ELLIPSE_RADIUS * 2);

		            if (hitRectX.HitTest(CONTROLS->GetMousePos()))
		            {
			            m_bLockX = true;
                    }
                }	
            //Y
                if (m_bLockX == false && m_bLockZ == false)
                {
                    HitRegion hitRectY(
			                            HitRegion::TYPE_ELLIPSE,
			                            posLineY_2D.X,
			                            posLineY_2D.Y,
			                            (float)END_ELLIPSE_RADIUS * 2,
			                            (float)END_ELLIPSE_RADIUS * 2);

		            if (hitRectY.HitTest(CONTROLS->GetMousePos()))
		            {
				        m_bLockY = true;
		            }
                }
            //Z
                if (m_bLockX == false && m_bLockY == false)
                {
                    HitRegion hitRectZ(
			                            HitRegion::TYPE_ELLIPSE,
			                            posLineZ_2D.X,
			                            posLineZ_2D.Y,
			                            (float)END_ELLIPSE_RADIUS * 2,
			                            (float)END_ELLIPSE_RADIUS * 2);

		            if (hitRectZ.HitTest(CONTROLS->GetMousePos()))
		            {
				        m_bLockZ = true;
                    }
                }
            #pragma endregion
        }
        //<---------------
		      
        if (m_bLockX == true)
        {
            float move((CONTROLS->GetMouseMovement().y) / 100.0f);
            /*for_each(pObjectSelecter->GetSelectedObjects().begin(),
                     pObjectSelecter->GetSelectedObjects().end(), 
                     [&](IEditorObject* obj)
            {
                obj->Rotate(Vector3::Right, move);
            });*/
            for (UINT i = 0; i < pObjectSelecter->GetSelectedObjects().size(); ++i)
            {
                pObjectSelecter->GetSelectedObjects()[i]->Rotate(Vector3::Right, move);
            }
        }
        if (m_bLockY == true)
        {
            float move((CONTROLS->GetMouseMovement().x) / 100.0f);
            /*for_each(pObjectSelecter->GetSelectedObjects().begin(),
                     pObjectSelecter->GetSelectedObjects().end(), 
                     [&](IEditorObject* obj)
            {
                obj->Rotate(Vector3::Up, move);
            });*/
            for (UINT i = 0; i < pObjectSelecter->GetSelectedObjects().size(); ++i)
            {
                pObjectSelecter->GetSelectedObjects()[i]->Rotate(Vector3::Up, move);
            }
        }
        if (m_bLockZ == true)
        {
            float move((CONTROLS->GetMouseMovement().y) / 100.0f);
            /*for_each(pObjectSelecter->GetSelectedObjects().begin(),
                     pObjectSelecter->GetSelectedObjects().end(), 
                     [&](IEditorObject* obj)
            {
                obj->Rotate(Vector3::Forward, move);
            });*/
            for (UINT i = 0; i < pObjectSelecter->GetSelectedObjects().size(); ++i)
            {
                pObjectSelecter->GetSelectedObjects()[i]->Rotate(Vector3::Forward, move);
            }
        }
	}
}

