#include "TestObject2.h"
#include "ContentManager.h"

TestObject2::TestObject2(void): m_pModel(0), m_pEffect(0)//, m_pDiffuseMap(0), m_pSpecMap(0) ,m_pGlossMap(0)
{
    D3DXMatrixIdentity(&m_mtxWorld);
}


TestObject2::~TestObject2(void)
{
}

void TestObject2::Init()
{
    //m_pDiffuseMap = Content->LoadTexture2D(_T("Content/Models/testtex.png"));
    //m_pSpecMap = Content->LoadTexture2D(_T("Content/Models/testtex.png"));
    //m_pGlossMap = Content->LoadTexture2D(_T("Content/Models/testtex.png"));

    m_pEffect = Content->LoadEffect<PhongEffect>(_T("phong.fx"));

    m_pModel = Content->LoadModel(_T("Content/Models/test3.obj"));
   
    for (vector<ModelMesh<VertexPosNormTex>*>::const_iterator it = m_pModel->GetModelMeshes().cbegin(); it != m_pModel->GetModelMeshes().cend(); ++it)
    {
        (*it)->SetEffect(m_pEffect);
    }
}

void TestObject2::Draw(const RenderContext* pRenderContext)
{
    //m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    //m_pEffect->SetSpecMap(m_pSpecMap);
    //m_pEffect->SetGlossMap(m_pGlossMap);

    m_pEffect->SetWorld(m_mtxWorld);
	m_pEffect->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());

    m_pModel->Draw();
}
