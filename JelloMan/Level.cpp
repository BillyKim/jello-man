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
#include "EditorGUI.h"
#include "Trigger.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
				m_pRenderContext(0),
				m_pBaseGrid(new BaseGrid(pDXDevice)),
				m_bShowGrid(false),
				m_bTickCharacter(false),
				m_pCharacter(0),
				m_bEditor(true),
				m_pFluidsCharacter(0),
                m_pGUI(0),
                m_pFluidRenderer(0)
{

}


Level::~Level()
{
	Clear();
	delete m_pBaseGrid;
	delete m_pCharacter;
    delete m_pFluidRenderer;
	delete m_pFluidsCharacter;
    delete m_pInstancingManager;
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, EditorGUI* pGUI, Graphics::Camera::FollowCamera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;
	
	m_pBaseGrid->Init();

    m_pGUI = pGUI;

	/*m_pCharacter = new SoftbodyCharacter(Vector3(0, 0, 500), pTrackingCamera);
	m_pCharacter->Init(m_pPhysXEngine);*/
    
	m_pFluidsCharacter = new FluidsCharacter(this);
	m_pFluidsCharacter->Init(m_pDXDevice, m_pPhysXEngine, pTrackingCamera, 1000, Vector3(-18, 3, 0));

	// forcefield
	/*NxForceFieldLinearKernelDesc linearKernelDesc;
    linearKernelDesc.constant = NxVec3(-1000,-500,500);
    
    NxForceFieldLinearKernel* pLinearKernel;
    pLinearKernel = pPhysXEngine->GetScene()->createForceFieldLinearKernel(linearKernelDesc);

    NxForceFieldDesc fieldDesc;
	fieldDesc.coordinates = NX_FFC_SPHERICAL;
    fieldDesc.kernel = pLinearKernel;

    NxForceField *pForceField; 
    pForceField = pPhysXEngine->GetScene()->createForceField(fieldDesc);

	NxForceFieldShape* s = NULL;
	NxBoxForceFieldShapeDesc b;
	b.dimensions = NxVec3(500, 700, 500);
	b.pose.t = NxVec3(0, 350, 0);
	s = pForceField->getIncludeShapeGroup().createShape(b);*/

    m_pFluidRenderer = new FluidRenderer(m_pDXDevice, 
                                            static_cast<int>(BX2D->GetWindowSize().width), 
                                            static_cast<int>(BX2D->GetWindowSize().height));

    m_pInstancingManager = new Instancing::InstancingManager(m_pDXDevice);

	//test

	Trigger* pTrigger = new Trigger();
	pTrigger->Init(m_pPhysXEngine, Vector3(5,5,5));
	pTrigger->Translate(Vector3(4,10,5));

	AddLevelObject(pTrigger);
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
    for_each(m_pLevelObjects.begin(), m_pLevelObjects.end(), [&](ILevelObject* obj)
	{
		obj->Tick(dTime);	
	});

	//m_pCharacter->Tick(dTime);

	//if (m_bTickCharacter)
		m_pFluidsCharacter->Tick(dTime);
}

void Level::AddLevelObject(ILevelObject* pLevelObject)
{
	m_pLevelObjects.push_back(pLevelObject);

	using namespace Instancing;
	IInstancible* instObj = dynamic_cast<IInstancible*>(pLevelObject);
	if (instObj != 0 && instObj->IsUsedForInstancing() == true)
	{
		m_pInstancingManager->AddObject(instObj);
	}
	else
	{
		IDrawable* drawObj = dynamic_cast<IDrawable*>(pLevelObject);
		if (drawObj != 0) //we allow objects that don't need to be drawn
			m_pDrawableObjects.push_back(drawObj);
	}
}
void Level::RemoveLevelObject(ILevelObject* pLevelObject)
{
    using namespace Instancing;
    IInstancible* instObj = dynamic_cast<IInstancible*>(pLevelObject);
    if (instObj != 0)
    {
        m_pInstancingManager->DeleteObject(instObj);
    }
    else
    {
        IDrawable* drawObj = dynamic_cast<IDrawable*>(pLevelObject);
        ASSERT(drawObj != 0, "What are you?");
        m_pDrawableObjects.erase(remove(m_pDrawableObjects.begin(), m_pDrawableObjects.end(), drawObj));
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
        default: PANIC("File corrupt!"); return 0;
    }
}
void Level::Serialize(const string& path)
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
        
        //Test is lvlObj
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
    m_pLevelObjects.clear();
    m_pDrawableObjects.clear();
    m_pInstancingManager->Clear();
}

// DRAW
void Level::DrawDeferred(RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	for_each(m_pDrawableObjects.cbegin(), m_pDrawableObjects.cend(), [&](IDrawable* obj)
	{
		obj->Draw(pRenderContext);
	});

    m_pInstancingManager->Draw(pRenderContext);

	//m_pCharacter->Draw(pRenderContext);
}

void Level::DrawShadowMap(RenderContext* pRenderContext)
{
    for_each(m_pDrawableObjects.cbegin(), m_pDrawableObjects.cend(), [&](IDrawable* obj)
	{
        obj->DrawShadow(pRenderContext);
	});

    m_pInstancingManager->DrawShadow(pRenderContext);
}

void Level::DrawForward(RenderContext* pRenderContext)
{
	if (m_pFluidsCharacter)
    {
        m_pFluidRenderer->Begin();

		m_pFluidsCharacter->Draw(pRenderContext);

        m_pFluidRenderer->End(pRenderContext);
    }

	if (m_bShowGrid)
		m_pBaseGrid->Draw(pRenderContext);

	// DRAW LIGHTS
	if (m_bEditor)
	{
		BX2D->SetAntiAliasing(true);

		for (unsigned int i = 0; i < pRenderContext->GetLightController()->GetLights().size(); ++i)
		{
			m_pRenderContext->GetLightController()->GetLights()[i]->Draw(m_pRenderContext);
		}
        
		BX2D->SetAntiAliasing(false);
	}
}