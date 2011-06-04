#include "StdAfx.h"
#include "PhysX.h"
#pragma warning(disable: 4512)
#include "NxCapsuleController.h"
#pragma warning(default: 4512)

#pragma comment(lib, "PhysXLoader.lib")
#pragma comment(lib, "NxCharacter.lib")

#include "Trigger.h"

#pragma region MyOutputStream
void MyOutputStream::reportError(NxErrorCode code, const char *message, const char*, int)
{
    #if _DEBUG
    if (code != 206) //disable warning 206: static actor moved
        cout << "--Physx: " << code << ": " << message << "\n";
    #endif
}              
NxAssertResponse MyOutputStream::reportAssertViolation (const char *message, const char*, int)
{
    //this should not get hit by
    // a properly debugged SDK!
    ASSERT(false, message);
    return NX_AR_CONTINUE;
}               
void MyOutputStream::print (const char *message)
{
    #if _DEBUG
    cout << "PhysX SDK says: \n" << message << "\n";
    #endif
}
#pragma endregion

PhysX::PhysX()
	:	m_pPhysicsSDK(0),
		m_pScene(0),
		m_pControllerManager(0),
		m_pUserOutputStream(0),
        m_pDefaultAllocator(0)
{
}

PhysX::~PhysX(void)
{
	NxReleaseControllerManager(m_pControllerManager);
	if(m_pScene)m_pPhysicsSDK->releaseScene(*m_pScene);
	if(m_pPhysicsSDK)m_pPhysicsSDK->release();

    delete m_pUserOutputStream;
    delete m_pDefaultAllocator;
}

bool PhysX::Init(void)
{
    m_pUserOutputStream = new MyOutputStream();
	m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, m_pUserOutputStream);

	ASSERT(m_pPhysicsSDK != 0, "Error creating PhysicsSDK");

    #if _DEBUG
	m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect("localhost");
    #endif

	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0, -9.81f, 0);

	m_pScene = m_pPhysicsSDK->createScene(sceneDesc);

	ASSERT(m_pScene != 0, "Error creating physics scene");

    #pragma region default material
	NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.3f);
	defaultMaterial->setStaticFriction(.6f);
	defaultMaterial->setDynamicFriction(.5f);
    #pragma endregion

    #pragma region groundplane
	NxPlaneShapeDesc planeDesc;
	planeDesc.d = 0;

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);

	m_pScene->createActor(actorDesc);
    #pragma endregion

	NxReal myTimestep = 1/60.0f;
	m_pScene->setTiming(myTimestep, 8, NX_TIMESTEP_FIXED);
	
	m_pScene->setUserTriggerReport(this);

    m_pDefaultAllocator = new NxUserAllocatorDefault();
    m_pControllerManager = NxCreateControllerManager(m_pDefaultAllocator);

	return true;
}

void PhysX::Simulate(float dt)
{
	NxReal myTimestep = dt;
	m_pScene->simulate(myTimestep);
}

void PhysX::FetchResults(void)
{
    m_pControllerManager->updateControllers();
	m_pScene->flushStream();
	m_pScene->fetchResults(NX_ALL_FINISHED, true);
}

NxShape *PhysX::GetClosestShape(NxRay& ray,float distance)
{
	NxRaycastHit hit;
	NxShape* result = m_pScene->raycastClosestShape(ray, NX_STATIC_SHAPES, hit, 1, distance);
	return result;
}

void PhysX::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{
    //PANIC("Not Implemented");

	NxActor& triggerActor = triggerShape.getActor();

	//NxActor& otherActor = otherShape.getActor();

	if (triggerActor.userData == NULL) // || otherActor.userData == NULL)
		return;

	Trigger* pTrigger = (Trigger*)triggerActor.userData;

	//Character* player = (Character*)otherActor.userData;
	//LevelElement* object = (LevelElement*)triggerActor.userData;
	
	if(status & NX_TRIGGER_ON_ENTER)
	{	
		pTrigger->OnTrigger();
		//object->OnTriggerEnter(player);
	}
	/*else if(status & NX_TRIGGER_ON_LEAVE)
	{
		object->OnTriggerExit(player);
	}*/
}
