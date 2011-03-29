#pragma once
#include "D3DUtil.h"
#include "Vector4.h"
#include "DeferredRenderer.h"

class ForwardRenderer
{
public:
    ForwardRenderer(ID3D10Device* pDevice);
    virtual ~ForwardRenderer(void);

    void Begin(DeferredRenderer* pDeferredRenderer); 
    void End();

    void Clear(const Vector4& color, bool depthstencil) const;

private: 
    ID3D10Device* m_pDevice;
    ID3D10RenderTargetView* m_pRenderTarget;
    ID3D10DepthStencilView* m_pDepthStencil, *m_pPrevDepthStencil;
};

