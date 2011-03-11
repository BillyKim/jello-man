#include "Level.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice), 
                m_pDeferredRenderer(new DeferredRenderer(pDXDevice)),
                m_pTestObject(new TestObject())
{

}


Level::~Level()
{
    delete m_pDeferredRenderer;
    delete m_pTestObject;
}

// GENERAL
void Level::Initialize()
{
	m_pDeferredRenderer->Init(	static_cast<int>(BLOX_2D->GetWindowSize().width),
								static_cast<int>(BLOX_2D->GetWindowSize().height)	);
    m_pTestObject->Init();
}

void Level::Tick(const float dTime)
{
}

void Level::Draw(const RenderContext* pRenderContext)
{
    m_pDeferredRenderer->Begin();

	m_pTestObject->Draw(pRenderContext);

    m_pDeferredRenderer->End();
}