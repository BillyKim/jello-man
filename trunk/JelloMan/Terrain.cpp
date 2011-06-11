#include "Terrain.h"
#include "ContentManager.h"
#include "RenderContext.h"
#include "PhysXTerrain.h"
#include "DeferredPreEffectNormals.h"
#include "UserData.h"

Terrain::Terrain(void): m_bIsSelected(false), 
                        m_pEffect(0), m_pModel(0), m_pTexDiffuse(0), m_pTexSpec(0), m_pTexGloss(0), m_pTexNorm(0),
                        m_strDiffusePath(_T("")), m_strSpecPath(_T("")), m_strGlossPath(_T("")), m_strNormalPath(_T("")), 
                        m_strTerrainPath(_T("")),
                        m_pUserData(0)
{
}


Terrain::~Terrain(void)
{
    delete m_pUserData;
}


void Terrain::Init(PhysX* pPhysX)
{
    m_pEffect = Content->LoadEffect<DeferredPreEffectNormals>(_T("../Content/Effects/predeferredNormal.fx"));
    m_pModel = Content->LoadTerrain(m_strTerrainPath);

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
        m_pTexNorm = Content->LoadTexture2D(DefaultTextureType_Normal);
    else
        m_pTexNorm = Content->LoadTexture2D(m_strNormalPath);
    #pragma endregion

    PhysXTerrain terrainShape(pPhysX, m_strTerrainPath, 20, 5);
    m_pUserData = new UserData(UserDataFlag_IsPickable, dynamic_cast<ILevelObject*>(this));
    terrainShape.GetShape()->userData = m_pUserData;
    InitActor(pPhysX, terrainShape, false, false);
}

IEditorObject* Terrain::Copy() const
{
    Terrain* pCopy = new Terrain();
    pCopy->SetDiffusePath(m_strDiffusePath);
    pCopy->SetSpecPath(m_strSpecPath);
    pCopy->SetGlossPath(m_strGlossPath);
    pCopy->SetNormalPath(m_strNormalPath);
    pCopy->SetTerrainPath(m_strTerrainPath);
    pCopy->Init(m_pPhysX);

    return dynamic_cast<IEditorObject*>(pCopy);
}

void Terrain::Draw(const RenderContext* pRenderContext)
{
    m_pEffect->SetDiffuseMap(m_pTexDiffuse);
    m_pEffect->SetSpecMap(m_pTexSpec);
    m_pEffect->SetGlossMap(m_pTexGloss);
    m_pEffect->SetNormalMap(m_pTexNorm);

    m_pEffect->Selected(m_bIsSelected);
    m_pEffect->SetWorld(Matrix::CreateScale(Vector3(20.0f, 5.0f, 20.0f)) * m_mtxWorldMatrix);
    m_pEffect->SetWorldViewProjection(Matrix::CreateScale(Vector3(20.0f, 5.0f, 20.0f)) * m_mtxWorldMatrix * pRenderContext->GetCamera()->GetViewProjection());

    m_pModel->Draw(m_pEffect);
}
void Terrain::DrawShadow(const RenderContext* pRenderContext)
{
    pRenderContext->GetPreShadowEffect()->SetWorldViewProjection(
        Matrix::CreateScale(Vector3(20.0f, 5.0f, 20.0f)) * m_mtxWorldMatrix * pRenderContext->GetCamera()->GetViewProjection());
    m_pModel->Draw(pRenderContext->GetPreShadowEffect());
}