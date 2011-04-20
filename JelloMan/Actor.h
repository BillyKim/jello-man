#pragma once
#include "PhysX.h"
#include "PhysXShape.h"
#include "Vector3.h"
#include "Matrix.h"
#include "RenderContext.h"
#include "ITransformable.h"

class Actor : public ITransformable
{
public:
	virtual ~Actor(void);

    virtual void Tick(float dTime);
    virtual void Draw(RenderContext* pRenderContext) = 0;

	void AddForce(const Vector3& pos);

	virtual void Translate(const Vector3& pos);
	virtual void SetPosition(const Vector3& pos);
	virtual const Vector3& GetPosition() const;

    virtual void Rotate(const Vector3& axis, float angle);
	virtual void Scale(const Vector3& scale);

	// GETTERS
	NxActor* GetActor() const
	{ return m_pActor; }


protected:
	PhysX* m_pPhysX;
	NxActor* m_pActor;
	Matrix m_WorldMatrix;

	void InitActor(PhysX* pPhysX, const PhysXShape& shape, bool moveable);
	//protected constructor
	Actor(void);
};

