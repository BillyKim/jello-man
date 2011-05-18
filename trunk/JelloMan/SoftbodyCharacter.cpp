#include "SoftbodyCharacter.h"
#include "ContentManager.h"

SoftbodyCharacter::SoftbodyCharacter(const Vector3& pos, Graphics::Camera::FollowCamera* pCamera): 
        Softbody(), 
        m_vPosition(pos), 
        m_RotSpeed(Pi / 2.0f),
        m_pCamera(pCamera)
{
}


SoftbodyCharacter::~SoftbodyCharacter()
{
}

void SoftbodyCharacter::Init(PhysX* pPhysX)
{
    SoftbodyMesh* pSBMesh = Content->LoadSoftbodyMesh(_T("../Sphere50SB.binobj"));
    InitSoftbody(pPhysX, pSBMesh, _T("../Sphere50SB.nxsoftbody"), m_vPosition);
    SetPosition(m_vPosition);

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("../Content/Effects/predeferred.fx"));

    m_pDiffuseMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_diffuse.png"));
    m_pSpecMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_spec.png"));
    m_pGlossMap = Content->LoadTexture2D(_T("../Content/Textures/weapon_gloss.png"));

    m_pSoftbody->setGroup(1);
}

void SoftbodyCharacter::Selected(bool selected)
{
    m_bIsSelected = selected;
}

void SoftbodyCharacter::Tick(const float dTime)
{
    TransformPositions();

    NxVec3 grav;
    m_pPhysX->GetScene()->getGravity(grav);
    NxRay ray(GetPosition(), Vector3::Up * ((grav.y < 0)? -1 : 1));
    bool isTouchingGround = m_pPhysX->GetScene()->raycastAnyShape(ray, NxShapesType::NX_STATIC_SHAPES, 1, GetRadius()*2);

    if (isTouchingGround)
    {
        if (CONTROLS->IsKeyPressed(VK_CONTROL))
        {
            NxVec3 grav;
            m_pPhysX->GetScene()->getGravity(grav);
            m_pPhysX->GetScene()->setGravity(-grav);
        }
        Vector3 forward = m_pCamera->GetLook();
        forward.Y = 0;
        forward.Normalize();
	    if (CONTROLS->IsKeyDown(VK_UP))
        {
            if (GetSpeed().Length() < 800)
                AddSpeed(forward * 8000 * dTime, GetPosition());
        }
        else
	    if (CONTROLS->IsKeyDown(VK_DOWN))
        {
            if (GetSpeed().Length() < 800)
		        AddSpeed(-forward * 8000 * dTime, GetPosition());
        }
	    if (CONTROLS->IsKeyDown(VK_LEFT))
        {
            AddSpeed(-m_pCamera->GetRight() * 8000 * dTime, GetPosition());
        }
        else if (CONTROLS->IsKeyDown(VK_RIGHT))
        {	
		    AddSpeed(m_pCamera->GetRight() * 8000 * dTime, GetPosition());
        }
    }
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
