#pragma once
#include "PhysX.h"
#include "PhysXShape.h"
#include "Vector3.h"
#include "Matrix.h"

class Actor
{
public:
	Actor(void);
	virtual ~Actor(void);

	void InitActor(PhysX* pPhysX, bool, const PhysXShape& shape);

	void Translate(const Vector3& pos);
	void AddForce(const Vector3& pos);

protected:
	PhysX* m_pPhysX;
	NxActor* m_pActor;
	Matrix m_WorldMatrix;
};

