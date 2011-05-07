#include "Camera.h"

#define CONTROLS (Controls::GetSingleton())

// CONSTRUCTOR - DESTRUCTOR
Camera::Camera(int windowWidth, int windowHeight, bool bControllable) :	m_Speed(200.0f),
																		m_FastForward(4.0f),
																		m_MouseSensitivity(100),
																		m_FOV(PiOver4),
																		m_AspectRatio(4.0f/3.0f),
																		m_NearClippingPlane(0.1f),
																		m_FarClippingPlane(10000.0f),
																		m_bIsActive(true),
																		m_bControllable(bControllable)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProjection);
	D3DXMatrixIdentity(&m_matViewProjection);
	D3DXMatrixIdentity(&m_World);

	m_PosWorld = Vector3(0.0f,7.0f,-18.0f);
	m_RightWorld = Vector3(1.0f,0.0f,0.0f);
	m_UpWorld = Vector3(0.0f,1.0f,0.0f);
	m_LookWorld = Vector3(0.0f,-0.4f,1.0f);

	SetAspectRatio(static_cast<float>(windowWidth/windowHeight));
}

Camera::~Camera()
{
}

// GENERAL
void Camera::Tick(const float dTime)
{
	if (!m_bIsActive) return;

	if (m_bControllable)
	{
		bool bRunning = false;

		// camera controls
		Vector3 dir(0.0f, 0.0f, 0.0f);

		if (!CONTROLS->IsKeyDown(VK_LCONTROL))
		{
			if (CONTROLS->GetKeyboardLayout() == GameConfig::KEYBOARD_LAYOUT_AZERTY)
			{
				if (CONTROLS->IsKeyDown('Z'))
					dir += m_LookWorld;
				if (CONTROLS->IsKeyDown('Q'))
					dir -= m_RightWorld;
			}
			else
			{
				if (CONTROLS->IsKeyDown('W'))
					dir += m_LookWorld;
				if (CONTROLS->IsKeyDown('A'))
					dir -= m_RightWorld;
			}

			if (CONTROLS->IsKeyDown('S'))
					dir -= m_LookWorld;
			if (CONTROLS->IsKeyDown('D'))
				dir += m_RightWorld;
		}

		// fast forward
		if (CONTROLS->IsKeyDown(VK_LSHIFT))
			bRunning = true;

		dir.Normalize();
		float finalSpeed = m_Speed;
		if (bRunning) finalSpeed *= m_FastForward;

		m_PosWorld += dir * finalSpeed * dTime;
	}

	// change FOV with mousewheel
	float angle = static_cast<float>((CONTROLS->GetMouseWheelPos() / 120) / 5.0f);

	if (angle != 0)
	{
		m_Speed += angle*500;

		if (m_Speed < 10)
			m_Speed = 10;
	}

	if (CONTROLS->RightMBDown())
	{
		Point2D mouseMovement = CONTROLS->GetMouseMovement();
		float pitch = mouseMovement.y / m_MouseSensitivity;
		float yAngle = mouseMovement.x / m_MouseSensitivity;

		D3DXMATRIX R;
		D3DXVECTOR3 rightWorld = m_RightWorld.ToD3DVector3();
		D3DXVECTOR3 lookWorld = m_LookWorld.ToD3DVector3();
		D3DXVECTOR3 upWorld = m_UpWorld.ToD3DVector3();

		D3DXMatrixRotationAxis(&R, &rightWorld, pitch);
		D3DXVec3TransformCoord(&lookWorld, &lookWorld, &R);
		D3DXVec3TransformCoord(&upWorld, &upWorld,&R);

		D3DXMatrixRotationY(&R, yAngle);
		D3DXVec3TransformCoord(&rightWorld, &rightWorld, &R);
		D3DXVec3TransformCoord(&upWorld, &upWorld, &R);
		D3DXVec3TransformCoord(&lookWorld, &lookWorld, &R);

		m_RightWorld = Vector3(rightWorld);
		m_LookWorld = Vector3(lookWorld);
		m_UpWorld = Vector3(upWorld);

		if (CONTROLS->GetKeyboardLayout() == GameConfig::KEYBOARD_LAYOUT_AZERTY)
		{
			if (CONTROLS->IsKeyDown('A'))
			{
				m_FOV += 0.1f;
				BuildProjectionMatrix();
			}

			else if (CONTROLS->IsKeyDown('W'))
			{
				m_FOV -= 0.1f;
				BuildProjectionMatrix();
			}
		}
		else
		{
			if (CONTROLS->IsKeyDown('Q'))
			{
				m_FOV += 0.1f;
				BuildProjectionMatrix();
			}

			else if (CONTROLS->IsKeyDown('Z'))
			{
				m_FOV -= 0.1f;
				BuildProjectionMatrix();
			}
		}
	}
	else
		m_FOV = PiOver4;

	BuildProjectionMatrix();
	BuildViewMatrix();
	m_matViewProjection = m_matView * m_matProjection;
}

void Camera::OnResize(int windowWidth, int windowHeight)
{

}

// SETTERS
void Camera::LookAt(	const Vector3& pos,
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

	m_matViewProjection = m_matView * m_matProjection;
}

//	camera params
void Camera::SetLens(	float aspectRatio,
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

void Camera::SetMouseSensitivity(float sens)
{
	if (sens <= 0) m_MouseSensitivity = 1000;
	else m_MouseSensitivity = 1000 / sens;
}

//	build matrix
void Camera::BuildViewMatrix()
{
	D3DXVECTOR3 posW,upW,lookW,rightW;
	D3DXVECTOR4 out;

	D3DXVECTOR3 rightWorld = m_RightWorld.ToD3DVector3();
	D3DXVECTOR3 lookWorld = m_LookWorld.ToD3DVector3();
	D3DXVECTOR3 upWorld = m_UpWorld.ToD3DVector3();
	D3DXVECTOR3 posWorld = m_PosWorld.ToD3DVector3();

	D3DXVec3Transform(&out, &posWorld, &m_World);
	posW = D3DXVECTOR3(out.x,out.y,out.z);

	m_World._41 = 0;
	m_World._42 = 0;
	m_World._43 = 0;

	D3DXVec3Transform(&out, &upWorld, &m_World);
	upW = D3DXVECTOR3(out.x,out.y,out.z);

	D3DXVec3Transform(&out, &lookWorld, &m_World);
	lookW = D3DXVECTOR3(out.x,out.y,out.z);

	D3DXVec3Transform(&out, &rightWorld, &m_World);
	rightW = D3DXVECTOR3(out.x,out.y,out.z);

	D3DXVec3Normalize(&lookW, &lookW);

	D3DXVec3Cross(&upW, &lookW, &rightW);
	D3DXVec3Normalize(&upW, &upW);

	D3DXVec3Cross(&rightW, &upW, &lookW);
	D3DXVec3Normalize(&rightW, &rightW);

    D3DXVECTOR3 lookWPlusPosW = lookW + posW;

	D3DXMatrixLookAtLH(&m_matView, &posW, &lookWPlusPosW, &upW);
}
void Camera::BuildProjectionMatrix()
{
	if (m_FOV > Pi / 5 * 4.0f) m_FOV = static_cast<float>(Pi / 5 * 4.0f);
	if (m_FOV < Pi / 30.0f) m_FOV = static_cast<float>(Pi / 30.0f);

	D3DXMatrixPerspectiveFovLH(	&m_matProjection,
								m_FOV, m_AspectRatio,
								m_NearClippingPlane,
								m_FarClippingPlane);

	m_matViewProjection = m_matView * m_matProjection;
}