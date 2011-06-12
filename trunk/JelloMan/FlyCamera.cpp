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

		Matrix R(Matrix::CreateRotation(m_RightWorld, pitch));
        m_LookWorld = Vector3::Transform(m_LookWorld, R).XYZ();
        m_UpWorld = Vector3::Transform(m_UpWorld, R).XYZ();

        R = Matrix::CreateRotation(Vector3::Up, yAngle);
        m_LookWorld = Vector3::Transform(m_LookWorld, R).XYZ();
        m_UpWorld = Vector3::Transform(m_UpWorld, R).XYZ();
        m_RightWorld = Vector3::Transform(m_RightWorld, R).XYZ();

		if (CONTROLS->GetKeyboardLayout() == GameConfig::KEYBOARD_LAYOUT_AZERTY)
		{
			if (CONTROLS->IsKeyDown('A'))
			{
				m_FOV += 0.1f;
			}

			else if (CONTROLS->IsKeyDown('W'))
			{
				m_FOV -= 0.1f;
			}
		}
		else
		{
			if (CONTROLS->IsKeyDown('Q'))
			{
				m_FOV += 0.1f;
			}

			else if (CONTROLS->IsKeyDown('Z'))
			{
				m_FOV -= 0.1f;
			}
		}
	}
	else
		m_FOV = PiOver4;

	BuildProjectionMatrix();
	BuildViewMatrix();
    m_BoundingFrustum.BuildFrustum(m_matViewProjection);
}

}} //end namespace