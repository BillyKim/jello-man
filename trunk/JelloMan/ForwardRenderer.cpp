#include "ForwardRenderer.h"


ForwardRenderer::ForwardRenderer(ID3D10Device* pDevice): 
            m_Height(0),
            m_Width(0),
            m_pBackbuffer(0),
            m_pDepthbuffer(0),
            m_pDevice(pDevice)
{
    ZeroMemory(&m_Viewport, sizeof(D3D10_VIEWPORT));
}


ForwardRenderer::~ForwardRenderer(void)
{
}

void ForwardRenderer::Init(UINT width, UINT height, ID3D10RenderTargetView* pBackbuffer, ID3D10DepthStencilView* pDepthbuffer)
{
    m_Width = width;
    m_Height = height;
    m_pBackbuffer = pBackbuffer;
    m_pDepthbuffer = pDepthbuffer;

    m_Viewport.Width = m_Width;
    m_Viewport.Height = m_Height;
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
}
void ForwardRenderer::Init(DeferredRenderer* pDeferredRenderer)
{
    m_Width = pDeferredRenderer->GetBackbufferWidth();
    m_Height = pDeferredRenderer->GetBackbufferHeight();
    m_pBackbuffer = pDeferredRenderer->GetBackbuffer();
    m_pDepthbuffer = pDeferredRenderer->GetDepthbuffer();

    m_Viewport.Width = m_Width;
    m_Viewport.Height = m_Height;
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
}

void ForwardRenderer::Begin() const
{
    ASSERT(m_pBackbuffer != NULL && m_pDepthbuffer != NULL);
    m_pDevice->OMSetRenderTargets(1, &m_pBackbuffer, m_pDepthbuffer);
    m_pDevice->RSSetViewports(1, &m_Viewport);
}
void ForwardRenderer::End() const
{
}

void ForwardRenderer::Clear(const Vector4& color) const
{
    m_pDevice->ClearDepthStencilView(m_pDepthbuffer, D3D10_CLEAR_DEPTH, 1.0f, 0);

    float c[4];
    color.ToFloat4(c);
    m_pDevice->ClearRenderTargetView(m_pBackbuffer, c);
}