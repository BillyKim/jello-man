#include "FluidRenderer.h"
#include <vector>
#include "ContentManager.h"

FluidRenderer::FluidRenderer(ID3D10Device* pDXDevice, int backBufferWidth, int backBufferHeight): 
        m_pBuffer(new Texture2D(pDXDevice, backBufferWidth, backBufferHeight, true, false)),
        m_pScreenMesh(new ModelMesh<VertexPosTex>(pDXDevice, _T("FluidPlane"))),
        m_pPrevBackBuffer(0),
        m_pPrevDepthStencilView(0),
        m_Width(backBufferWidth),
        m_Height(backBufferHeight),
        m_pDXDevice(pDXDevice),
        m_pPostEffect(0)
{
    Init();
}

void FluidRenderer::OnResized(int backBufferWidth, int backBufferHeight)
{
    delete  m_pBuffer;
    m_Width = backBufferWidth;
    m_Height = backBufferHeight;
    m_pBuffer = new Texture2D(m_pDXDevice, m_Width, m_Height, true, false);
}

FluidRenderer::~FluidRenderer(void)
{
    delete m_pBuffer;
    delete m_pScreenMesh;
}

void FluidRenderer::Init()
{
    vector<VertexPosTex> vertices;

	vertices.push_back(VertexPosTex(-1.0f,  1.0f, 0.5f, 0.0f, 0.0f));
	vertices.push_back(VertexPosTex( 1.0f,  1.0f, 0.5f, 1.0f, 0.0f));
	vertices.push_back(VertexPosTex(-1.0f, -1.0f, 0.5f, 0.0f, 1.0f));
	vertices.push_back(VertexPosTex( 1.0f, -1.0f, 0.5f, 1.0f, 1.0f));

	vector<DWORD> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	m_pScreenMesh->SetVertices(vertices);
    m_pScreenMesh->SetIndices(indices);

    m_pPostEffect = Content->LoadEffect<FluidPostEffect>(_T("../Content/Effects/fluidPostEffect.fx"));
    m_pPostEffect->SetBackbufferSize(m_Width, m_Height);
}
void FluidRenderer::Begin()
{
    m_pDXDevice->OMGetRenderTargets(1, &m_pPrevBackBuffer, &m_pPrevDepthStencilView);

    m_pBuffer->BeginDraw();
    m_pBuffer->Clear(Vector4(0, 0, 0, 0));
}
void FluidRenderer::End(const RenderContext* pRenderContext)
{
    ASSERT(m_pPrevBackBuffer != 0, "PostProcessor::Begin() must be called first,  or backbuffer got lost");

    m_pBuffer->EndDraw();

    m_pDXDevice->OMSetRenderTargets(1, &m_pPrevBackBuffer, NULL);

    //float c[4];
    //Vector4(0, 0, 0, 1).ToFloat4(c);
    //m_pDXDevice->ClearRenderTargetView(m_pPrevBackBuffer, c);

    m_pPostEffect->SetBackbufferMap(m_pBuffer->GetColorMap());
    m_pPostEffect->SetDepthMaps(pRenderContext->GetDeferredRenderer()->GetDepthMap(), m_pBuffer->GetDepthMap());
    m_pPostEffect->SetCamDir(pRenderContext->GetCamera()->GetLook());

    m_pScreenMesh->Draw(m_pPostEffect->GetEffect());

    m_pPostEffect->SetBackbufferMap(0);
    m_pPostEffect->SetDepthMaps(0, 0);
    m_pPostEffect->GetEffect()->GetCurrentTechnique()->GetPassByIndex(0)->Apply(0); //unbind rendertarget

    m_pDXDevice->OMSetRenderTargets(1, &m_pPrevBackBuffer, m_pPrevDepthStencilView);
    SafeRelease(m_pPrevBackBuffer);
    SafeRelease(m_pPrevDepthStencilView);
}
