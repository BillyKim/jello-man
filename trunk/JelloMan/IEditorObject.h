#pragma once
#include "ITransformable.h"

class IEditorObject : public ITransformable
{
public:
    virtual ~IEditorObject() {}

    virtual void Rotate(const Vector3& axis, float angle) = 0;

	virtual void Translate(const Vector3& add) = 0;
	virtual void SetPosition(const Vector3& pos) = 0;
	virtual Vector3 GetPosition() const = 0;

	virtual void Scale(const Vector3& scale) = 0;
    
    virtual void Selected(bool selected) = 0;
    virtual bool IsSelected() const = 0;
};