#include "TestSoftbody.h"
#include "ContentManager.h"

TestSoftbody::TestSoftbody(const Vector3& pos): Softbody(), m_vPosition(pos)
{
}


TestSoftbody::~TestSoftbody()
{
}

void TestSoftbody::Init(PhysX* pPhysX)
{
    SoftbodyMesh* pSBMesh = Content->LoadSoftbodyMesh(_T("../Sphere50SB.binobj"));
    InitSoftbody(pPhysX, pSBMesh, _T("../Sphere50SB.nxsoftbody"), m_vPosition);

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("../Content/Effects/predeferred.fx"));

    m_pDiffuseMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_diffuse.png"));
    m_pSpecMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_spec.png"));
    m_pGlossMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_gloss.png"));
}

void TestSoftbody::Selected(bool selected)
{
    m_bIsSelected = selected;
}

void TestSoftbody::Tick(const float /*dTime*/)
{
    TransformPositions();
}

void TestSoftbody::Draw(RenderContext* pRenderContext)
{
    m_pEffect->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());
    m_pEffect->SetWorld(Matrix::Identity);
    m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

	SetVertices();

    m_pSoftbodyMesh->Draw(m_pEffect);
}
void TestSoftbody::DrawShadow(RenderContext* pRenderContext)
{
    pRenderContext->GetPreShadowEffect()->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());
	
	SetVertices();

    m_pSoftbodyMesh->Draw(pRenderContext->GetPreShadowEffect());
}
