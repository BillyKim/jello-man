#include "Level.h"
#include "ContentManager.h"
#include "PhysXSphere.h"
#include "PhysXBox.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
				m_pRenderContext(0),
				m_pBaseGrid(new BaseGrid(pDXDevice)),
				m_bShowGrid(false),
				m_bTickCharacter(false),
				m_pCharacter(0)
{

}


Level::~Level()
{
	delete m_pBaseGrid;
	
	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
		delete *it;
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, Camera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;
	
	m_pBaseGrid->Init();

	// CHARACTER
	Character* pCharacter = new Character(pTrackingCamera);

	pCharacter->UseNormalMap(false);
	pCharacter->UseSimplifiedPhysXMesh(false);

	pCharacter->SetModelPath(_T("Content/Models/jman.binobj"));
	pCharacter->SetPhysXModelPath(_T("Content/Models/jman.nxconcave"));

	pCharacter->SetDiffusePath(_T("Content/Textures/weapon_diffuse.png"));
	pCharacter->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
	pCharacter->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

	pCharacter->SetRigid(false);

	pCharacter->Init(pPhysXEngine);

	pCharacter->Translate(Vector3(0,0,0));

	m_pCharacter = pCharacter;

	m_pLevelObjects.push_back(pCharacter);

	// LOAD NEW LEVELOBJECT
	LevelObject* pLevelObject = new LevelObject();

	pLevelObject->UseNormalMap(true);
	pLevelObject->UseSimplifiedPhysXMesh(false);

	pLevelObject->SetModelPath(_T("Content/Models/as_val.binobj"));
	pLevelObject->SetPhysXModelPath(_T("Content/Models/as_val.nxconcave"));

	pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_color.png"));
	pLevelObject->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
	pLevelObject->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));
	pLevelObject->SetNormalPath(_T("Content/Textures/weapon_normal.png"));

	pLevelObject->SetRigid(true);

	pLevelObject->Init(pPhysXEngine);

	pLevelObject->Translate(Vector3(0,200,0));

	m_pLevelObjects.push_back(pLevelObject);
}

void Level::Tick(const float dTime)
{
    if (CONTROLS->IsKeyPressed(VK_SPACE))
	{
		int r = rand() % 3;
        if (r == 0)
        {
		    // LOAD NEW LEVELOBJECT - WITH NORMAL MAP
			LevelObject* pLevelObject = new LevelObject();

			pLevelObject->UseNormalMap(true);
			pLevelObject->UseSimplifiedPhysXMesh(true);

			pLevelObject->SetModelPath(_T("Content/Models/sphere50.binobj"));
	
			PhysXSphere sphere(50.0f, 1000);
			pLevelObject->SetSimplifiedPhysXMesh(&sphere);

			pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_diffuse.png"));
			pLevelObject->SetNormalPath(_T("Content/Textures/weapon_normal.png"));
			pLevelObject->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
			pLevelObject->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

			pLevelObject->SetRigid(true);

			pLevelObject->Init(m_pPhysXEngine);

			pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

			m_pLevelObjects.push_back(pLevelObject);

			pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
        }
        else if (r == 1)
        {
            // LOAD NEW LEVELOBJECT - WITH NORMAL MAP
			LevelObject* pLevelObject = new LevelObject();

			pLevelObject->UseNormalMap(true);
			pLevelObject->UseSimplifiedPhysXMesh(true);

			pLevelObject->SetModelPath(_T("Content/Models/box50.binobj"));
	
			PhysXBox box(Vector3(50,50,50),1000);
			pLevelObject->SetSimplifiedPhysXMesh(&box);

			pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_diffuse.png"));
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
			LevelObject* pLevelObject = new LevelObject();

			pLevelObject->UseNormalMap(false);

			pLevelObject->SetModelPath(_T("Content/Models/jman.binobj"));
			pLevelObject->SetPhysXModelPath(_T("Content/Models/jman.nxconcave"));

			pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_diffuse.png"));
			pLevelObject->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
			pLevelObject->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

			pLevelObject->SetRigid(true);
			pLevelObject->SetMass(1000);

			pLevelObject->Init(m_pPhysXEngine);

			pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

			m_pLevelObjects.push_back(pLevelObject);

			pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
		}
	}
    if (CONTROLS->IsKeyPressed(VK_CONTROL))
	{
        //if (rand() % 2 == 0)
        //{
            TestSoftbody* pSB = new TestSoftbody(m_pRenderContext->GetCamera()->GetPosition());
            pSB->Init(m_pPhysXEngine);
            m_pLevelObjects.push_back(pSB);
		    pSB->AddSpeed(m_pRenderContext->GetCamera()->GetLook() * 2000);
      //  }
      //  else
      //  {
      //      TestPhysXSphere* pTestPhysXShere = new TestPhysXSphere(m_pPhysXEngine, m_pRenderContext->GetCamera()->GetPosition());
		    //pTestPhysXShere->Init();
		    //m_vecActor.push_back(pTestPhysXShere);
		    //pTestPhysXShere->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
      //  }
	}

	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		if (!m_bTickCharacter)
		{
			if ((*it) != m_pCharacter)
				(*it)->Tick(dTime);
		}
		else
			(*it)->Tick(dTime);
		
	}
}

void Level::DrawDeferred(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	for (vector<LevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		(*it)->Draw(pRenderContext);
	}
}


void Level::DrawShadowMap(RenderContext* pRenderContext, PreShadowEffect* pPreShadowEffect)
{
    for (vector<LevelObject*>::const_iterator lIt = m_pLevelObjects.cbegin(); lIt != m_pLevelObjects.cend(); ++lIt)
	{
		LevelObject* lobj = *lIt;
        lobj->DrawShadow(pRenderContext, pPreShadowEffect);
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