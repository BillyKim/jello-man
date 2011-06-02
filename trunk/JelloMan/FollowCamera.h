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
    #pragma warning(disable:4480) //we only use 8bits for the flags, so we define the type of this enum as unsigned char
    enum SmoothFlag : BYTE
    {
        SmoothFlag_Distance = 1 << 1,
        SmoothFlag_Up = 1 << 2,
        SmoothFlag_Direction = 1 << 3
    };
    #pragma warning(default:4480)

public:
    FollowCamera(int windowWidth, int windowHeight, const PhysX* pPhysX);
    virtual ~FollowCamera(void);

    void SetFollowPosition(const Vector3& pos);
    void SetFollowDistance(float dist);
    void SetFollowUp(const Vector3& up);
    void SetFollowDirection(const Vector3& direction);

    void SetSmoothFlags(BYTE flags);

	virtual void Tick(const float dTime);

private:
    BYTE m_SmoothFlags;

    Vector3 m_Up, m_Direction, m_Position;
    float m_Distance;

    const PhysX* m_pPhysX;

    const static int SMOOTH = 30;
    std::deque<Vector3> m_DirectionQueue;
    std::deque<Vector3> m_UpQueue;
    std::deque<float> m_DistQueue;

    // DISABLE DEFAULT COPY & ASSIGNMENT
    FollowCamera(const FollowCamera& t);
    FollowCamera& operator=(const FollowCamera& t);
};

}} //end namespace

