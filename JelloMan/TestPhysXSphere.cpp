#include "TestPhysXSphere.h"
#include "PhysXSphere.h"
#include "ContentManager.h"

TestPhysXSphere::TestPhysXSphere(PhysX* pPhysX, const Vector3& pos)
{
    PhysXSphere sphere(50.0f, 1000);
    InitActor(pPhysX, sphere, true);
    Translate(pos);
}


TestPhysXSphere::~TestPhysXSphere(void)
{

}

void TestPhysXSphere::Init()
{
    m_pDiffuseMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_diffuse.png"));
    m_pSpecMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_spec.png"));
    m_pGlossMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_gloss.png"));

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("../Content/Effects/predeferred.fx"));

    //m_pModel = Content->LoadModel(_T("../Content/Models/sphere50.binobj"));
}

void TestPhysXSphere::Draw(const RenderContext* pRenderContext)
{
    m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

	m_pEffect->SetWorld(m_WorldMatrix);
	m_pEffect->SetWorldViewProjection(m_WorldMatrix * pRenderContext->GetCamera()->GetViewProjection());

    m_pModel->Draw(m_pEffect);
}