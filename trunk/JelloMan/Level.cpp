#include "Level.h"
#include "ContentManager.h"
#include "TestPhysXBox.h"
#include "TestPhysXSphere.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
                m_pTestObject(new TestObject()),
                m_pTestObject2(new TestObject2()),
				m_pRenderContext(0),
				m_pCharacter(0),
				m_pBaseGrid(new BaseGrid(pDXDevice)),
				m_bShowGrid(false),
				m_bTickCharacter(false)
{

}


Level::~Level()
{
    delete m_pTestObject;
    delete m_pTestObject2;
	for (int i = 0; i < m_vecActor.size(); ++i)
		delete m_vecActor[i];
	delete m_pCharacter;

	delete m_pBaseGrid;

	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
		delete *it;
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, Camera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;

    m_pTestObject->Init(pPhysXEngine);
    m_pTestObject2->Init();

	TestPhysXBox* pTestPhysXBox = new TestPhysXBox(pPhysXEngine, Vector3(0, 50, 0));
	pTestPhysXBox->Init();
	m_vecActor.push_back(pTestPhysXBox);

	m_pCharacter = new Character(pTrackingCamera);
	m_pCharacter->Init();

	m_pBaseGrid->Init();
}

void Level::Tick(const float dTime)
{
	if (CONTROLS->IsKeyDown(VK_SPACE))
	{
        if (rand() % 2 == 0)
        {
		    TestPhysXBox* pTestPhysXBox = new TestPhysXBox(m_pPhysXEngine, m_pRenderContext->GetCamera()->GetPosition());
		    pTestPhysXBox->Init();
		    m_vecActor.push_back(pTestPhysXBox);
		    pTestPhysXBox->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
        }
        else
        {
            TestPhysXSphere* pTestPhysXShere = new TestPhysXSphere(m_pPhysXEngine, m_pRenderContext->GetCamera()->GetPosition());
		    pTestPhysXShere->Init();
		    m_vecActor.push_back(pTestPhysXShere);
		    pTestPhysXShere->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
        }
	}

	if (m_bTickCharacter)
		m_pCharacter->Tick(dTime);
	
	for (int i = 0; i < m_vecActor.size(); ++i)
		m_vecActor[i]->Update(dTime);

	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		(*it)->Tick(dTime);
	}
}

void Level::DrawDeferred(const RenderContext* pRenderContext)
{
	//m_pTestObject->Draw(pRenderContext);
	
	m_pCharacter->Draw(pRenderContext);
<<<<<<< .mine
	for (int i = 0; i < m_vecTestPhysXBox.size(); ++i)
		m_vecTestPhysXBox[i]->Draw(pRenderContext);
=======	for (int i = 0; i < m_vecActor.size(); ++i)
		m_vecActor[i]->Draw(pRenderContext);
>>>>>>> .theirs
	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		(*it)->Draw(pRenderContext);
	}

	m_pRenderContext = pRenderContext;
}

void Level::DrawForward(const RenderContext* pRenderContext)
{
	//m_pTestObject2->Draw(pRenderContext);

	if (m_bShowGrid)
		m_pBaseGrid->Draw(pRenderContext);
}

void Level::AddLevelObject(LevelObject* pLevelObject)
{
	m_pLevelObjects.push_back(pLevelObject);
}