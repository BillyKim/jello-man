#pragma once

#include "RenderContext.h"

class IGizmo
{
public:

	virtual ~IGizmo() {}

	virtual void Tick() = 0;
	virtual void Draw(const RenderContext* pRenderContext) = 0;
};

