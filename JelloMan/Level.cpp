#include "Level.h"
#include "ContentManager.h"
#include "PhysXSphere.h"
#include "PhysXBox.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
				m_pRenderContext(0),
				m_pCharacter(0),
				m_pBaseGrid(new BaseGrid(pDXDevice)),
				m_bShowGrid(false),
				m_bTickCharacter(false)
{

}


Level::~Level()
{
	delete m_pCharacter;

	delete m_pBaseGrid;
	
	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
		delete *it;
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, Camera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;
	
	m_pBaseGrid->Init();

	//m_pCharacter = new Character(pTrackingCamera);

	//m_pCharacter->UseNormalMap(false);
	//m_pCharacter->UseSimplifiedPhysXMesh(false);

	//m_pCharacter->SetModelPath(_T("Content/Models/jman.binobj"));
	//m_pCharacter->SetPhysXModelPath(_T("Content/Models/jman.nxconcave"));

	//m_pCharacter->SetDiffusePath(_T("Content/Textures/weapon_color.png"));
	//m_pCharacter->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
	//m_pCharacter->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

	//m_pCharacter->SetRigid(true);

	//m_pCharacter->Init(pPhysXEngine);

	// LOAD NEW LEVELOBJECT - WITHOUT NORMAL MAP
	LevelObject* pLevelObject = new LevelObject();

	pLevelObject->UseNormalMap(false);
	pLevelObject->UseSimplifiedPhysXMesh(true);

	pLevelObject->SetModelPath(_T("Content/Models/sphere50.binobj"));
	
	PhysXSphere sphere(50.0f, 1000);
	pLevelObject->SetSimplifiedPhysXMesh(&sphere);

	pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_color.png"));
	pLevelObject->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
	pLevelObject->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

	pLevelObject->SetRigid(true);

	pLevelObject->Init(pPhysXEngine);

	pLevelObject->Translate(Vector3(0,200,0));

	m_pLevelObjects.push_back(pLevelObject);
}

void Level::Tick(const float dTime)
{
    if (CONTROLS->IsKeyPressed(VK_SPACE))
	{
        if (rand() % 2 == 0)
        {
		    // LOAD NEW LEVELOBJECT - WITH NORMAL MAP
			LevelObject* pLevelObject = new LevelObject();

			pLevelObject->UseNormalMap(true);
			pLevelObject->UseSimplifiedPhysXMesh(true);

			pLevelObject->SetModelPath(_T("Content/Models/sphere50.binobj"));
	
			PhysXSphere sphere(50.0f, 1000);
			pLevelObject->SetSimplifiedPhysXMesh(&sphere);

			pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_color.png"));
			pLevelObject->SetNormalPath(_T("Content/Textures/weapon_normal.png"));
			pLevelObject->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
			pLevelObject->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

			pLevelObject->SetRigid(true);

			pLevelObject->Init(m_pPhysXEngine);

			pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

			m_pLevelObjects.push_back(pLevelObject);

			pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
        }
        else
        {
            // LOAD NEW LEVELOBJECT - WITH NORMAL MAP
			LevelObject* pLevelObject = new LevelObject();

			pLevelObject->UseNormalMap(true);
			pLevelObject->UseSimplifiedPhysXMesh(true);

			pLevelObject->SetModelPath(_T("Content/Models/box50.binobj"));
	
			PhysXBox box(Vector3(50,50,50),1000);
			pLevelObject->SetSimplifiedPhysXMesh(&box);

			pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_color.png"));
			pLevelObject->SetNormalPath(_T("Content/Textures/weapon_normal.png"));
			pLevelObject->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
			pLevelObject->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

			pLevelObject->SetRigid(true);

			pLevelObject->Init(m_pPhysXEngine);

			pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

			m_pLevelObjects.push_back(pLevelObject);

			pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
        }
	}

	/*m_pCharacter->Update(dTime);

	if (m_bTickCharacter)
		m_pCharacter->Tick(dTime);*/

	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		(*it)->Update(dTime);
		(*it)->Tick(dTime);
	}
}

void Level::DrawDeferred(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	//m_pCharacter->Draw(pRenderContext);

	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		(*it)->Draw(pRenderContext);
	}
}

void Level::DrawForward(const RenderContext* pRenderContext)
{
	if (m_bShowGrid)
		m_pBaseGrid->Draw(pRenderContext);
}

void Level::AddLevelObject(LevelObject* pLevelObject)
{
	m_pLevelObjects.push_back(pLevelObject);
}