#pragma once
#include "D3DUtil.h"
#include "Vector4.h"

class Texture2D
{
public:
	Texture2D(ID3D10Device* device, ID3D10Resource* pTex);
    Texture2D(ID3D10Device* device, UINT width, UINT height, bool hasColormap = true, bool mips = true, DXGI_FORMAT colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM);

	virtual ~Texture2D(void);

    ID3D10Texture2D* GetTextureResource() const { return m_pTex2D; }
    ID3D10ShaderResourceView* GetColorMap() const { return m_pColorMapSRV; }
    ID3D10ShaderResourceView* GetDepthMap() const { return m_pDepthMapSRV; }

    UINT GetWidth() const { return m_Width; }
    UINT GetHeight() const { return m_Height; }

    void BeginDraw();
    void EndDraw();

    void Clear(const Vector4& color);

private:
    void BuildColorMap();
    void BuildDepthMap();

    UINT m_Width;
    UINT m_Height;
    DXGI_FORMAT m_ColorFormat;
    ID3D10Device* m_pDevice;

    bool m_MakeMips;

    ID3D10ShaderResourceView* m_pColorMapSRV;
    ID3D10RenderTargetView* m_pColorMapRTV;

    ID3D10ShaderResourceView* m_pDepthMapSRV;
    ID3D10DepthStencilView* m_pDepthMapDSV;

    ID3D10Texture2D* m_pTex2D;

    ID3D10RenderTargetView* m_pPrevRenderTarget;
    ID3D10DepthStencilView* m_pPrevDSV;
    D3D10_VIEWPORT m_PrevViewport;

    D3D10_VIEWPORT m_Viewport;
};

