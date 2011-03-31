#include "Level.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
                m_pTestObject(new TestObject()),
                m_pTestObject2(new TestObject2()),
				m_pRenderContext(0),
				m_pCharacter(0)
{

}


Level::~Level()
{
    delete m_pTestObject;
    delete m_pTestObject2;
	delete m_pCharacter;
}

// GENERAL
void Level::Initialize()
{
    m_pTestObject->Init();
    m_pTestObject2->Init();

	m_pCharacter = new Character();
	m_pCharacter->Init();
}

void Level::Tick(const float dTime)
{
	m_pCharacter->Tick(dTime);
}

void Level::DrawDeferred(const RenderContext* pRenderContext)
{
	m_pTestObject->Draw(pRenderContext);
	m_pCharacter->Draw(pRenderContext);

	m_pRenderContext = pRenderContext;
}

void Level::DrawForward(const RenderContext* pRenderContext)
{
	//m_pTestObject2->Draw(pRenderContext);
}