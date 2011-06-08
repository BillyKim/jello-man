#pragma once

class RenderContext;

class IDrawable
{
public:
    virtual ~IDrawable() {}

    virtual void Draw(const RenderContext* pRenderContext) = 0;
    virtual void DrawShadow(const RenderContext* pRenderContext) = 0;
};