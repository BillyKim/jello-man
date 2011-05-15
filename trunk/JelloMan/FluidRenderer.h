#pragma once
#include "D3DUtil.h"
#include "Texture2D.h"
#include "ModelMesh.h"
#include "vertex.h"
#include "PostProcessEffect.h"
#include "DeferredRenderer.h"

class FluidRenderer
{
public:
    FluidRenderer(ID3D10Device* pDXDevice, int backBufferWidth, int backBufferHeight);
    ~FluidRenderer(void);

    void SetEffect(PostProcessEffect* pEffect);

    void OnResized(int backBufferWidth, int backBufferHeight);

    void Begin();
    void End();

private:
    void Init();

    ID3D10Device* m_pDXDevice;

    Texture2D* m_pBuffer;
    int m_Width, m_Height;
    PostProcessEffect* m_pEffect;

    ModelMesh<VertexPosTex>* m_pScreenMesh;

    ID3D10RenderTargetView* m_pPrevBackBuffer;
    ID3D10DepthStencilView* m_pPrevDepthStencilView;
};

