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
				m_pCharacter(0),
				m_bEditor(true),
				m_pTestFluid(0),
				m_pEmitter(0),
                m_pFluidPostProcessor(0),
				m_pFluidsCharacter(0)
{

}


Level::~Level()
{
	delete m_pBaseGrid;
	delete m_pTestFluid;
	delete m_pCharacter;
    delete m_pFluidPostProcessor;
	delete m_pFluidsCharacter;
	
	Clear();
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, Graphics::Camera::FollowCamera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;
	
	m_pBaseGrid->Init();

	// TEST FLUID
	int MAX_PARTICLES = 10000;

	// setup fluid descriptor
	NxFluidDesc fluidDesc;
    fluidDesc.maxParticles                  = MAX_PARTICLES;
    fluidDesc.kernelRadiusMultiplier		= 2.0f;
    fluidDesc.restParticlesPerMeter			= 1.0f;
	fluidDesc.motionLimitMultiplier			= 2.0f;
	fluidDesc.packetSizeMultiplier			= 8;
    fluidDesc.collisionDistanceMultiplier   = 0.5f;
    fluidDesc.stiffness						= 5.0f;
    fluidDesc.viscosity						= 5.0f;
	fluidDesc.restDensity					= 5.0f;
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
	//emitterDesc.relPose.M.rotX(NxHalfPiF32);
	emitterDesc.relPose.t = NxVec3(100.0f,200.0f,0);
	
	m_pTestFluid = new Fluid(pPhysXEngine->GetScene(), fluidDesc, Color(1.0f,0.0f,1.0f,1.0f), 3.0f, m_pDXDevice);
	ASSERT(m_pTestFluid, "fluid creation failed");

	m_pEmitter = m_pTestFluid->GetNxFluid()->createEmitter(emitterDesc);

	m_pCharacter = new SoftbodyCharacter(Vector3(0, 0, 500), pTrackingCamera);
	m_pCharacter->Init(m_pPhysXEngine);
    pTrackingCamera->SetFollowObject(m_pCharacter);
    pTrackingCamera->SetFollowDistance(800);
    pTrackingCamera->SetFollowAngle(20);

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

    m_pFluidPostProcessor = new PostProcessor(m_pDXDevice, 
                                            static_cast<int>(BX2D->GetWindowSize().width), 
                                            static_cast<int>(BX2D->GetWindowSize().height));
    m_pFluidPostProcessor->SetEffect(Content->LoadEffect<FluidPostEffect>(_T("../Content/Effects/fluidPostEffect.fx")));

	m_pFluidsCharacter = new FluidsCharacter();
	m_pFluidsCharacter->Init(m_pDXDevice, m_pPhysXEngine, 1000);
}

void Level::Tick(const float dTime)
{
    for_each(m_pLevelObjects.begin(), m_pLevelObjects.end(), [&](ILevelObject* obj)
	{
		obj->Tick(dTime);	
	});
	m_pCharacter->Tick(dTime);

	m_pFluidsCharacter->Tick(dTime);
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
	m_pCharacter->Draw(pRenderContext);
}

void Level::DrawShadowMap(RenderContext* pRenderContext, PreShadowEffect* pPreShadowEffect)
{
    for_each(m_pLevelObjects.cbegin(), m_pLevelObjects.cend(), [&](ILevelObject* lobj)
	{
        lobj->DrawShadow(pRenderContext, pPreShadowEffect);
	});
	m_pCharacter->DrawShadow(pRenderContext, pPreShadowEffect);
}

void Level::DrawForward(const RenderContext* pRenderContext)
{

	if (m_pTestFluid)
    {
        m_pFluidPostProcessor->Begin();
		//m_pTestFluid->Draw(pRenderContext);
		m_pFluidsCharacter->Draw(pRenderContext);
        m_pFluidPostProcessor->End(pRenderContext->GetDeferredRenderer());
        //m_pFluidRenderer->End();
    }

	if (m_bShowGrid)
		m_pBaseGrid->Draw(pRenderContext);
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