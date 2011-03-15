#include "Level.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice), 
                m_pDeferredRenderer(new DeferredRenderer(pDXDevice)),
                m_pForwardRenderer(new ForwardRenderer(pDXDevice)),
                m_pTestObject(new TestObject()),
                m_pTestObject2(new TestObject2())
{

}


Level::~Level()
{
    delete m_pDeferredRenderer;
    delete m_pForwardRenderer;
    delete m_pTestObject;
    delete m_pTestObject2;
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
    m_pTestObject2->Init();

    m_pDeferredRenderer->SetClearColor(Vector4(0.1f, 0.1f, 0.9f, 1.0f));
}

void Level::Tick(const float dTime)
{
}

void Level::Draw(const RenderContext* pRenderContext)
{
    pRenderContext->GetCamera()->LookAt(Vector3(-225, 199, -197), Vector3(0, 0, 0), Vector3(0, 1, 0));
    pRenderContext->GetCamera()->SetLens();

 //   m_pDeferredRenderer->Begin();

	//m_pTestObject->Draw(pRenderContext);

 //   m_pDeferredRenderer->End();

    m_pForwardRenderer->Begin();
    m_pForwardRenderer->Clear(Vector4(0.1f, 0.2f, 0.5f, 1.0f));

    m_pTestObject2->Draw(pRenderContext);

    m_pForwardRenderer->End();
}

void Level::OnResize(ID3D10RenderTargetView* pRTView)
{
	m_pDeferredRenderer->OnResize();
	m_pDeferredRenderer->OnResized(	static_cast<int>(BLOX_2D->GetWindowSize().width),
									static_cast<int>(BLOX_2D->GetWindowSize().height),
									pRTView);
}