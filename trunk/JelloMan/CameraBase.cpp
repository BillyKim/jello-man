#include "CameraBase.h"

namespace Graphics{
namespace Camera{

// CONSTRUCTOR - DESTRUCTOR
CameraBase::CameraBase(int windowWidth, int windowHeight) :	m_Speed(4.0f),
																		m_FastForward(4.0f),
																		m_MouseSensitivity(100),
																		m_FOV(PiOver4),
																		m_AspectRatio(4.0f/3.0f),
																		m_NearClippingPlane(0.1f),
																		m_FarClippingPlane(1000.0f),
																		m_bIsActive(true),
                                                                        m_matView(Matrix::Identity),
                                                                        m_matProjection(Matrix::Identity),
                                                                        m_matViewProjection(Matrix::Identity)
{
	m_PosWorld = Vector3(0.0f,7.0f,-18.0f);
	m_RightWorld = Vector3(1.0f,0.0f,0.0f);
	m_UpWorld = Vector3(0.0f,1.0f,0.0f);
	m_LookWorld = Vector3(0.0f,-0.4f,1.0f);

	SetAspectRatio(static_cast<float>(windowWidth/windowHeight));
}

CameraBase::~CameraBase()
{
}

bool CameraBase::IsInView(const BoundingSphere& sphere)
{
    return m_BoundingFrustum.CheckCollision(sphere);
}

// GENERAL
void CameraBase::Tick(const float /*dTime*/)
{
}

void CameraBase::OnResize(int windowWidth, int windowHeight)
{
	SetAspectRatio(static_cast<float>(windowWidth/windowHeight));
}

// SETTERS
void CameraBase::LookAt(	const Vector3& pos,
						const Vector3& target,
						const Vector3& up	)
{
	Vector3 lookAt = target - pos;
	lookAt.Normalize();

	Vector3	right = up.Cross(lookAt);

	Vector3 newUp = up.Cross(right);
	newUp.Normalize();

	m_PosWorld = pos;
	m_RightWorld = right;
	m_UpWorld = newUp;
	m_LookWorld = lookAt;

	BuildViewMatrix();
}

//	camera params
void CameraBase::SetLens(	float aspectRatio,
						float fov,
						float nearZ,
						float farZ	)
{
	m_FOV = fov;
	m_AspectRatio = aspectRatio;
	m_NearClippingPlane = nearZ;
	m_FarClippingPlane = farZ;
	
	BuildProjectionMatrix();
}

void CameraBase::SetMouseSensitivity(float sens)
{
	if (sens <= 0) m_MouseSensitivity = 1000;
	else m_MouseSensitivity = 1000 / sens;
}

//	build matrix
void CameraBase::BuildViewMatrix()
{
    m_matView = Matrix::CreateLookAt(m_PosWorld, m_PosWorld + m_LookWorld, -m_UpWorld);
	m_matViewProjection = m_matView * m_matProjection;
}
void CameraBase::BuildProjectionMatrix()
{
	if (m_FOV > Pi / 5 * 4.0f) m_FOV = static_cast<float>(Pi / 5 * 4.0f);
	if (m_FOV < Pi / 30.0f) m_FOV = static_cast<float>(Pi / 30.0f);

    m_matProjection = Matrix::CreatePerspectiveFov(m_FOV, m_AspectRatio, m_NearClippingPlane, m_FarClippingPlane);

	m_matViewProjection = m_matView * m_matProjection;
}

}} //end namespace