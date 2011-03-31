#include "StdAfx.h"
#include "PhysX.h"
#include "NxCapsuleController.h"

#pragma comment(lib, "PhysXLoader.lib")
//#pragma comment(lib, "NxCharacter.lib")

PhysX::PhysX():m_pPhysicsSDK(0),m_pScene(0), m_pControllerManager(0)
{

}

PhysX::~PhysX(void)
{
	//NxReleaseControllerManager(m_pControllerManager);
	if(m_pScene)m_pPhysicsSDK->releaseScene(*m_pScene);
	if(m_pPhysicsSDK)m_pPhysicsSDK->release();
	if(m_pAllocator) delete m_pAllocator;
}
class MyOutputStream : public NxUserOutputStream
{
    void reportError (NxErrorCode code, const char *message, const char* file, int line)
    {
        //this should be routed to the application
        //specific error handling. If this gets hit
        //then you are in most cases using the SDK
        //wrong and you need to debug your code!
        //however, code may  just be a warning or
        //information.
        
        cout << "--Physx: " << message << "\n";
    }
                
    NxAssertResponse reportAssertViolation (const char *message, const char *file,int line)
    {
        //this should not get hit by
        // a properly debugged SDK!
        ASSERT(0);
        return NX_AR_CONTINUE;
    }
                
    void print (const char *message)
    {
         cout << "SDK says: %s\n" << message << "\n";
    }

} myOutputStream;
bool PhysX::Init(void)
{
	//create PhysicsSDK object
	m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, new MyOutputStream());
	if(!m_pPhysicsSDK)
	{
		OutputDebugString(_T("Wrong SDK DLL version?"));
		return false;
	}
	//(if debug?)
	m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
	m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.1f);
    m_pPhysicsSDK->setParameter(NxParameter::NX_DEFAULT_SLEEP_ENERGY, 50000.0f);
    m_pPhysicsSDK->setParameter(NxParameter::NX_DYN_FRICT_SCALING, 100.0f);
    m_pPhysicsSDK->setParameter(NxParameter::NX_STA_FRICT_SCALING, 100.0f);
	//create a scene object
	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0, -981.0f, 0);
	m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
	if(!m_pScene)
	{
		OutputDebugString(_T("Scene creation failed"));
		return false;
	}
	// Set default material
	NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.3f);
	defaultMaterial->setStaticFriction(.6f);
	defaultMaterial->setDynamicFriction(.2f);

	// Create ground plane
	NxPlaneShapeDesc planeDesc;
	planeDesc.d=-5;//set plane below origine
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	NxActor* plane = m_pScene->createActor(actorDesc);

	NxReal myTimestep = 1/60.0f;
	m_pScene->setTiming(myTimestep, 8, NX_TIMESTEP_FIXED);//4 substeps
	
	m_pScene->setUserTriggerReport(this);

	m_pAllocator = new DAEAllocator();
	//m_pControllerManager = NxCreateControllerManager(m_pAllocator);
	
	return true;
}
//fixed is best
void PhysX::Simulate(float dt)
{
	NxReal myTimestep = dt;//1/60.0f;
	m_pScene->simulate(myTimestep);//thread start met rekenen, er is een soort back en frontbuffer
}
void PhysX::FetchResults(void)
{
	//Flushes any buffered commands so that they get executed. 
	//Ensures that commands buffered in the system will continue to make forward progress until completion
	m_pScene->flushStream();

	//Wait until physx calcs are done, then proceed
	m_pScene->fetchResults(NX_ALL_FINISHED ,true);
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
