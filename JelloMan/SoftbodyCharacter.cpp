#include "SoftbodyCharacter.h"
#include "ContentManager.h"

SoftbodyCharacter::SoftbodyCharacter(const Vector3& pos): Softbody(), m_vPosition(pos)
{
}


SoftbodyCharacter::~SoftbodyCharacter()
{
}

void SoftbodyCharacter::Init(PhysX* pPhysX)
{
    SoftbodyMesh* pSBMesh = Content->LoadSoftbodyMesh(_T("../Sphere50SB.binobj"));
    InitSoftbody(pPhysX, pSBMesh, _T("../Sphere50SB.nxsoftbody"), m_vPosition);

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("../Content/Effects/predeferred.fx"));

    m_pDiffuseMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_diffuse.png"));
    m_pSpecMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_spec.png"));
    m_pGlossMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_gloss.png"));
}

void SoftbodyCharacter::Selected(bool selected)
{
    m_bIsSelected = selected;
}

void SoftbodyCharacter::Tick(const float dTime)
{
    TransformPositions();

	if (CONTROLS->IsKeyDown(VK_CONTROL))
        AddForce(Vector3::Up * -80000000000, GetPosition());
	if (CONTROLS->IsKeyDown(VK_UP))
		AddSpeed(Vector3::Forward * 800, GetPosition());
	if (CONTROLS->IsKeyDown(VK_DOWN))
		AddSpeed(-Vector3::Forward * 800, GetPosition());
	if (CONTROLS->IsKeyDown(VK_LEFT))
		AddSpeed(-Vector3::Right * 800, GetPosition());
	if (CONTROLS->IsKeyDown(VK_RIGHT))
		AddSpeed(Vector3::Right * 800, GetPosition());
}

void SoftbodyCharacter::Draw(RenderContext* pRenderContext)
{
    m_pEffect->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());
    m_pEffect->SetWorld(Matrix::Identity);
    m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

	SetVertices();

    m_pSoftbodyMesh->Draw(m_pEffect);
}
void SoftbodyCharacter::DrawShadow(RenderContext* pRenderContext, PreShadowEffect* e)
{
    e->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());
	
	SetVertices();

    m_pSoftbodyMesh->Draw(e);
}
