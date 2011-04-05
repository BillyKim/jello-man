#pragma once
#include "PhysX.h"
#include "PhysXShape.h"
#include "Vector3.h"
#include "Matrix.h"
#include "RenderContext.h"

class Actor
{
public:
	virtual ~Actor(void);

    virtual void Tick(float dTime);
    virtual void Draw(const RenderContext* pRenderContext) = 0;

	void Translate(const Vector3& pos);
	void AddForce(const Vector3& pos);

	// GETTERS
	NxActor* GetActor() const
	{ return m_pActor; }

	Vector3 GetPosition() const;

protected:
	PhysX* m_pPhysX;
	NxActor* m_pActor;
	Matrix m_WorldMatrix;

	void InitActor(PhysX* pPhysX, const PhysXShape& shape, bool moveable);
	//protected constructor
	Actor(void);
};

