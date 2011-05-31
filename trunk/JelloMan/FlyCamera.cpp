#include "FlyCamera.h"

namespace Graphics{
namespace Camera{

FlyCamera::FlyCamera(int windowWidth, int windowHeight) : CameraBase(windowWidth, windowHeight), m_bControllable(true)
{
}

FlyCamera::~FlyCamera(void)
{
}

void FlyCamera::Tick(const float dTime)
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

	float angle = static_cast<float>((CONTROLS->GetMouseWheelPos() / 120) / 200.0f);

	if (angle != 0)
	{
		m_Speed += angle*200;

		if (m_Speed < 2)
			m_Speed = 2;
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

}} //end namespace