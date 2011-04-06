#include "VisualModelDebugger.h"

// CONSTRUCTOR - DESTRUCTOR
VisualModelDebugger::VisualModelDebugger(PhysX* pPhysXEngine)	:	m_bClick(false),
																	m_TotalModels(0),
																	m_TotalSelectedModels(0),
																	m_pRenderContext(0),
																	m_pPhysXEngine(pPhysXEngine)
{
}


VisualModelDebugger::~VisualModelDebugger()
{
}

// GENERAL
void VisualModelDebugger::Tick(const RenderContext* pRenderContex, vector<LevelObject*>& pLevelObjects)
{
	m_pLevelObjects = pLevelObjects;
	m_pRenderContext = pRenderContex;

	while (m_ModelsSelected.size() < pLevelObjects.size())
	{
		m_ModelsSelected.push_back(false);
	}

	CheckControls();
}

void VisualModelDebugger::CheckControls()
{
	if (CONTROLS->LeftMBDown())
		m_bClick = true;

	if (CONTROLS->LeftMBUp())
	{
		if (m_bClick)
		{
			RayCast();

			m_bClick = false;
		}
	}

	if (CONTROLS->IsKeyDown(VK_LCONTROL) && CONTROLS->IsKeyDown('D'))
	{
		DeselectAll();
	}
}

void VisualModelDebugger::Draw()
{
	// MATRIX
	D3DXMATRIX matProj = m_pRenderContext->GetCamera()->GetProjection();
	D3DXMATRIX matView = m_pRenderContext->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 0.0f;
	viewP.MaxDepth = 1.0f;

	for (unsigned int i = 0; i < m_ModelsSelected.size(); ++i)
	{
		if (m_ModelsSelected[i])
		{
			/*D3DXVECTOR3 pos(
				m_pLevelObjects[i]->GetActor()->getGlobalPosition().x,
				m_pLevelObjects[i]->GetActor()->getGlobalPosition().y,
				m_pLevelObjects[i]->GetActor()->getGlobalPosition().z
				);

			D3DXVECTOR3 pos2D;
			D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

			BLOX_2D->SetColor(255,255,255,0.5f);
			BLOX_2D->FillEllipse(static_cast<int>(pos2D.x), static_cast<int>(pos2D.y), 10,10);*/

			m_pLevelObjects[i]->Selected(true);
		}

		else
			m_pLevelObjects[i]->Selected(false);
	}
}

void VisualModelDebugger::DeselectAll()
{
	for (unsigned int i = 0; i < m_ModelsSelected.size(); ++i)
		m_ModelsSelected[i] = false;
}

void VisualModelDebugger::RayCast()
{
	// MATRIX
	D3DXMATRIX matProj = m_pRenderContext->GetCamera()->GetProjection();
	D3DXMATRIX matView = m_pRenderContext->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BLOX_2D->GetWindowSize().width;
	viewP.Height = (UINT)BLOX_2D->GetWindowSize().height;
	viewP.MinDepth = 0.0f;
	viewP.MaxDepth = 1.0f;

	D3DXVECTOR3 mPos2DNear = D3DXVECTOR3(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,0);
	D3DXVECTOR3 mPos2DFar = D3DXVECTOR3(CONTROLS->GetMousePos().x,CONTROLS->GetMousePos().y,1);

	D3DXVECTOR3 mPosNear;
	D3DXVec3Unproject(&mPosNear, &mPos2DNear, &viewP, &matProj, &matView, &matIdent);
	D3DXVECTOR3 mPosFar;
	D3DXVec3Unproject(&mPosFar, &mPos2DFar, &viewP, &matProj, &matView, &matIdent);

	D3DXVECTOR3 look = mPosFar - mPosNear;
	D3DXVec3Normalize(&look,&look);

	NxRay ray;
	NxRaycastHit hit;

	ray.orig = NxVec3(mPosNear.x,mPosNear.y,mPosNear.z);
	ray.dir = NxVec3(look.x,look.y,look.z);

	NxShape* shape = m_pPhysXEngine->GetScene()->raycastClosestShape(ray,NX_ALL_SHAPES,hit);

	if (shape && shape->getGlobalPosition() != NxVec3(0,0,0))
	{
		NxVec3 nxPosHit(shape->getGlobalPosition());
		Vector3 vPosHit(nxPosHit.x, nxPosHit.y, nxPosHit.z);

		for (unsigned int i = 0; i < m_pLevelObjects.size(); ++i)
		{
			NxVec3 nxPosModel(m_pLevelObjects[i]->GetPosition());
			Vector3 vPosModel(nxPosModel.x, nxPosModel.y, nxPosModel.z);

			if (vPosHit == vPosModel)
				m_ModelsSelected[i] = !m_ModelsSelected[i];
		}
	}
}