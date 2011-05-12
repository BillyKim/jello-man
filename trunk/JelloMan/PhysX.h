#pragma once

#include "NxPhysics.h"
#pragma warning(disable: 4100)
#include "ControllerManager.h"
#pragma warning(default: 4100)
#include <vector>
#include "D3DUtil.h"

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

private:

	NxPhysicsSDK *m_pPhysicsSDK;
	NxScene *m_pScene;
    MyOutputStream* m_pUserOutputStream;

	NxControllerManager* m_pControllerManager;
	
private:

	PhysX(const PhysX& t);
	PhysX& operator=(const PhysX& t);
};
