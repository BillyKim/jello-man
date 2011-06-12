#include "SimpleSoftbody.h"
#include "ContentManager.h"
#include "UserData.h"

SimpleSoftbody::SimpleSoftbody(const Vector3& pos): 
                Softbody(), m_vPosition(pos), m_pSBMesh(0),
                m_strDiffusePath(_T("")), m_strSpecPath(_T("")),
                m_strGlossPath(_T("")), m_strNormalPath(_T("")),
                m_strModelPath(_T("")), m_strPhysXPath(_T("")),
                m_Timer(0), m_pUserData(0)
{
}


SimpleSoftbody::~SimpleSoftbody()
{
    delete m_pSBMesh;
    delete m_pUserData;
}

void SimpleSoftbody::Init(PhysX* pPhysX)
{
    #pragma region Textures
    if (m_strDiffusePath == _T(""))
        m_pTexDiffuse = Content->LoadTexture2D(DefaultTextureType_Gray);
    else
        m_pTexDiffuse = Content->LoadTexture2D(m_strDiffusePath);

    if (m_strSpecPath == _T(""))
        m_pTexSpec = Content->LoadTexture2D(DefaultTextureType_Gray);
    else
        m_pTexSpec = Content->LoadTexture2D(m_strSpecPath);

    if (m_strGlossPath == _T(""))
        m_pTexGloss = Content->LoadTexture2D(DefaultTextureType_Gray);
    else
        m_pTexGloss = Content->LoadTexture2D(m_strGlossPath);

    if (m_strNormalPath == _T(""))
        m_pTexNormal = Content->LoadTexture2D(DefaultTextureType_Normal);
    else
        m_pTexNormal = Content->LoadTexture2D(m_strNormalPath);
    #pragma endregion

    m_pEffect = Content->LoadEffect<DeferredPreEffectNormals>(_T("../Content/Effects/predeferredNormal.fx"));

    m_pSBMesh = Content->LoadSoftbodyMesh(m_strModelPath)->Copy();
    InitSoftbody(pPhysX, m_pSBMesh, m_strPhysXPath, m_vPosition);
    m_pUserData = new UserData(UserDataFlag_IsPickable, dynamic_cast<ILevelObject*>(this));
    m_pSoftbody->userData = m_pUserData;
}

void SimpleSoftbody::Selected(bool selected)
{
    m_bIsSelected = selected;
}

void SimpleSoftbody::Tick(const float dTime)
{
    m_Timer += dTime;

    if (m_Timer >= 1.0f / 60.0f)
    {
        TransformPositions();
	    SetVertices();
        m_Timer = 0.0f;
    }
}

void SimpleSoftbody::Draw(const RenderContext* pRenderContext)
{
    m_pEffect->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());
    m_pEffect->SetWorld(Matrix::Identity);

    m_pEffect->SetDiffuseMap(m_pTexDiffuse);
    m_pEffect->SetSpecMap(m_pTexSpec);
    m_pEffect->SetGlossMap(m_pTexGloss);
    m_pEffect->SetNormalMap(m_pTexNormal);
    m_pEffect->Selected(m_bIsSelected);

    m_pSoftbodyMesh->Draw(m_pEffect);
}
void SimpleSoftbody::DrawShadow(const RenderContext* pRenderContext)
{
    pRenderContext->GetPreShadowEffect()->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());

    m_pSoftbodyMesh->Draw(pRenderContext->GetPreShadowEffect());
}
