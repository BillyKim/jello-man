#include "PostProcessor.h"
#include <vector>

PostProcessor::PostProcessor(ID3D10Device* pDXDevice, int backBufferWidth, int backBufferHeight): 
        m_pBuffer(new Texture2D(pDXDevice, backBufferWidth, backBufferHeight, true, false)),
        m_pScreenMesh(new ModelMesh<VertexPosTex>(pDXDevice, _T("PostProcessorPlane"))),
        m_pPrevBackBuffer(0),
        m_pPrevDepthStencilView(0),
        m_Width(backBufferWidth),
        m_Height(backBufferHeight),
        m_pDXDevice(pDXDevice)
{
    Init();
}


PostProcessor::~PostProcessor(void)
{
    delete m_pBuffer;
    delete m_pScreenMesh;
}

void PostProcessor::Init()
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
void PostProcessor::SetEffect(PostProcessEffect* pEffect)
{
    m_pEffect = pEffect;
    m_pEffect->SetBackbufferSize(m_Width, m_Height);
}
void PostProcessor::Begin()
{
    m_pDXDevice->OMGetRenderTargets(1, &m_pPrevBackBuffer, &m_pPrevDepthStencilView);

    m_pBuffer->BeginDraw();
    m_pBuffer->Clear(Vector4(0, 0, 0, 1));
}
void PostProcessor::End(DeferredRenderer* deferredRenderer)
{
    ASSERT(m_pPrevBackBuffer != 0, "PostProcessor::Begin() must be called first,  or backbuffer got lost");

    m_pBuffer->EndDraw();

    m_pDXDevice->OMSetRenderTargets(1, &m_pPrevBackBuffer, NULL);

    float c[4];
    Vector4(0, 0, 0, 1).ToFloat4(c);
    m_pDXDevice->ClearRenderTargetView(m_pPrevBackBuffer, c);

    m_pEffect->SetBackbufferMap(m_pBuffer->GetColorMap());
    m_pEffect->SetColorGlowMap(deferredRenderer->GetColorGlowMap());
    m_pEffect->SetDepthMap(m_pBuffer->GetDepthMap());
    m_pEffect->SetNormalMap(deferredRenderer->GetNormalSpecMap());

    m_pScreenMesh->Draw(m_pEffect->GetEffect());

    m_pEffect->SetBackbufferMap(0);
    m_pEffect->SetNormalMap(0);
    m_pEffect->SetDepthMap(0);
    m_pEffect->GetEffect()->GetCurrentTechnique()->GetPassByIndex(0)->Apply(0); //unbind rendertarget

    m_pDXDevice->OMSetRenderTargets(1, &m_pPrevBackBuffer, m_pPrevDepthStencilView);
    SafeRelease(m_pPrevBackBuffer);
    SafeRelease(m_pPrevDepthStencilView);
}