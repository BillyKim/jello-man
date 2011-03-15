#include "Level.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice), 
                m_pDeferredRenderer(new DeferredRenderer(pDXDevice)),
                m_pForwardRenderer(new ForwardRenderer(pDXDevice)),
                m_pTestObject(new TestObject())
{

}


Level::~Level()
{
    delete m_pDeferredRenderer;
    delete m_pForwardRenderer;
    delete m_pTestObject;
}

// GENERAL
void Level::Initialize()
{
    ID3D10RenderTargetView* rtv;
    m_pDXDevice->OMGetRenderTargets(1, &rtv, NULL);

	m_pDeferredRenderer->Init(	static_cast<int>(BLOX_2D->GetWindowSize().width),
								static_cast<int>(BLOX_2D->GetWindowSize().height),
                                rtv);
    m_pForwardRenderer->Init(m_pDeferredRenderer);
    m_pTestObject->Init();

    m_pDeferredRenderer->SetClearColor(Vector4(0.1f, 0.1f, 0.9f, 1.0f));
}

void Level::Tick(const float dTime)
{
}

void Level::Draw(const RenderContext* pRenderContext)
{
    m_pDeferredRenderer->Begin();

	m_pTestObject->Draw(pRenderContext);

    m_pDeferredRenderer->End();

    //m_pForwardRenderer->Begin();



    //m_pForwardRenderer->End();
}