#include "stdafx.h"
#include "Texture2D.h"


Texture2D::Texture2D(ID3D10ShaderResourceView* pTex): 
    m_Width(0),
    m_Height(0),
    m_ColorFormat(DXGI_FORMAT_B8G8R8A8_UNORM),
    m_pDevice(0),
    m_pColorMapSRV(pTex), m_pColorMapRTV(0),
    m_pDepthMapSRV(0), m_pDepthMapDSV(0)
{
    ZeroMemory(&m_Viewport, sizeof(D3D10_VIEWPORT)); 
}
Texture2D::Texture2D(ID3D10Device* device, UINT width, UINT height, bool hasColormap, bool mips, DXGI_FORMAT colorFormat):  
    m_Width(width),
    m_Height(height),
    m_ColorFormat(colorFormat),
    m_pDevice(device),
    m_pColorMapSRV(0), m_pColorMapRTV(0),
    m_pDepthMapSRV(0), m_pDepthMapDSV(0),
    m_MakeMips(mips)
{
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = m_Width;
    m_Viewport.Height = m_Height;
    m_Viewport.MinDepth = 0.f;
    m_Viewport.MaxDepth = 1.f;

    BuildDepthMap();
    if (hasColormap)
        BuildColorMap();
}


Texture2D::~Texture2D(void)
{
    SafeRelease(m_pColorMapRTV);
    SafeRelease(m_pColorMapSRV);
    SafeRelease(m_pDepthMapDSV);
    SafeRelease(m_pDepthMapSRV);
}


void Texture2D::BuildColorMap()
{
    ID3D10Texture2D* colorMap = 0;
    D3D10_TEXTURE2D_DESC texDesc;

    texDesc.Width = m_Width;
    texDesc.Height = m_Height;
    texDesc.Format = m_ColorFormat;
    texDesc.ArraySize = 1;
    texDesc.MipLevels = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D10_USAGE_DEFAULT;
    texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

    HR(m_pDevice->CreateTexture2D(&texDesc, 0, &colorMap));

    HR(m_pDevice->CreateRenderTargetView(colorMap, 0, &m_pColorMapRTV));
    HR(m_pDevice->CreateShaderResourceView(colorMap, 0, &m_pColorMapSRV));

    SafeRelease(colorMap);
}
void Texture2D::BuildDepthMap()
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
    HR(m_pDevice->CreateDepthStencilView(depthMap, &dDesc, &m_pDepthMapDSV));

    //Create Shader Resource View
    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    HR(m_pDevice->CreateShaderResourceView(depthMap, &srvDesc, &m_pDepthMapSRV));

    SafeRelease(depthMap);
}

void Texture2D::BeginDraw()
{
    ASSERT(m_pDepthMapDSV != 0, "");

    m_pDevice->OMGetRenderTargets(1, &m_pPrevRenderTarget, &m_pPrevDSV);

    ID3D10RenderTargetView* renderTargets[1] = { m_pColorMapRTV };
    m_pDevice->OMSetRenderTargets(1, renderTargets, m_pDepthMapDSV);
    m_pDevice->RSSetViewports(1, &m_Viewport);

    m_pDevice->ClearDepthStencilView(m_pDepthMapDSV, D3D10_CLEAR_DEPTH, 1.0f, 0);
}
void Texture2D::EndDraw()
{
    if (m_pColorMapSRV && m_MakeMips)
        m_pDevice->GenerateMips(m_pColorMapSRV);
    
    ID3D10RenderTargetView* renderTargets[1] = { m_pPrevRenderTarget };
    m_pDevice->OMSetRenderTargets(1, renderTargets, m_pPrevDSV);
}
void Texture2D::Clear(const Vector4& color)
{
    ASSERT(m_pColorMapRTV != 0, "");
    float c[4];
    color.ToFloat4(c);
    m_pDevice->ClearRenderTargetView(m_pColorMapRTV, c);
}