#pragma once

#include "Model.h"
#include "Texture2D.h"
#include "DeferredPreEffect.h"
#include "Vector4.h"
#include "Matrix.h"
#include "RenderContext.h"
#include "LevelObject.h"

class Character	: public LevelObject
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Character(Camera* pTrackingCamera);
	virtual ~Character();

	// GENERAL
	virtual void Tick(const float dTime);
	void Move(Vector3 move);

private:

	// DATAMEMBERS
	float m_RotationAngle;
	int m_Speed;

	Vector3 m_Pos;

	Camera* m_pTrackingCamera;
};

