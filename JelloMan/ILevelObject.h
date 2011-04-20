#pragma once
#include "IUpdatable.h"
#include "IDrawable.h"
#include "ITransformable.h"
#include "PhysX.h"

class ILevelObject : public IUpdatable, public IDrawable, public ITransformable
{
public:
    virtual void Init(PhysX* pPhysX) = 0;
    virtual void Selected(bool selected) = 0;

    //ITransformable
    virtual void Rotate(const Vector3& axis, float angle) = 0;

	virtual void Translate(const Vector3& add) = 0;
	virtual void SetPosition(const Vector3& pos) = 0;
	virtual const Vector3& GetPosition() const = 0;

	virtual void Scale(const Vector3& scale) = 0;

    //IUpdateable
    virtual void Tick(float dTime) = 0;

    //IDrawable
    virtual void Draw(RenderContext* pRenderContext) = 0;
    virtual void DrawShadow(RenderContext* pRenderContext, PreShadowEffect* e) = 0;
};