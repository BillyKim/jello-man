#pragma once

#include "D3DUtil.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Controls.h"
#include "Blox2D.h"

class Camera
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Camera(int windowWidth, int windowHeight);
	virtual ~Camera();


	// GENERAL
	void Tick(const float dTime);
	void OnResize(int windowWidth, int windowHeight);


	// SETTERS
	void SetPosition(const Vector3& pos)
	{ m_PosWorld = pos; }

	void LookAt(	const Vector3& pos,
					const Vector3& target,
					const Vector3& up	);

	//	camera params
	void SetLens(	float aspectRatio = (4.0f/3.0f),
					float fov = PiOver4,
					float nearZ = 1.0f,
					float farZ = 10000.0f	);

	void SetAspectRatio(float aspectRatio = (4.0f/3.0f))
	{ m_AspectRatio = aspectRatio; }

	void SetSpeed(float s)
	{ m_Speed = s; }
	void SetMouseSensitivity(float sens);

	//	make camera active
	void SetActive(bool active);


	// GETTERS
	//	matrices
	const Matrix& GetView() const
	{ return m_matView; }
	const Matrix& GetProjection() const
	{ return m_matProjection; }
	const Matrix& GetViewProjection() const
	{ return m_matViewProjection; }

	//	vectors
	const Vector3& GetRight() const
	{ return m_RightWorld; }
	const Vector3& GetUp() const
	{ return m_UpWorld; }
	const Vector3& GetLook() const
	{ return m_LookWorld; }
	const Vector3& GetPosition() const
	{ return m_PosWorld; }

private:

	//	build matrix
	void BuildViewMatrix();
	void BuildProjectionMatrix();

	// DATAMEMBERS
	Matrix m_matView;
	Matrix m_matProjection;
	Matrix m_matViewProjection;
	Matrix m_World;

	Vector3 m_PosWorld;
	Vector3 m_RightWorld;
	Vector3 m_UpWorld;
	Vector3 m_LookWorld;

	float m_Speed;
	float m_FastForward;
	float m_MouseSensitivity;
	float m_FOV;
	float m_AspectRatio;
	float m_NearClippingPlane, m_FarClippingPlane;

	bool m_bIsActive;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Camera(const Camera& t);
	Camera& operator=(const Camera& t);
};