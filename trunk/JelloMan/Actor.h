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

    virtual void Update(float dTime);
    virtual void Draw(const RenderContext* pRenderContext) = 0;

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

