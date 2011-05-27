#pragma once

#include "RenderContext.h"
#include "PreShadowEffect.h"

class IDrawable
{
public:
    virtual ~IDrawable() {}

    virtual void Draw(RenderContext* pRenderContext) = 0;
    virtual void DrawShadow(RenderContext* pRenderContext) = 0;
};