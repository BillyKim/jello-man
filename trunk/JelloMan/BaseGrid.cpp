#include "BaseGrid.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
BaseGrid::BaseGrid()	:	m_pEffect(0),
												m_mtxWorld(Matrix::Identity),
												m_pBaseGrid(0),
												m_pBaseGridCenter(0)

{
}

BaseGrid::~BaseGrid()
{
	
}

// GENERAL
void BaseGrid::Init()
{
	m_pEffect = Content->LoadEffect<PosColEffect>(_T("../Content/Effects/poscol.fx"));

	m_pBaseGrid = Content->LoadSpline(_T("../Content/Models/basegrid.obj"));
	m_pBaseGridCenter = Content->LoadSpline(_T("../Content/Models/basegrid_center.obj"));
}

void BaseGrid::Draw(const RenderContext* pRenderContext)
{
	m_pEffect->SetWorld(m_mtxWorld);
	m_pEffect->SetWorldViewProjection(m_mtxWorld * pRenderContext->GetCamera()->GetViewProjection());

	m_pEffect->SetColor(Color(0.f,.1f,.2f,1.f));
	m_pBaseGrid->Draw(m_pEffect);

	m_pEffect->SetColor(Color(0.f,.25f,.5f,1.f));
	m_pBaseGridCenter->Draw(m_pEffect);
}