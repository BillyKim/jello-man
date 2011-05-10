#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
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
	PhysX();//constructor
	virtual ~PhysX( );

	bool Init(void);
	void Simulate(float dt);
	void FetchResults(void);
	NxScene *GetScene() const {return m_pScene;}
	NxPhysicsSDK* GetSDK() const {return m_pPhysicsSDK;}

	/**
	* Gets the closest shape 
	* @param ray the ray to check.
	* @param distance the maximum distance for the shape.
	* @return the found shape
	*/
	NxShape *GetClosestShape(NxRay& ray,float distance);

	/**
	* Creates a new character controller at the specified location
	* @param player the player character.
	*/
	//NxController *CreateController(Character* player);
	/**
	* Implementation of the NxUserTriggerReport class.
	* Called when a shape is triggered.
	* @param triggerShape the shape of the trigger.
	* @param otherShape the shape that triggered the trigger.
	* @param status the kind of trigger event.
	*/
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);

private:

	NxPhysicsSDK *m_pPhysicsSDK;
	NxScene *m_pScene;
    MyOutputStream* m_pUserOutputStream;

	//std::vector<NxController*> m_Controllers;
	NxControllerManager* m_pControllerManager;
	//DAEAllocator *m_pAllocator;
private:
	PhysX(const PhysX& t);
	PhysX& operator=(const PhysX& t);

};
