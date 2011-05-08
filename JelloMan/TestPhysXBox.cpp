#include "TestPhysXBox.h"
#include "PhysXBox.h"
#include "PhysXMesh.h"
#include "ContentManager.h"

TestPhysXBox::TestPhysXBox(PhysX* pPhysX, const Vector3& pos) : Actor()
{
    PhysXBox box = PhysXBox(Vector3(50, 50, 50), 1000);
    //PhysXMesh box(pPhysX, "../Content/Models/box50.nxconvex", 1000);
    InitActor(pPhysX, box, true);
    Translate(pos);
}


TestPhysXBox::~TestPhysXBox(void)
{

}

void TestPhysXBox::Init()
{
    m_pDiffuseMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_diffuse.png"));
    m_pSpecMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_spec.png"));
    m_pGlossMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_gloss.png"));

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("../Content/Effects/predeferred.fx"));

//    m_pModel = Content->LoadModel(_T("../Content/Models/box50.binobj"));
}

void TestPhysXBox::Draw(const RenderContext* pRenderContext)
{
    m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

	m_pEffect->SetWorld(m_WorldMatrix);
	m_pEffect->SetWorldViewProjection(m_WorldMatrix * pRenderContext->GetCamera()->GetViewProjection());

    m_pModel->Draw(m_pEffect);
}