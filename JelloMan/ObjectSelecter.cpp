#include "ObjectSelecter.h"
#include <algorithm>
#include "Light.h"

// CONSTRUCTOR - DESTRUCTOR
ObjectSelecter::ObjectSelecter(PhysX* pPhysXEngine)	:	m_bClick(false),
														m_pPhysXEngine(pPhysXEngine),
                                                        m_vCenterPos(Vector3::Zero)
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
}


ObjectSelecter::~ObjectSelecter()
{
}

// GENERAL
void ObjectSelecter::Tick(const RenderContext* pRenderContext)
{
	m_ViewPort.Width = (UINT)BX2D->GetWindowSize().width;
	m_ViewPort.Height = (UINT)BX2D->GetWindowSize().height;

    CheckControls(pRenderContext);
}

void ObjectSelecter::CheckControls(const RenderContext* pRenderContext)
{
	if (CONTROLS->LeftMBDown())
		m_bClick = true;

	if (CONTROLS->LeftMBUp())
	{
		if (m_bClick)
		{
			if (CONTROLS->GetMousePos().x > 200 && CONTROLS->GetMousePos().y > 50)
            {
                if (CONTROLS->IsKeyDown(VK_CONTROL) == false)
                    DeselectAll();
                if (TrySelectLight(pRenderContext) == false)
                {
                    if (TrySelectObject(pRenderContext) == true)
                        CalcCenterPos();
                }
                else
                    CalcCenterPos();
            }

			m_bClick = false;
		}
	}

	if (CONTROLS->IsKeyDown(VK_LCONTROL) && CONTROLS->IsKeyDown('D'))
	{
		DeselectAll();
	}
}


void ObjectSelecter::DeselectAll()
{
    for_each(m_SelectedObjects.begin(), m_SelectedObjects.end(), [](IEditorObject* obj)
	{
		obj->Selected(false);
	});

    m_SelectedObjects.clear();

    m_vCenterPos = Vector3::Infinity;
}

bool ObjectSelecter::TrySelectLight(const RenderContext* pRenderContext)
{
	// MATRIX
	Matrix matProj = pRenderContext->GetCamera()->GetProjection();
	Matrix matView = pRenderContext->GetCamera()->GetView();
	Matrix matWorld = Matrix::Identity;

	Vector3 vPos2DNear = Vector3(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, 0);
    Vector3 vPosNear = Vector3::UnProject(vPos2DNear, &m_ViewPort, matProj, matView, matWorld);

    float dist = FloatMax();
    Light* pSelectedLight = 0;

    for_each(pRenderContext->GetLightController()->GetLights().cbegin(),
        pRenderContext->GetLightController()->GetLights().cend(), [&](Light* light)
    {
        if (light->GetHitRegion()->HitTest(CONTROLS->GetMousePos()))
        {
            float temp = (vPosNear - light->GetPosition()).LengthSqr();
            if (temp < dist)
            {
                dist = temp;
                pSelectedLight = light;
            }
        }
    });

    if (pSelectedLight != 0)
    {
        if (pSelectedLight->IsSelected())
        {
            m_SelectedObjects.erase(remove(m_SelectedObjects.begin(), m_SelectedObjects.end(), pSelectedLight));
            pSelectedLight->Selected(false);
        }
        else
        {
            pSelectedLight->Selected(true);
            m_SelectedObjects.push_back(dynamic_cast<IEditorObject*>(pSelectedLight));
        }
        return true;
    }
    else
        return false;
}
bool ObjectSelecter::TrySelectObject(const RenderContext* pRenderContext)
{
	// MATRIX
	Matrix matProj = pRenderContext->GetCamera()->GetProjection();
	Matrix matView = pRenderContext->GetCamera()->GetView();
	Matrix matWorld = Matrix::Identity;

	Vector3 vPos2DNear = Vector3(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, 0);
	Vector3 vPos2DFar = Vector3(CONTROLS->GetMousePos().x, CONTROLS->GetMousePos().y, 1);

    Vector3 vPosNear = Vector3::UnProject(vPos2DNear, &m_ViewPort, matProj, matView, matWorld);
	Vector3 vPosFar = Vector3::UnProject(vPos2DFar, &m_ViewPort, matProj, matView, matWorld);

	Vector3 vLook = vPosFar - vPosNear;
	vLook.Normalize();

    //Raycast --->
	NxRay ray;
	NxRaycastHit hit;

    ray.orig = vPosNear;
	ray.dir = vLook;

	NxShape* shape = m_pPhysXEngine->GetScene()->raycastClosestShape(ray, NX_ALL_SHAPES, hit);
    //<-----

	if (shape && shape->getGlobalPosition() != NxVec3(0,0,0))
	{
        ASSERT(shape->getActor().userData != 0, "");
        ILevelObject* obj = reinterpret_cast<ILevelObject*>(shape->getActor().userData);

        if (obj->IsSelected())
        {
            m_SelectedObjects.erase(remove(m_SelectedObjects.begin(), m_SelectedObjects.end(), obj));
            obj->Selected(false);
        }
        else
        {
		    m_SelectedObjects.push_back(dynamic_cast<IEditorObject*>(obj));
            obj->Selected(true);
        }
        return true;
	}
    return false;
}
void ObjectSelecter::AbortControls()
{
    m_bClick = false;
}

void ObjectSelecter::CalcCenterPos()
{
    if (m_SelectedObjects.size() > 0)
    {
        m_vCenterPos = Vector3::Zero;
        for_each(m_SelectedObjects.cbegin(), m_SelectedObjects.cend(), [&](IEditorObject* obj)
        {
            m_vCenterPos += obj->GetPosition();
        });
        m_vCenterPos /= static_cast<float>(m_SelectedObjects.size());
    }
    else
        m_vCenterPos = Vector3::Infinity;
}