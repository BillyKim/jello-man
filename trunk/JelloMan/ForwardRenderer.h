#pragma once
#include "D3DUtil.h"
#include "Vector4.h"
#include "DeferredRenderer.h"

class ForwardRenderer
{
public:
    ForwardRenderer(ID3D10Device* pDevice);
    virtual ~ForwardRenderer(void);

    void Init(UINT width, UINT height, ID3D10RenderTargetView* pBackbuffer, ID3D10DepthStencilView* pDepthbuffer);
    void Init(DeferredRenderer* pDeferredRenderer);

    void Begin() const; 
    void End() const;

    void Clear(const Vector4& color) const;

private: 
    UINT m_Width, m_Height;

    ID3D10Device* m_pDevice;
    ID3D10RenderTargetView* m_pBackbuffer;
    ID3D10DepthStencilView* m_pDepthbuffer;
    
    D3D10_VIEWPORT m_Viewport;
};

