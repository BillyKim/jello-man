#pragma once
#include "D3DUtil.h"
#include "Texture2D.h"
#include "ModelMesh.h"
#include "vertex.h"
#include "DeferredRenderer.h"
#include "FluidEffect.h"
#include "FluidPostEffect.h"

class FluidRenderer
{
public:
    FluidRenderer(ID3D10Device* pDXDevice, int backBufferWidth, int backBufferHeight);
    ~FluidRenderer(void);

    void OnResized(int backBufferWidth, int backBufferHeight);

    void Begin();
    void End(const RenderContext* pRenderContext);

private:
    void Init();

    ID3D10Device* m_pDXDevice;

    Texture2D* m_pBuffer;
    int m_Width, m_Height;
    FluidPostEffect* m_pPostEffect;

    ModelMesh<VertexPosTex>* m_pScreenMesh;

    ID3D10RenderTargetView* m_pPrevBackBuffer;
    ID3D10DepthStencilView* m_pPrevDepthStencilView;
};

