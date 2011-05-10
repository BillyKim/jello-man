#pragma once

#include "Vector3.h"

class ITransformable
{
public:
    virtual ~ITransformable() {}

    virtual void Rotate(const Vector3& axis, float angle) = 0;

	virtual void Translate(const Vector3& add) = 0;
	virtual void SetPosition(const Vector3& pos) = 0;
	virtual Vector3 GetPosition() const = 0;

	virtual void Scale(const Vector3& scale) = 0;
};