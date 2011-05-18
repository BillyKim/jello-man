#include "FollowCamera.h"

namespace Graphics{
namespace Camera{

FollowCamera::FollowCamera(int windowWidth, int windowHeight, const PhysX* pPhysX) : 
        CameraBase(windowWidth, windowHeight), m_pPhysX(pPhysX),
            m_FollowDist(8), m_FollowAngle(PiOver4), m_pFollowObject(0), m_PrevPosition(0, 0, 0),
            m_UpRotation(0.0f)
{
}


FollowCamera::~FollowCamera(void)
{
}

void FollowCamera::Tick(const float dTime)
{
    NxVec3 grav; 
    m_pPhysX->GetScene()->getGravity(grav);
    Vector3 up;
    if (grav.y > 0)
    {
        if (m_UpRotation < Pi)
            m_UpRotation += dTime*4;        
        if (m_UpRotation > Pi)
            m_UpRotation = Pi;
    }
    else
    {
        if (m_UpRotation >= Pi)
            m_UpRotation -= TwoPi;        
        if (m_UpRotation < 0.0f)
            m_UpRotation += dTime*4;
        if (m_UpRotation > 0.0f)
            m_UpRotation = 0.0f;
    }
    Vector3 tmpFwd = m_LookWorld;
    tmpFwd.Y = 0;
    tmpFwd.Normalize();
    up = Vector3::Transform(Vector3::Up, Matrix::CreateRotation(tmpFwd, m_UpRotation)).XYZ();

    Vector3 pos = m_pFollowObject->GetPosition();
    Vector3 direction = pos - m_PrevPosition;
    m_PrevPosition = pos;
    direction.Normalize();
    float dot = up.Dot(direction);
    direction -= up * dot;
    direction.Normalize();

    Vector3 rot = up.Cross(direction);
    Matrix mtxRot = Matrix::CreateRotation(rot, -m_FollowAngle);
    direction = Vector3::Transform(direction, mtxRot).XYZ();
    direction.X *= -1;
    direction.Z *= -1;

    if (m_DirectionQueue.size() == SMOOTH)
        m_DirectionQueue.pop_back();
    m_DirectionQueue.push_front(direction);

    direction = Vector3::Zero;

    for_each(m_DirectionQueue.cbegin(), m_DirectionQueue.cend(), [&](const Vector3& dir)
    {
        direction += dir;
    });
    direction /= m_DirectionQueue.size();


    pos += direction * m_FollowDist;


    LookAt(pos, m_pFollowObject->GetPosition(), up);

}

}} //end namespace