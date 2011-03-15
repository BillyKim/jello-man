#include "DeferredRenderer.h"
#include "Vector4.h"
#include "ContentManager.h"
#include "vertex.h"
#include <vector>

DeferredRenderer::DeferredRenderer(ID3D10Device* device):
    m_pDepthDSV(0),
    m_pDevice(device),
	m_pScreenMesh(new ModelMesh<VertexPosTex>(device, _T("screenMesh"))),
	m_pEffect(0),
    m_Width(0), m_Height(0),
    m_pBackbuffer(0)
{
    ZeroMemory(&m_Viewport, sizeof(D3D10_VIEWPORT));
    Vector4(0.f, 0.f, 0.f, 0.f).ToFloat4(m_ClearColor);

    for (int i = 0; i < MAXRENDERTARGETS; ++i)
        m_RenderTargets[i] = 0;
    for (int i = 0; i < MAXRENDERTARGETS + 1; ++i)
        m_pSRV[i] = 0;
}


DeferredRenderer::~DeferredRenderer(void)
{
    for (int i = 0; i < MAXRENDERTARGETS; ++i)
        SafeRelease(m_RenderTargets[i]); 
    for (int i = 0; i < MAXRENDERTARGETS + 1; ++i)
        SafeRelease(m_pSRV[i]);
    SafeRelease(m_pDepthDSV);

	delete m_pScreenMesh;
}

void DeferredRenderer::Init(UINT width, UINT height, ID3D10RenderTargetView* pBackbuffer)
{
    m_Width = width;
    m_Height = height;
    
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = m_Width;
    m_Viewport.Height = m_Height;
    m_Viewport.MinDepth = 0.f;
    m_Viewport.MaxDepth = 1.f;

    m_pBackbuffer = pBackbuffer;

    CreateColorMap(DeferredRenderMap_Color, DXGI_FORMAT_R8G8B8A8_UNORM); //R G B A
	CreateColorMap(DeferredRenderMap_Normal, DXGI_FORMAT_R32G32B32A32_FLOAT); //X Y Z Spec
    CreateColorMap(DeferredRenderMap_Position, DXGI_FORMAT_R32G32B32A32_FLOAT); //X Y Z gloss

    CreateDepthMap();

	m_pEffect = ContentManager::GetSingleton()->LoadEffect<DeferredPostEffect>(_T("postdeferred.fx"));

	m_pScreenMesh->SetEffect(m_pEffect);
	vector<VertexPosTex> vertices;

	vertices.push_back(VertexPosTex(-1.0f, -1.0f, 1.f, 0.0f, 0.0f));
	vertices.push_back(VertexPosTex(1.0f, -1.0f, 1.f, 1.0f, 0.0f));
	vertices.push_back(VertexPosTex(-1.0f, 1.0f, 1.f, 0.0f, 1.0f));
	vertices.push_back(VertexPosTex(1.0f, 1.0f, 1.f, 1.0f, 1.0f));

	vector<DWORD> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	m_pScreenMesh->SetVertices(vertices);
    m_pScreenMesh->SetIndices(indices);
	m_pScreenMesh->SetEffect(m_pEffect);
}

UINT DeferredRenderer::GetBackbufferWidth() const
{
    return m_Width;
}
UINT DeferredRenderer::GetBackbufferHeight() const
{
    return m_Height;
}
ID3D10RenderTargetView* DeferredRenderer::GetBackbuffer() const
{
    return m_pBackbuffer;
}
ID3D10DepthStencilView* DeferredRenderer::GetDepthbuffer() const
{
    return m_pDepthDSV;
}

void DeferredRenderer::SetClearColor(const Vector4& color)
{
    color.ToFloat4(m_ClearColor);
}

void DeferredRenderer::CreateColorMap(DeferredRenderMap map, DXGI_FORMAT format)
{
    ID3D10Texture2D* colorMap = 0;
    D3D10_TEXTURE2D_DESC texDesc;

    texDesc.Width = m_Width;
    texDesc.Height = m_Height;
    texDesc.Format = format;
    texDesc.ArraySize = 1;
    texDesc.MipLevels = 1; //1
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D10_USAGE_DEFAULT;
    texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0; //no mips

    HR(m_pDevice->CreateTexture2D(&texDesc, 0, &colorMap));

    HR(m_pDevice->CreateRenderTargetView(colorMap, 0, &m_RenderTargets[map]));
    HR(m_pDevice->CreateShaderResourceView(colorMap, 0, &m_pSRV[map]));

    SafeRelease(colorMap);
}
void DeferredRenderer::CreateDepthMap()
{
    ID3D10Texture2D* depthMap = 0;
    D3D10_TEXTURE2D_DESC texDesc;

    texDesc.Width = m_Width;
    texDesc.Height = m_Height;
    texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    texDesc.ArraySize = 1;
    texDesc.MipLevels = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D10_USAGE_DEFAULT;
    texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    HR(m_pDevice->CreateTexture2D(&texDesc, 0, &depthMap));

    //Create Depth Stencil View
    D3D10_DEPTH_STENCIL_VIEW_DESC dDesc;
    dDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
    dDesc.Texture2D.MipSlice = 0;
    HR(m_pDevice->CreateDepthStencilView(depthMap, &dDesc, &m_pDepthDSV));

    //Create Shader Resource View
    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    HR(m_pDevice->CreateShaderResourceView(depthMap, &srvDesc, &m_pSRV[DeferredRenderMap_Depth]));

    SafeRelease(depthMap);
}


void DeferredRenderer::Begin() const
{ 
    m_pDevice->OMSetRenderTargets(MAXRENDERTARGETS, m_RenderTargets, m_pDepthDSV);
    m_pDevice->RSSetViewports(1, &m_Viewport);

    m_pDevice->ClearDepthStencilView(m_pDepthDSV, D3D10_CLEAR_DEPTH, 1.0f, 0);

    for (int i = 0; i < MAXRENDERTARGETS; ++i)
        m_pDevice->ClearRenderTargetView(m_RenderTargets[i], m_ClearColor);
}

void DeferredRenderer::End() const
{ 
    ASSERT(m_pBackbuffer != 0 && m_pDepthDSV != 0);
    m_pDevice->OMSetRenderTargets(1, &m_pBackbuffer, m_pDepthDSV);
    m_pDevice->RSSetViewports(1, &m_Viewport);

    m_pDevice->ClearDepthStencilView(m_pDepthDSV, D3D10_CLEAR_DEPTH, 1.0f, 0);
    m_pDevice->ClearRenderTargetView(m_pBackbuffer, m_ClearColor);

	m_pEffect->SetColorMap(m_pSRV[DeferredRenderMap_Color]);
	m_pEffect->SetNormalSpecMap(m_pSRV[DeferredRenderMap_Normal]);
	m_pEffect->SetPosGlossMap(m_pSRV[DeferredRenderMap_Position]);

	m_pScreenMesh->Draw();
}