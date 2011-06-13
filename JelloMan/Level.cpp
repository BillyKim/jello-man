#include "Level.h"
#include "ContentManager.h"
#include "PhysXSphere.h"
#include "PhysXBox.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Particle.h"
#include "ParticleFactory.h"
#include "SimpleObject.h"
#include "IInstancible.h"
#include "Editor.h"
#include "SpawnPoint.h"
#include "CharacterController.h"
#include "LightBehaviourBroken.h"
#include "Terrain.h"
#include "Coin.h"
#include "SimpleSoftbody.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
				m_pRenderContext(0),
				m_pBaseGrid(new BaseGrid()),
				m_bShowGrid(false),
				m_bTickCharacter(false),
				m_bEditor(true),
                m_pGUI(0),
                m_pFluidRenderer(0),
                m_pCharacterController(0)
{

}


Level::~Level()
{
	Clear();
	delete m_pBaseGrid;
    delete m_pFluidRenderer;
    delete m_pInstancingManager;
    delete m_pCharacterController;
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, Editor* pGUI, Graphics::Camera::FollowCamera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;
	
	m_pBaseGrid->Init();

    m_pGUI = pGUI;

    m_pFluidRenderer = new FluidRenderer(m_pDXDevice, 
                                            static_cast<int>(BX2D->GetWindowSize().width), 
                                            static_cast<int>(BX2D->GetWindowSize().height));

    m_pInstancingManager = new Instancing::InstancingManager(m_pDXDevice);

    m_pCharacterController = new CharacterController(this);
    m_pCharacterController->Init(m_pDXDevice, m_pPhysXEngine, pTrackingCamera);

    SpawnPoint* pSpawnpoint = new SpawnPoint();
    pSpawnpoint->Init(m_pPhysXEngine);
    AddLevelObject(pSpawnpoint);
    m_pCharacterController->SetSpawnPoint(pSpawnpoint);

    Terrain* pTerrain = new Terrain();
    pTerrain->SetDiffusePath(_T("../Content/Textures/heightmap_tex.png"));
    pTerrain->SetTerrainPath(_T("../Content/Textures/heightmap.png"));
    pTerrain->Init(m_pPhysXEngine);
    AddLevelObject(pTerrain);

    Coin* pCoin = new Coin();
    pCoin->Init(pPhysXEngine);
    AddLevelObject(pCoin);
    pCoin->Translate(Vector3(5, 0.5f, 0.0f));
    //for (int x = 0; x < 20; ++x)
    //    for (int y = 0; y < 20; ++y)
    //        for (int z = 0; z < 20; ++z)
    //        {
    //            SimpleObject* pTemp = new SimpleObject(true);
    //            pTemp->SetModelPath(_T("../Content/Models/box1.binobj"));
    //            pTemp->SetPhysXModel(new PhysXBox(Vector3(1.0f, 1.0f, 1.0f), 1));
    //            pTemp->SetRigid(false);
    //            pTemp->Init(pPhysXEngine);
    //            pTemp->Translate(Vector3(x*2, y*2, z*2));

    //            AddLevelObject(pTemp);
    //        }

}
void Level::WakeUpAll()
{
    for_each(m_pLevelObjects.begin(), m_pLevelObjects.end(), [&](ILevelObject* obj)
	{
        Actor* ac = dynamic_cast<Actor*>(obj);
        if (ac != 0)
            ac->WakeUp();
	});
}

void Level::Tick(const float dTime)
{
    while (m_RemoveQueue.size() > 0)
    {
        RemoveLevelObject(m_RemoveQueue.front());
        m_RemoveQueue.pop_front();
    }

    for_each(m_pLevelObjects.begin(), m_pLevelObjects.end(), [&](ILevelObject* obj)
	{
		obj->Tick(dTime);
	});

    
    m_pCharacterController->Tick(dTime);

    //if (CONTROLS->IsKeyPressed('R'))
    //{
    //    SpawnPoint* pSpawnpoint = new SpawnPoint(Vector3::Zero);
    //    pSpawnpoint->Init(m_pPhysXEngine);
    //    AddLevelObject(pSpawnpoint);
    //    m_pCharacterController->SetSpawnPoint(pSpawnpoint);
    //}
    if (CONTROLS->IsKeyPressed('C'))
    {
        Coin* pCoin = new Coin();
        pCoin->Init(m_pPhysXEngine);
        AddLevelObject(pCoin);
    }
}

void Level::AddLevelObject(ILevelObject* pLevelObject)
{
    if (pLevelObject == 0)
    {
        PANIC("adding a NULL levelobject --> ignored");
        return;
    }

	m_pLevelObjects.push_back(pLevelObject);

    if (pLevelObject->GetType() == LevelObjectType_InstancedDraw)
    {
        using namespace Instancing;
	    IInstancible* instObj = dynamic_cast<IInstancible*>(pLevelObject);
        ASSERT(instObj != 0, "trying to add an instanced object which isn't an instancible object");
		m_pInstancingManager->AddObject(instObj);
    }
    else if (pLevelObject->GetType() == LevelObjectType_NormalDraw)
    {
        IDrawable* drawObj = dynamic_cast<IDrawable*>(pLevelObject);
		ASSERT(drawObj != 0, "trying to add a drawable object which can't be drawn");
		m_pDrawableObjects.push_back(drawObj);
    }
    else if (pLevelObject->GetType() == LevelObjectType_Spawnpoint)
    {
        SpawnPoint* pSpawnpoint = dynamic_cast<SpawnPoint*>(pLevelObject);
	    ASSERT(pSpawnpoint != 0, "trying to add a spawnpoint object which isn't a spawnpoint");
        m_pCharacterController->SetSpawnPoint(pSpawnpoint);
        m_pDrawableObjects.push_back(pSpawnpoint);
    }
    else if (pLevelObject->GetType() == LevelObjectType_Trigger)
    {
        Trigger* pTrigger = dynamic_cast<Trigger*>(pLevelObject);
	    ASSERT(pTrigger != 0, "trying to add a trigger object which isn't a trigger");
		tstring triggerName = pTrigger->GetTriggerName();
		m_pTriggers[triggerName] = pTrigger;
    }
    else if (pLevelObject->GetType() == LevelObjectType_NoDraw)
    {
    }
    else
    {
        ASSERT(false, "Unkown LevelObjectType");
    }

}

void Level::RemoveLevelObjectWaitTick(const ILevelObject* pLevelObject)
{
    m_RemoveQueue.push_back(pLevelObject);
}
void Level::RemoveLevelObject(const ILevelObject* pLevelObject)
{
    if (pLevelObject == 0)
    {
        PANIC("removing a NULL levelobject --> ignored");
        return;
    }

    if (pLevelObject->GetType() == LevelObjectType_InstancedDraw)
    {
        using namespace Instancing;
	    const IInstancible* instObj = dynamic_cast<const IInstancible*>(pLevelObject);
        ASSERT(instObj != 0, "trying to delete an instanced object which isn't an instancible object");
		m_pInstancingManager->DeleteObject(instObj);
    }
    else if (pLevelObject->GetType() == LevelObjectType_NormalDraw)
    {
        const IDrawable* drawObj = dynamic_cast<const IDrawable*>(pLevelObject);
		ASSERT(drawObj != 0, "trying to delete a drawable object which can't be drawn");
		m_pDrawableObjects.erase(remove(m_pDrawableObjects.begin(), m_pDrawableObjects.end(), drawObj));
    }
    else if (pLevelObject->GetType() == LevelObjectType_Spawnpoint)
    {
        const SpawnPoint* pSpawnpoint = dynamic_cast<const SpawnPoint*>(pLevelObject);
	    ASSERT(pSpawnpoint != 0, "trying to delete a spawnpoint object which isn't a spawnpoint");
        m_pCharacterController->SetSpawnPoint(0);
		m_pDrawableObjects.erase(remove(m_pDrawableObjects.begin(), m_pDrawableObjects.end(), pSpawnpoint));
    }
    else if (pLevelObject->GetType() == LevelObjectType_Trigger)
    {
        const Trigger* pTrigger = dynamic_cast<const Trigger*>(pLevelObject);
	    ASSERT(pTrigger != 0, "trying to delete a trigger object which isn't a trigger");
        m_pTriggers.erase(pTrigger->GetTriggerName());
    }
    else if (pLevelObject->GetType() == LevelObjectType_NoDraw)
    {
    }
    else
    {
        ASSERT(false, "Unkown LevelObjectType");
    }

	m_pLevelObjects.erase(remove(m_pLevelObjects.begin(), m_pLevelObjects.end(), pLevelObject));
	delete pLevelObject;
}

void Level::AddLight(Light* pLight)
{
    m_pRenderContext->GetLightController()->AddLight(pLight);
}
void Level::RemoveLight(Light* pLight)
{
    m_pRenderContext->GetLightController()->DeleteLight(pLight);
}

// SERIALISATION
ISerializable* GetObject(DWORD id)
{
    switch (id)
    {
        case SerializeTypes::SpotLight: return new SpotLight();
        case SerializeTypes::PointLight: return new PointLight();
        case SerializeTypes::SimpleObject: return new SimpleObject(true);
        case SerializeTypes::PhysXTrigger: return new Trigger();
        case SerializeTypes::SpawnPoint: return new SpawnPoint();
        case SerializeTypes::SimpleSoftbody: return new SimpleSoftbody();
        case SerializeTypes::Coin: return new Coin();
        default: PANIC("File corrupt!"); return 0;
    }
}
void Level::Serialize(const string& path) const 
{
    Serializer s(path, m_pPhysXEngine);

    s.Begin(false);
    for_each(m_pLevelObjects.cbegin(), m_pLevelObjects.cend(), [&](ILevelObject* obj)
    {
        ISerializable* is = dynamic_cast<ISerializable*>(obj);
        if (is != 0)
            s.Serialize(is);
    });
    for_each(m_pRenderContext->GetLightController()->GetLights().cbegin(), m_pRenderContext->GetLightController()->GetLights().cend(), [&](Light* light)
    {
        ISerializable* is = dynamic_cast<ISerializable*>(light);
        if (is != 0)
            s.Serialize(is);
    });
    s.End();
}
void Level::Deserialize(const string& path)
{
    Serializer s(path, m_pPhysXEngine);

    s.Begin(true);
    
    Clear();
    while (s.eof() == false)
    {
        ISerializable* obj = s.Deserialize(GetObject);
        
        //Test if lvlObj
        ILevelObject* lvlObj = dynamic_cast<ILevelObject*>(obj);
        if (lvlObj != 0)
        {
            AddLevelObject(lvlObj);
        }
        else
        {
            Light* l = dynamic_cast<Light*>(obj);
            if (l != 0)
            {
				/*int r = rand() % 10;
				if (r < 4)
					l->SetBehaviour(new LightBehaviourBroken(BrokenLightType_LightBulb));*/
                m_pRenderContext->GetLightController()->AddLight(l);              
            }
        }
    }
    s.End();
}
void Level::Clear()
{   
    m_pGUI->Clear();
    m_pRenderContext->GetLightController()->Clear();
    for_each(m_pLevelObjects.cbegin(), m_pLevelObjects.cend(), [](ILevelObject* obj)
    {
		delete obj;
    });
    m_pTriggers.clear();
    m_pLevelObjects.clear();
    m_pDrawableObjects.clear();
    m_pInstancingManager->Clear();
}

// DRAW
void Level::DrawDeferred(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	for_each(m_pDrawableObjects.cbegin(), m_pDrawableObjects.cend(), [&](IDrawable* obj)
	{
		obj->Draw(pRenderContext);
	});

    m_pInstancingManager->Draw(pRenderContext);

    m_pCharacterController->DrawDeferred(m_pRenderContext);
}

void Level::DrawShadowMap(const RenderContext* pRenderContext)
{
    for_each(m_pDrawableObjects.cbegin(), m_pDrawableObjects.cend(), [&](IDrawable* obj)
	{
        obj->DrawShadow(pRenderContext);
	});

    m_pInstancingManager->DrawShadow(pRenderContext);
}

void Level::DrawForward(const RenderContext* pRenderContext)
{
    m_pCharacterController->DrawForward(m_pRenderContext);

	if (m_bShowGrid)
		m_pBaseGrid->Draw(pRenderContext);

	// DRAW TRIGGERS
	if (m_pGUI->GetEditorMode() != Editor::EDITOR_MODE_PLAY)
	{
		for_each(GetTriggers().cbegin(), GetTriggers().cend(), [&](pair<tstring, Trigger*> trigger)
		{
			trigger.second->Draw(pRenderContext);
		});
	}
 
    m_pFluidRenderer->Begin();
    m_pCharacterController->DrawFluids(m_pRenderContext);
    m_pFluidRenderer->End(pRenderContext);

	// DRAW LIGHTS
	if (m_bEditor)
	{
		BX2D->SetAntiAliasing(true);

        for_each(m_pRenderContext->GetLightController()->GetLights().cbegin(),
                 m_pRenderContext->GetLightController()->GetLights().cend(), [&](Light* l)
		{
			l->Draw(m_pRenderContext);
		});
        
		BX2D->SetAntiAliasing(false);
	}
}

bool Level::Triggered(const tstring& triggerName)
{
	ASSERT(m_pTriggers.find(triggerName) != m_pTriggers.end(), "Trigger not present in the level!");

	return m_pTriggers[triggerName]->Triggered();
}