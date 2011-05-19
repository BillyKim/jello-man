#include "StdAfx.h"
#include "PhysX.h"
#pragma warning(disable: 4512)
#include "NxCapsuleController.h"
#pragma warning(default: 4512)

#pragma comment(lib, "PhysXLoader.lib")
//#pragma comment(lib, "NxCharacter.lib")

#pragma region MyOutputStream
void MyOutputStream::reportError(NxErrorCode code, const char *message, const char*, int)
{
  cout << "--Physx: " << code << ": " << message << "\n";
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
    cout << "SDK says: %s\n" << message << "\n";
}
#pragma endregion

PhysX::PhysX()
	:	m_pPhysicsSDK(0),
		m_pScene(0),
		m_pControllerManager(0),
		m_pUserOutputStream(0)
{

}

PhysX::~PhysX(void)
{
	//NxReleaseControllerManager(m_pControllerManager);
	if(m_pScene)m_pPhysicsSDK->releaseScene(*m_pScene);
	if(m_pPhysicsSDK)m_pPhysicsSDK->release();

    delete m_pUserOutputStream;
}

bool PhysX::Init(void)
{
    m_pUserOutputStream = new MyOutputStream();
	m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, m_pUserOutputStream);

	ASSERT(m_pPhysicsSDK != 0, "Error creating PhysicsSDK");

	m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
	//m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.1f);
    //m_pPhysicsSDK->setParameter(NX_DEFAULT_SLEEP_ENERGY, 50000.0f);
   /* m_pPhysicsSDK->setParameter(NX_DYN_FRICT_SCALING, 100.0f);
    m_pPhysicsSDK->setParameter(NX_STA_FRICT_SCALING, 100.0f);*/

	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0, -9.81f, 0);

	m_pScene = m_pPhysicsSDK->createScene(sceneDesc);

	ASSERT(m_pScene != 0, "Error creating physics scene");

	NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.3f);
	defaultMaterial->setStaticFriction(.6f);
	defaultMaterial->setDynamicFriction(.5f);

	NxPlaneShapeDesc planeDesc;
	planeDesc.d = 0;

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);

	m_pScene->createActor(actorDesc);

	NxReal myTimestep = 1/60.0f;
	m_pScene->setTiming(myTimestep, 8, NX_TIMESTEP_FIXED);
	
	m_pScene->setUserTriggerReport(this);

	//m_pControllerManager = NxCreateControllerManager(m_pAllocator);
	
	return true;
}

void PhysX::Simulate(float dt)
{
	NxReal myTimestep = dt;
	m_pScene->simulate(myTimestep);
}

void PhysX::FetchResults(void)
{
	m_pScene->flushStream();

	m_pScene->fetchResults(NX_ALL_FINISHED, true);
}

//NxController* PhysX::CreateController(Character* player)
//{
//	NxCapsuleControllerDesc desc;
//	//desc.radius = player->GetRadius();
//	//desc.height = player->GetHeight();
//	//D3DXVECTOR3 pos = player->GetPosition();
//	//desc.position = NxExtendedVec3( pos.x, pos.y, pos.z);
//	desc.climbingMode = CLIMB_EASY ;
//	desc.upDirection = NX_Y;
//
//	NxController* c = m_pControllerManager->createController(m_pScene, desc);
//
//	c->setCollision(true);
//	c->getActor()->setGroup(1);
//	c->getActor()->userData=player;
//	c->getActor()->setName("character");
//
//	m_Controllers.push_back(c);
//	return c;
//}

NxShape *PhysX::GetClosestShape(NxRay& ray,float distance)
{
	NxRaycastHit hit;
	NxShape* result = m_pScene->raycastClosestShape(ray,NX_STATIC_SHAPES,hit,1,distance);
	return result;
}

void PhysX::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{
	NxActor& triggerActor = triggerShape.getActor();
	NxActor& otherActor = otherShape.getActor();
	if (triggerActor.userData == NULL || otherActor.userData == NULL)
		return;
	//Character* player = (Character*)otherActor.userData;
	//LevelElement* object = (LevelElement*)triggerActor.userData;
	
	//if(status & NX_TRIGGER_ON_ENTER)
	//{	
	//	object->OnTriggerEnter(player);
	//}
	//else if(status & NX_TRIGGER_ON_LEAVE)
	//{
	//	object->OnTriggerExit(player);
	//}
}
