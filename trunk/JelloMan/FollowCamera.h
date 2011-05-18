#pragma once
#include "CameraBase.h"
#include "ILevelObject.h"
#include "PhysX.h"
#include <deque>

namespace Graphics{
namespace Camera{

class FollowCamera : public CameraBase
{
public:
    FollowCamera(int windowWidth, int windowHeight, const PhysX* pPhysX);
    virtual ~FollowCamera(void);

    void SetFollowObject(const ILevelObject* obj) { m_pFollowObject = obj; }
    void SetFollowDistance(float dist) { m_FollowDist = dist; }
    void SetFollowAngle(float degrees) { m_FollowAngle = ToRadians(degrees); }

	virtual void Tick(const float dTime);

private:

    const ILevelObject* m_pFollowObject;
    float m_FollowDist;
    float m_FollowAngle;

    const PhysX* m_pPhysX;

    const static int SMOOTH = 30;
    std::deque<Vector3> m_DirectionQueue;
    
    float m_UpRotation;
    Vector3 m_PrevPosition;


    // DISABLE DEFAULT COPY & ASSIGNMENT
    FollowCamera(const FollowCamera& t);
    FollowCamera& operator=(const FollowCamera& t);
};

}} //end namespace

