#pragma once

#include "D3DUtil.h"
#include "Vector4.h"
#include "Controls.h"
#include "Blox2D.h"
#include "BoundingFrustum.h"
#include "BoundingSphere.h"
#include "Matrix.h"

namespace Graphics {
namespace Camera {
class CameraBase
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	CameraBase(int windowWidth, int windowHeight);
	virtual ~CameraBase();


	// GENERAL
	virtual void Tick(const float dTime);
	virtual void OnResize(int windowWidth, int windowHeight);


	// SETTERS
	virtual void SetPosition(const Vector3& pos)
	{ m_PosWorld = pos; }

	virtual void LookAt(	const Vector3& pos,
					const Vector3& target,
					const Vector3& up	);

	//	camera params
	virtual void SetLens(	float aspectRatio = (4.0f/3.0f),
					float fov = PiOver4,
					float nearZ = 10.0f,
					float farZ = 10000.0f	);

	virtual void SetAspectRatio(float aspectRatio = (4.0f/3.0f))
	{ m_AspectRatio = aspectRatio; }

	virtual void SetSpeed(float s)
	{ m_Speed = s; }
	virtual void SetMouseSensitivity(float sens);

	//	make camera active
    virtual void SetActive(bool active) { m_bIsActive = active; }

    virtual bool IsInView(const BoundingSphere& sphere);

	// GETTERS
	//	matrices
	virtual Matrix GetView() const
	{ return Matrix(m_matView); }
	virtual Matrix GetProjection() const
	{ return Matrix(m_matProjection); }
	virtual Matrix GetViewProjection() const
	{ return Matrix(m_matViewProjection); }

	//	vectors
	virtual Vector3 GetRight() const
	{ return m_RightWorld; }
	virtual Vector3 GetUp() const
	{ return m_UpWorld; }
	virtual Vector3 GetLook() const
	{ return m_LookWorld; }
	virtual Vector3 GetPosition() const
	{ return m_PosWorld; }

protected:

	//	build matrix
	void BuildViewMatrix();
	void BuildProjectionMatrix();

	// DATAMEMBERS
	Matrix m_matView;
	Matrix m_matProjection;
	Matrix m_matViewProjection;

	Vector3 m_PosWorld;
	Vector3 m_RightWorld;
	Vector3 m_UpWorld;
	Vector3 m_LookWorld;

    BoundingFrustum m_BoundingFrustum;

	float m_Speed;
	float m_FastForward;
	float m_MouseSensitivity;
	float m_FOV;
	float m_AspectRatio;
	float m_NearClippingPlane, m_FarClippingPlane;

	bool m_bIsActive;

private:
	// DISABLE DEFAULT COPY & ASSIGNMENT
	CameraBase(const CameraBase& t);
	CameraBase& operator=(const CameraBase& t);
};
}}