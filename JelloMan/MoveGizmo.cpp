#include "MoveGizmo.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
MoveGizmo::MoveGizmo()	:	m_pRenderContext(0),
								m_pAxisFont(0),
                                m_bLockX(false),
                                m_bLockY(false),
                                m_bLockZ(false),
                                m_vAnchor(Vector3::Zero)
{
	m_pAxisFont = Content->LoadTextFormat(_T("Verdana"),14, false, false);

    ZeroMemory(&m_ViewPort, sizeof(m_ViewPort));
    m_ViewPort.MinDepth = 0;
    m_ViewPort.MaxDepth = 1;
    m_ViewPort.TopLeftX = 0;
    m_ViewPort.TopLeftY = 0;
}


MoveGizmo::~MoveGizmo()
{
}

void MoveGizmo::Draw()
{
    if (m_vCenterPos != Vector3::Infinity)
        DrawGizmo(m_vCenterPos);
}

void MoveGizmo::DrawAxis(const Vector3& pos, Axis axis,
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

    Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - pos;
    float l = length.Length() * 0.001f;

    Vector3 pointStart(pos + vAxis * AXIS_SMALL_LENGTH * l);
    Vector3 pointEnd(pos + vAxis * AXIS_LENGTH * l);

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
		pointStart.X,
		pointStart.Y);
    
    
}
void MoveGizmo::Draw2Axis(const Vector3& pos, Axis axis1, Axis axis2,
                           const Matrix& world, const Matrix& view, const Matrix& projection)
{
    Vector3 vAxis1, vAxis2;
    bool isSelected = false;
    //Init
    #pragma region
    switch (axis1)
    {
        case Axis_X: 
            vAxis1 = Vector3::Right; 
            isSelected = m_bLockX;
            break;
        case Axis_Y: 
            vAxis1 = Vector3::Up;  
            isSelected = m_bLockY;
            break;
        case Axis_Z: 
            vAxis1 = -Vector3::Forward; 
            isSelected = m_bLockZ; 
            break;
    }
    switch (axis2)
    {
        case Axis_X: 
            vAxis2 = Vector3::Right; 
            isSelected = m_bLockX && isSelected;
            break;
        case Axis_Y: 
            vAxis2 = Vector3::Up;  
            isSelected = m_bLockY && isSelected;
            break;
        case Axis_Z: 
            vAxis2 = -Vector3::Forward; 
            isSelected = m_bLockZ && isSelected; 
            break;
    }
    #pragma endregion

    BX2D->SetColor(50, 50, 255, 0.5f);

    Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - pos;
    float l = length.Length() * 0.001f;

    Vector3 p1 = Vector3::Project(pos, 
        &m_ViewPort, projection, view, world);
    Vector3 p2 = Vector3::Project(pos + vAxis1 * AXIS_SMALL_LENGTH * l,
        &m_ViewPort, projection, view, world);
    Vector3 p3 = Vector3::Project(pos + (vAxis1 + vAxis2) * AXIS_SMALL_LENGTH * l,
        &m_ViewPort, projection, view, world);
    Vector3 p4 = Vector3::Project(pos + vAxis2 * AXIS_SMALL_LENGTH * l, 
        &m_ViewPort, projection, view, world);

	Point2D poly[4];
    poly[0] = Point2F(p1.X , p1.Y);
    poly[1] = Point2F(p2.X , p2.Y);
    poly[2] = Point2F(p3.X , p3.Y);
    poly[3] = Point2F(p4.X , p4.Y);

    if (isSelected)
    {
        BX2D->SetColor(150, 150, 255, 0.5f);
    }

    BX2D->FillPolygon(poly, 4);
    
    BX2D->SetColor(100, 100, 255);
    BX2D->DrawPolygon(poly, 4, true, 2);
}

void MoveGizmo::DrawGizmo(const Vector3& pos)
{
	Matrix proj = m_pRenderContext->GetCamera()->GetProjection();
	Matrix view = m_pRenderContext->GetCamera()->GetView();
	Matrix world = Matrix::Identity;
    
    Vector3 objLook(pos - m_pRenderContext->GetCamera()->GetPosition());
    objLook.Normalize();

    if (objLook.Dot(m_pRenderContext->GetCamera()->GetLook()) > 0.5f)
    {
        Draw2Axis(pos, Axis_X, Axis_Y, world, view, proj);
        Draw2Axis(pos, Axis_X, Axis_Z, world, view, proj);
        Draw2Axis(pos, Axis_Y, Axis_Z, world, view, proj);
        DrawAxis(pos, Axis_X, world, view, proj);
        DrawAxis(pos, Axis_Y, world, view, proj);
        DrawAxis(pos, Axis_Z, world, view, proj);
    }
}

// GENERAL
void MoveGizmo::Tick(const RenderContext* pRenderContext, ObjectSelecter* pObjectSelecter)
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

void MoveGizmo::CheckControls(ObjectSelecter* pObjectSelecter)
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
        bool setAnchor = false;
        if (CONTROLS->LeftMBDown() == true)
        {
            if (m_bLockX == false && m_bLockY == false && m_bLockZ == false)
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
                
                //XZ - XY - YZ
                if (m_bLockX == false && m_bLockY == false && m_bLockZ == false)
                {
                    if (PolyCollisionCheck(m_vCenterPos, Vector3::Right * l, Vector3::Forward * -l, matProj, matView, matWorld))
                    {
                        m_bLockX = true;
                        m_bLockZ = true;
                    }
                    else if (PolyCollisionCheck(m_vCenterPos, Vector3::Right * l, Vector3::Up * l, matProj, matView, matWorld))
                    {
                        m_bLockX = true;
                        m_bLockY = true;
                    }
                    else if (PolyCollisionCheck(m_vCenterPos, Vector3::Up * l, Vector3::Forward * -l, matProj, matView, matWorld))
                    {
                        m_bLockY = true;
                        m_bLockZ = true;
                    }
                }
                #pragma endregion
                setAnchor = (m_bLockX == true || m_bLockY == true || m_bLockZ == true);
            }
        }
        //<---------------
		    
        if (m_bLockX || m_bLockY || m_bLockZ)
        {
            if (m_bLockX == true)
            {
                Vector3 mousePos = Vector3(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineX_2D.Z);
                Vector3 mousePos3D = Vector3::UnProject(mousePos, &m_ViewPort, matProj, matView, matWorld);
                if (setAnchor)
                    m_vAnchor.X = (mousePos3D - posLineX).X;
                float diff = (mousePos3D - posLineX).X - m_vAnchor.X;
                
                for (UINT i = 0; i < pObjectSelecter->GetSelectedObjects().size(); ++i)
                {
                    pObjectSelecter->GetSelectedObjects()[i]->Translate(Vector3::Right * diff);
                }
            }
            if (m_bLockY == true)
            {
                Vector3 mousePos = Vector3(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineY_2D.Z);
                Vector3 mousePos3D = Vector3::UnProject(mousePos, &m_ViewPort, matProj, matView, matWorld);
                if (setAnchor)
                    m_vAnchor.Y = (mousePos3D - posLineY).Y;
                float diff = (mousePos3D - posLineY).Y - m_vAnchor.Y;

                for (UINT i = 0; i < pObjectSelecter->GetSelectedObjects().size(); ++i)
                {
                    pObjectSelecter->GetSelectedObjects()[i]->Translate(Vector3::Up * diff);
                }
            }
            if (m_bLockZ == true)
            {
                Vector3 mousePos = Vector3(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, posLineZ_2D.Z);
                Vector3 mousePos3D = Vector3::UnProject(mousePos, &m_ViewPort, matProj, matView, matWorld);
                if (setAnchor)
                    m_vAnchor.Z = (mousePos3D - posLineZ).Z;
                float diff = (mousePos3D - posLineZ).Z - m_vAnchor.Z;

                for (UINT i = 0; i < pObjectSelecter->GetSelectedObjects().size(); ++i)
                {
                    pObjectSelecter->GetSelectedObjects()[i]->Translate(Vector3::Forward * -diff);
                }
            }

            pObjectSelecter->CalcCenterPos();
        }
	}

    
}
bool MoveGizmo::PolyCollisionCheck(const Vector3& pos, const Vector3& vAxis1, const Vector3& vAxis2, 
                                       const Matrix& proj, const Matrix& view, const Matrix& world)
{
    Vector3 p1 = Vector3::Project(pos, 
        &m_ViewPort, proj, view, world);
    Vector3 p2 = Vector3::Project(pos + vAxis1 * AXIS_SMALL_LENGTH,
        &m_ViewPort, proj, view, world);
    Vector3 p3 = Vector3::Project(pos + (vAxis1 + vAxis2) * AXIS_SMALL_LENGTH,
        &m_ViewPort, proj, view, world);
    Vector3 p4 = Vector3::Project(pos + vAxis2 * AXIS_SMALL_LENGTH, 
        &m_ViewPort, proj, view, world);

    D2D1_POINT_2F poly[4];
    poly[0] = Point2F(p1.X , p1.Y);
    poly[1] = Point2F(p2.X , p2.Y);
    poly[2] = Point2F(p3.X , p3.Y);
    poly[3] = Point2F(p4.X , p4.Y);

    HitRegion hitr(HitRegion::TYPE_POLYGON,
            poly, 4);
    return hitr.HitTest(CONTROLS->GetMousePos());
}