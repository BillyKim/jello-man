#include "ForwardRenderer.h"


ForwardRenderer::ForwardRenderer(ID3D10Device* pDevice): 
            m_pDevice(pDevice),
            m_pDepthStencil(0), m_pPrevDepthStencil(0),
            m_pRenderTarget(0)
{
}


ForwardRenderer::~ForwardRenderer(void)
{
}

void ForwardRenderer::Begin(DeferredRenderer* pDeferredRenderer)
{
    m_pDevice->OMGetRenderTargets(1, &m_pRenderTarget, &m_pDepthStencil);
    m_pDepthStencil = pDeferredRenderer->GetDepthbuffer();
    m_pDevice->OMSetRenderTargets(1, &m_pRenderTarget, m_pDepthStencil);
}
void ForwardRenderer::End()
{
    m_pDevice->OMSetRenderTargets(1, &m_pRenderTarget, m_pPrevDepthStencil);
    m_pRenderTarget = 0;
    m_pPrevDepthStencil = 0;
    m_pDepthStencil = 0;
}

void ForwardRenderer::Clear(const Vector4& color, bool depthstencil) const
{
    ASSERT(m_pDepthStencil != 0 && m_pRenderTarget != 0, "");

    if (depthstencil)
        m_pDevice->ClearDepthStencilView(m_pDepthStencil, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);

    float c[4];
    color.ToFloat4(c);
    m_pDevice->ClearRenderTargetView(m_pRenderTarget, c);
}