#include "FluidRenderer.h"
#include <vector>

FluidRenderer::FluidRenderer(ID3D10Device* pDXDevice, int backBufferWidth, int backBufferHeight): 
        m_pBuffer(new Texture2D(pDXDevice, backBufferWidth, backBufferHeight, true, false)),
        m_pScreenMesh(new ModelMesh<VertexPosTex>(pDXDevice, _T("FluidPlane"))),
        m_pPrevBackBuffer(0),
        m_pPrevDepthStencilView(0),
        m_Width(backBufferWidth),
        m_Height(backBufferHeight),
        m_pDXDevice(pDXDevice)
{
    Init();
}

void FluidRenderer::OnResized(int backBufferWidth, int backBufferHeight)
{
    delete  m_pBuffer;
    m_pBuffer = new Texture2D(m_pDXDevice, backBufferWidth, backBufferHeight, true, false);
    m_Width = backBufferWidth;
    m_Height = backBufferHeight;
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
}
void FluidRenderer::SetEffect(PostProcessEffect* pEffect)
{
    m_pEffect = pEffect;
    m_pEffect->SetBackbufferSize(m_Width, m_Height);
}
void FluidRenderer::Begin()
{
    m_pDXDevice->OMGetRenderTargets(1, &m_pPrevBackBuffer, &m_pPrevDepthStencilView);

    m_pBuffer->BeginDraw();
    m_pBuffer->Clear(Vector4(0, 0, 0, 1));
}
void FluidRenderer::End()
{
    ASSERT(m_pPrevBackBuffer != 0, "PostProcessor::Begin() must be called first,  or backbuffer got lost");

    m_pBuffer->EndDraw();

    m_pDXDevice->OMSetRenderTargets(1, &m_pPrevBackBuffer, NULL);

    float c[4];
    Vector4(0, 0, 0, 1).ToFloat4(c);
    m_pDXDevice->ClearRenderTargetView(m_pPrevBackBuffer, c);

    m_pEffect->SetBackbufferMap(m_pBuffer->GetColorMap());

    m_pScreenMesh->Draw(m_pEffect->GetEffect());

    m_pEffect->SetBackbufferMap(0);
    m_pEffect->SetNormalMap(0);
    m_pEffect->SetDepthMap(0);
    m_pEffect->SetColorGlowMap(0);
    m_pEffect->GetEffect()->GetCurrentTechnique()->GetPassByIndex(0)->Apply(0); //unbind rendertarget

    m_pDXDevice->OMSetRenderTargets(1, &m_pPrevBackBuffer, m_pPrevDepthStencilView);
    SafeRelease(m_pPrevBackBuffer);
    SafeRelease(m_pPrevDepthStencilView);
}
