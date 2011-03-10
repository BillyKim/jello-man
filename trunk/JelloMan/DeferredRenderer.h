#pragma once
#include "D3DUtil.h"

enum DeferredRenderMap
{
    DeferredRenderMap_Color     = 0,
    DeferredRenderMap_Normal    = 1,
    DeferredRenderMap_Position  = 2,
    DeferredRenderMap_Depth     = 3
};
class DeferredRenderer
{
public:
    DeferredRenderer(ID3D10Device* device);
    virtual ~DeferredRenderer(void);

    void Init(UINT width, UINT height);

    void Begin();
    void End();

private:
    UINT m_Width, m_Height;

    void CreateColorMap(DeferredRenderMap map, DXGI_FORMAT format);
    void CreateDepthMap();

    ID3D10Device* m_pDevice;
    
    static const int MAXRENDERTARGETS = 3;
    ID3D10ShaderResourceView* m_pSRV[MAXRENDERTARGETS + 1];    //Color, Normal(+spec), Position(+gloss), Depth
    ID3D10RenderTargetView* m_RenderTargets[MAXRENDERTARGETS]; //Color, Normal(+spec), Position(+gloss)

    ID3D10DepthStencilView* m_pDepthDSV;

    D3D10_VIEWPORT m_Viewport;
    
    float m_ClearColor[4];
};



