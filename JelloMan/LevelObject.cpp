#include "LevelObject.h"
#include "ContentManager.h"
#include "PhysXMesh.h"

LevelObject::LevelObject()	:	m_pModel(0),
								m_pEffect(0),
								m_pDiffuseMap(0),
								m_pSpecMap(0),
								m_pGlossMap(0),
								m_pNormalMap(0),
								m_ModelPath(_T("")),
								m_DiffusePath(_T("")),
								m_SpecPath(_T("")),
								m_GlossPath(_T("")),
								m_NormalPath(_T("")),
								m_bInitialized(false),
								m_bUseSimplifiedPhysXMesh(false),
								m_Mass(100),
								m_bRigid(false),
								m_bUseNormalMap(false),
								m_bIsSelected(false)
{
}


LevelObject::~LevelObject()
{
}

void LevelObject::Init(PhysX* pPhysX)
{
    m_pDiffuseMap = Content->LoadTexture2D(m_DiffusePath);
    m_pSpecMap = Content->LoadTexture2D(m_SpecPath);
    m_pGlossMap = Content->LoadTexture2D(m_GlossPath);
	
	m_pNormalMap = Content->LoadTexture2D(m_NormalPath);
	
	if (m_bUseNormalMap)
		m_pEffectNormals = Content->LoadEffect<DeferredPreEffectNormals>(_T("../Content/Effects/predeferredNormal.fx"));
	else
		m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("../Content/Effects/predeferred.fx"));

    m_pModel = Content->LoadModel(m_ModelPath);
    
	if (!m_bUseSimplifiedPhysXMesh)
	{
		string s(m_PhysXModelPath.begin(), m_PhysXModelPath.end());

		PhysXMesh mesh = PhysXMesh(pPhysX, s, static_cast<float>(m_Mass));

		InitActor(pPhysX, mesh, m_bRigid);
	}
	else
	{
		InitActor(pPhysX, *m_pPhysXShape, m_bRigid);
	}

    m_pActor->userData = dynamic_cast<ILevelObject*>(this);

	m_bInitialized = true;
}
void LevelObject::Selected(bool selected)
{
    m_bIsSelected = selected;
}

void LevelObject::Tick(const float dTime)
{
    Actor::Tick(dTime);
}

void LevelObject::Draw(RenderContext* pRenderContext)
{
	if (m_bUseNormalMap)
	{
		m_pEffectNormals->SetDiffuseMap(m_pDiffuseMap);
		m_pEffectNormals->SetSpecMap(m_pSpecMap);
		m_pEffectNormals->SetGlossMap(m_pGlossMap);
		m_pEffectNormals->SetNormalMap(m_pNormalMap);

		m_pEffectNormals->Selected(m_bIsSelected);

		m_pEffectNormals->SetWorld(m_WorldMatrix);
		m_pEffectNormals->SetWorldViewProjection(m_WorldMatrix * pRenderContext->GetCamera()->GetViewProjection());
	}
	else
	{
		m_pEffect->SetDiffuseMap(m_pDiffuseMap);
		m_pEffect->SetSpecMap(m_pSpecMap);
		m_pEffect->SetGlossMap(m_pGlossMap);

		m_pEffect->Selected(m_bIsSelected);

		m_pEffect->SetWorld(m_WorldMatrix);
		m_pEffect->SetWorldViewProjection(m_WorldMatrix * pRenderContext->GetCamera()->GetViewProjection());
	}
    if (m_bUseNormalMap)
	{
        m_pModel->Draw(m_pEffectNormals);
    }
    else
    {
        m_pModel->Draw(m_pEffect);
    }
}
void LevelObject::DrawShadow(RenderContext* pRenderContext, PreShadowEffect* e)
{
    e->SetWorldViewProjection(m_WorldMatrix * pRenderContext->GetCamera()->GetViewProjection());
    m_pModel->Draw(e);
}
