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
	//delete m_pModel;
	/*delete m_pEffect;
	delete m_pEffectNormals;
	delete m_pDiffuseMap;
	delete m_pSpecMap;
	delete m_pGlossMap;
	delete m_pNormalMap;*/
}

void LevelObject::Init(PhysX* pPhysX)
{
    m_pDiffuseMap = Content->LoadTexture2D(m_DiffusePath);
    m_pSpecMap = Content->LoadTexture2D(m_SpecPath);
    m_pGlossMap = Content->LoadTexture2D(m_GlossPath);
	
	m_pNormalMap = Content->LoadTexture2D(m_NormalPath);
	
	if (m_bUseNormalMap)
		m_pEffectNormals = Content->LoadEffect<DeferredPreEffectNormals>(_T("predeferredNormal.fx"));
	else
		m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("predeferred.fx"));

    m_pModel = Content->LoadModel(m_ModelPath);
    
    for (vector<ModelMesh<VertexPosNormTanTex>*>::const_iterator it = m_pModel->GetModelMeshes().cbegin(); it != m_pModel->GetModelMeshes().cend(); ++it)
    {
		if (m_bUseNormalMap)
			(*it)->SetEffect(m_pEffectNormals);
		else
			(*it)->SetEffect(m_pEffect);
    }

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

	m_bInitialized = true;
}

void LevelObject::Tick(const float dTime)
{
    Actor::Tick(dTime);
}

void LevelObject::Draw(const RenderContext* pRenderContext)
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

    m_pModel->Draw();
}
void LevelObject::DrawEffectless()
{
    m_pModel->DrawEffectless();
}
