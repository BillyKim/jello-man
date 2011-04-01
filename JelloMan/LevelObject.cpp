#include "LevelObject.h"
#include "ContentManager.h"
#include "PhysXMesh.h"

LevelObject::LevelObject()	:	m_pModel(0),
								m_pEffect(0),
								m_mtxWorld(Matrix::Identity), 
								m_pDiffuseMap(0),
								m_pSpecMap(0),
								m_pGlossMap(0),
								m_ModelPath(_T("")),
								m_DiffusePath(_T("")),
								m_SpecPath(_T("")),
								m_GlossPath(_T("")),
								m_bInitialized(false)
{
}


LevelObject::~LevelObject()
{
	delete m_pModel;
	//delete m_pEffect;
	//delete m_pDiffuseMap;
	//delete m_pSpecMap;
	//delete m_pGlossMap;
}

void LevelObject::Init(PhysX* pPhysX)
{
    m_pDiffuseMap = Content->LoadTexture2D(m_DiffusePath);
    m_pSpecMap = Content->LoadTexture2D(m_SpecPath);
    m_pGlossMap = Content->LoadTexture2D(m_GlossPath);

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("predeferred.fx"));

    m_pModel = Content->LoadModel(m_ModelPath);
    
    for (vector<ModelMesh<VertexPosNormTex>*>::const_iterator it = m_pModel->GetModelMeshes().cbegin(); it != m_pModel->GetModelMeshes().cend(); ++it)
    {
        (*it)->SetEffect(m_pEffect);
    }

	string s(m_PhysXModelPath.begin(), m_PhysXModelPath.end());
	s.assign(m_PhysXModelPath.begin(), m_PhysXModelPath.end());

	PhysXMesh mesh = PhysXMesh(pPhysX, s, 10000);
    InitActor(pPhysX, mesh, false);

	m_bInitialized = true;
}

void LevelObject::Tick(const float dTime)
{
	m_mtxWorld = m_mtxWorld.CreateTranslation(m_Pos);
}

void LevelObject::Draw(const RenderContext* pRenderContext)
{
    m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

    m_pEffect->SetWorld(m_mtxWorld);
	m_pEffect->SetWorldViewProjection(m_mtxWorld * pRenderContext->GetCamera()->GetViewProjection());

    m_pModel->Draw();
}
