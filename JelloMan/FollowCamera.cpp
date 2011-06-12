#include "FollowCamera.h"

namespace Graphics{
namespace Camera{

FollowCamera::FollowCamera(int windowWidth, int windowHeight, const PhysX* pPhysX) : 
        CameraBase(windowWidth, windowHeight), m_pPhysX(pPhysX),
            m_Direction(Vector3::Forward), m_Up(Vector3::Up) , m_Distance(8),
            m_SmoothFlags(SmoothFlag_Direction | SmoothFlag_Distance | SmoothFlag_Up)
{
    m_DistQueue.resize(SMOOTH);
    m_UpQueue.resize(SMOOTH);
    m_DirectionQueue.resize(SMOOTH);
}


FollowCamera::~FollowCamera(void)
{
}

void FollowCamera::SetFollowPosition(const Vector3& pos)
{
    m_Position = pos;
}
void FollowCamera::SetFollowDistance(float dist)
{
    m_Distance = dist;
    if (m_SmoothFlags & SmoothFlag_Distance)
    {
        if (m_DistQueue.size() >= SMOOTH)
            m_DistQueue.pop_front();
        m_DistQueue.push_back(dist);
    }
}
void FollowCamera::SetFollowUp(const Vector3& up)
{
    m_Up = up;
    if (m_SmoothFlags & SmoothFlag_Up)
    {
        if (m_UpQueue.size() >= SMOOTH)
            m_UpQueue.pop_front();
        m_UpQueue.push_back(up);
    }
}
void FollowCamera::SetFollowDirection(const Vector3& direction)
{
    m_Direction = direction;
    if (m_SmoothFlags & SmoothFlag_Direction)
    {
        if (m_DirectionQueue.size() >= SMOOTH)
            m_DirectionQueue.pop_front();
        m_DirectionQueue.push_back(direction);
    }
}
void FollowCamera::SetSmoothFlags(BYTE flags)
{
    m_SmoothFlags = flags;
}

void FollowCamera::Tick(const float /*dTime*/)
{
    if (m_SmoothFlags & SmoothFlag_Direction)
    {
        m_Direction = Vector3::Zero;
        for_each(m_DirectionQueue.cbegin(), m_DirectionQueue.cend(), [&](const Vector3& dir)
        {
            m_Direction += dir;
        });
        m_Direction /= static_cast<float>(m_DirectionQueue.size());
        m_Direction.Normalize();
    }
    if (m_SmoothFlags & SmoothFlag_Distance)
    {
        m_Distance = 0.0f;
        for_each(m_DistQueue.cbegin(), m_DistQueue.cend(), [&](float dist)
        {
            m_Distance += dist;
        });
        m_Distance /= m_DistQueue.size();
    }
    if (m_SmoothFlags & SmoothFlag_Up)
    {
        m_Up = Vector3::Zero;    
        for_each(m_UpQueue.cbegin(), m_UpQueue.cend(), [&](const Vector3& up)
        {
            m_Up += up;
        });
        m_Up /= static_cast<float>(m_UpQueue.size());
        m_Up.Normalize();
    }

    LookAt(m_Position - m_Direction * m_Distance, m_Position, m_Up);
    m_BoundingFrustum.BuildFrustum(m_matViewProjection);
}

}} //end namespace