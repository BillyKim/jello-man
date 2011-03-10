#include "TestObject.h"
#include "ContentManager.h"

TestObject::TestObject(void): m_pModel(0), m_pEffect(0), m_pDiffuseMap(0), m_pSpecMap(0) ,m_pGlossMap(0)
{
    D3DXMatrixIdentity(&m_mtxWorld);
}


TestObject::~TestObject(void)
{
}

void TestObject::Init()
{
    m_pDiffuseMap = Content->LoadTexture2D(_T(""));
    m_pSpecMap = Content->LoadTexture2D(_T(""));
    m_pGlossMap = Content->LoadTexture2D(_T(""));

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("predeferred.fx"));

    m_pModel = Content->LoadModel(_T(""));
}

void TestObject::Draw() //rendercontext ? => wvp 
{
    m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

    m_pEffect->SetWorld(m_mtxWorld);
    m_pEffect->SetWorldViewProjection(m_mtxWorld); //WRONG!!!

    m_pModel->Draw();
}
