#include "Level.h"
#include "ContentManager.h"
#include "PhysXSphere.h"
#include "PhysXBox.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Particle.h"
#include "ParticleFactory.h"
#include "SimpleObject.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
				m_pRenderContext(0),
				m_pBaseGrid(new BaseGrid(pDXDevice)),
				m_bShowGrid(false),
				m_bTickCharacter(false),
				//m_pCharacter(0),
				m_bEditor(true),
				m_pTestFluid(0),
				m_pEmitter(0)
{

}


Level::~Level()
{
	delete m_pBaseGrid;
	delete m_pTestFluid;
	
	Clear();
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, Camera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;
	
	m_pBaseGrid->Init();

	// TEST FLUID
	int MAX_PARTICLES = 1000;

	// setup fluid descriptor
	NxFluidDesc fluidDesc;
    fluidDesc.maxParticles                  = MAX_PARTICLES;
    fluidDesc.kernelRadiusMultiplier		= 30.0f;
    fluidDesc.restParticlesPerMeter			= 7.0f;
	fluidDesc.motionLimitMultiplier			= 30.0f;
	fluidDesc.packetSizeMultiplier			= 8;
    fluidDesc.collisionDistanceMultiplier   = 0.1f;
    fluidDesc.stiffness						= 50.0f;
    fluidDesc.viscosity						= 50.0f;
	fluidDesc.restDensity					= 10.0f;
    fluidDesc.damping						= 0.0f;
    fluidDesc.restitutionForStaticShapes	= 0.1f;
	fluidDesc.dynamicFrictionForStaticShapes= 0.8f;
	fluidDesc.simulationMethod				= NX_F_SPH;
	fluidDesc.flags &= ~NX_FF_HARDWARE;

	// create an attached emitter
	NxFluidEmitterDesc emitterDesc;

	emitterDesc.maxParticles = 0; // using fluid max particles

	emitterDesc.dimensionX = 30.f;
	emitterDesc.dimensionY = 30.f;

	emitterDesc.type = NX_FE_CONSTANT_FLOW_RATE;

	emitterDesc.rate = 300.0f;

	emitterDesc.fluidVelocityMagnitude = 100.0f;

	emitterDesc.shape = NX_FE_RECTANGULAR;
	emitterDesc.particleLifetime = 10.0f; // in seconds

	////attach to actor
	//emitterDesc.flags |= NX_FEF_ADD_BODY_VELOCITY;
	//emitterDesc.repulsionCoefficient = 300.0f;

	emitterDesc.relPose.M.id();
	emitterDesc.relPose.M.rotX(-NxHalfPiF32);
	emitterDesc.relPose.t = NxVec3(100.0f,200.0f,0);
	
	m_pTestFluid = new Fluid(pPhysXEngine->GetScene(), fluidDesc, Color(1.0f,0.0f,1.0f,1.0f), 3.0f, m_pDXDevice);
	ASSERT(m_pTestFluid, "fluid creation failed");

	m_pEmitter = m_pTestFluid->GetNxFluid()->createEmitter(emitterDesc);
}

void Level::Tick(const float dTime)
{
    for_each(m_pLevelObjects.begin(), m_pLevelObjects.end(), [&](ILevelObject* obj)
	{
		obj->Tick(dTime);	
	});
}

void Level::AddLevelObject(ILevelObject* pLevelObject)
{
	m_pLevelObjects.push_back(pLevelObject);
}

// SERIALISATION
ISerializable* GetObject(DWORD id)
{
    switch (id)
    {
        case SerializeTypes::SpotLight: return new SpotLight();
        case SerializeTypes::PointLight: return new PointLight();
        case SerializeTypes::SimpleObject: return new SimpleObject();
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
            m_pLevelObjects.push_back(lvlObj);
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
    m_pRenderContext->GetLightController()->Clear();
    for_each(m_pLevelObjects.cbegin(), m_pLevelObjects.cend(), [](ILevelObject* obj)
    {
		delete obj;
    });
    m_pLevelObjects.clear();
}

// DRAW
void Level::DrawDeferred(RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	for (vector<ILevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		(*it)->Draw(pRenderContext);
	}
}

void Level::DrawShadowMap(RenderContext* pRenderContext, PreShadowEffect* pPreShadowEffect)
{
    for_each(m_pLevelObjects.cbegin(), m_pLevelObjects.cend(), [&](ILevelObject* lobj)
	{
        lobj->DrawShadow(pRenderContext, pPreShadowEffect);
	});
}

void Level::DrawForward(const RenderContext* pRenderContext)
{
	if (m_bShowGrid)
		m_pBaseGrid->Draw(pRenderContext);

	if (m_pTestFluid)
		m_pTestFluid->Draw(pRenderContext);

	/*tstringstream stream;
	stream << m_pEmitter->getNbParticlesEmitted();

	BX2D->DrawString(stream.str(), 300,200);*/

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