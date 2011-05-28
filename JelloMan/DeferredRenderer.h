#pragma once
#include "D3DUtil.h"
#include "DeferredPostEffect.h"
#include "vertex.h"
#include "ModelMesh.h"
#include "Matrix.h"
#include "RenderContext.h"

enum DeferredRenderMap
{
    DeferredRenderMap_Color     = 0,
    DeferredRenderMap_Normal    = 1,
    DeferredRenderMap_Position  = 2,
    DeferredRenderMap_Depth     = 3
};

enum LightMode
{
	LIGHT_MODE_LIT = 0,
	LIGHT_MODE_UNLIT = 1
};
class DeferredRenderer
{
public:
    DeferredRenderer(ID3D10Device* device);
    virtual ~DeferredRenderer(void);

    void Init(UINT width, UINT height);

    void OnResize();
    void OnResized(UINT width, UINT height);

    void Begin();
    void End(const RenderContext* pRenderContext);

    UINT GetBackbufferWidth() const;
    UINT GetBackbufferHeight() const;
    ID3D10RenderTargetView* GetBackbuffer() const;
    ID3D10DepthStencilView* GetDepthbuffer() const;

    void SetClearColor(const Vector4& color);
	void SetLightMode(LightMode lMode);
    
    ID3D10ShaderResourceView* GetColorGlowMap() const;
    ID3D10ShaderResourceView* GetNormalSpecMap() const;
    ID3D10ShaderResourceView* GetPositionGlossMap() const;
    ID3D10ShaderResourceView* GetDepthMap() const;

private:

    void CreateColorMap(DeferredRenderMap map, DXGI_FORMAT format);
    void CreateDepthMap();

    UINT m_Width, m_Height;

    ID3D10Device* m_pDevice;
    ID3D10RenderTargetView* m_pPrevBackbuffer;
    ID3D10DepthStencilView* m_pPrevDepthStencil;
    
    static const int MAXRENDERTARGETS = 3;
    ID3D10ShaderResourceView* m_pSRV[MAXRENDERTARGETS + 1];    //Color, Normal(+spec), Position(+gloss), Depth
    ID3D10RenderTargetView* m_RenderTargets[MAXRENDERTARGETS]; //Color, Normal(+spec), Position(+gloss)

    ID3D10DepthStencilView* m_pDepthDSV;

    D3D10_VIEWPORT m_Viewport;
    
    float m_ClearColor[4];

	ModelMesh<VertexPosTex>* m_pScreenMesh;
	DeferredPostEffect* m_pEffect;

    Matrix m_mtxOrthoProjection;

	LightMode m_LightMode;
};



