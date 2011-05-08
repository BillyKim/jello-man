#include "TestObject.h"
#include "ContentManager.h"
#include "PhysXMesh.h"

TestObject::TestObject(void): m_pModel(0), m_pEffect(0), m_mtxWorld(Matrix::Identity), m_pDiffuseMap(0), m_pSpecMap(0) ,m_pGlossMap(0)
{
}


TestObject::~TestObject(void)
{
}

void TestObject::Init(PhysX* pPhysX)
{
    m_pDiffuseMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_diffuse.png"));
    m_pSpecMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_spec.png"));
    m_pGlossMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_gloss.png"));

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("../Content/Effects/predeferred.fx"));

//    m_pModel = Content->LoadModel(_T("Content/Models/lvl_opt_physX.binobj"));

	PhysXMesh mesh = PhysXMesh(pPhysX, "../Content/Models/lvl_opt_physX.nxconcave", 10000);
    InitActor(pPhysX, mesh, false);
}

void TestObject::Draw(const RenderContext* pRenderContext)
{
    m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

    m_pEffect->SetWorld(m_mtxWorld);
	m_pEffect->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());

    m_pModel->Draw(m_pEffect);
}
