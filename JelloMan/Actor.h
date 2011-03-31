#pragma once
#include "PhysX.h"
#include "PhysXShape.h"
#include "Vector3.h"
#include "Matrix.h"

class Actor
{
public:
	virtual ~Actor(void);

    void Update(float dTime);

	void Translate(const Vector3& pos);
	void AddForce(const Vector3& pos);

protected:
	PhysX* m_pPhysX;
	NxActor* m_pActor;
	Matrix m_WorldMatrix;

	void InitActor(PhysX* pPhysX, const PhysXShape& shape, bool moveable);
	//protected constructor
	Actor(void);
};

