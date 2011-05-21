#pragma once

#include "NxPhysics.h"
#pragma warning(disable: 4100)
#include "ControllerManager.h"
#pragma warning(default: 4100)
#include <vector>
#include "D3DUtil.h"

#pragma warning(disable:4005)
#include "boost\thread\thread.hpp"
#pragma warning(default:4005)

class MyOutputStream : public NxUserOutputStream
{
    void reportError (NxErrorCode code, const char *message, const char* /*file*/, int /*line*/);               
    NxAssertResponse reportAssertViolation (const char *message, const char* /*file*/,int /*line*/);              
    void print (const char *message);
};

class PhysX : public NxUserTriggerReport
{
public:

	PhysX();
	virtual ~PhysX( );

	bool Init();
	void Simulate(float dt);
	void FetchResults();
	NxScene *GetScene() const {return m_pScene;}
	NxPhysicsSDK* GetSDK() const {return m_pPhysicsSDK;}

	NxShape *GetClosestShape(NxRay& ray,float distance);

	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);

	boost::mutex& GetPhysXLock() { return m_PhysXLock; }

private:

	NxPhysicsSDK *m_pPhysicsSDK;
	NxScene *m_pScene;
    MyOutputStream* m_pUserOutputStream;

	NxControllerManager* m_pControllerManager;

	boost::mutex m_PhysXLock;
	
private:

	PhysX(const PhysX& t);
	PhysX& operator=(const PhysX& t);
};
