#include "DeferredRenderer.h"
#include "Vector4.h"
#include "ContentManager.h"
#include "vertex.h"
#include <vector>
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"

DeferredRenderer::DeferredRenderer(ID3D10Device* device):
    m_pDepthDSV(0),
    m_pDevice(device),
	m_pScreenMesh(new ModelMesh<VertexPosTex>(device, _T("screenMesh"))),
	m_pEffect(0),
    m_Width(0), m_Height(0),
    m_pPrevBackbuffer(0),
    m_pPrevDepthStencil(0)
{
    ZeroMemory(&m_Viewport, sizeof(D3D10_VIEWPORT));
    Vector4(0.f, 0.f, 0.f, 0.f).ToFloat4(m_ClearColor);

    for (int i = 0; i < MAXRENDERTARGETS; ++i)
        m_RenderTargets[i] = 0;
    for (int i = 0; i < MAXRENDERTARGETS + 1; ++i)
        m_pSRV[i] = 0;

	m_LightMode = LIGHT_MODE_LIT;
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

void DeferredRenderer::Init(UINT width, UINT height)
{
    OnResized(width, height);

	m_pEffect = ContentManager::GetSingleton()->LoadEffect<DeferredPostEffect>(_T("postdeferred.fx"));

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


void DeferredRenderer::OnResize()
{
    for (int i = 0; i < MAXRENDERTARGETS; ++i)
        SafeRelease(m_RenderTargets[i]); 
    for (int i = 0; i < MAXRENDERTARGETS + 1; ++i)
        SafeRelease(m_pSRV[i]);
    SafeRelease(m_pDepthDSV);
    m_pPrevBackbuffer = 0;
}
void DeferredRenderer::OnResized(UINT width, UINT height)
{
    ASSERT(m_pDepthDSV == 0 && m_RenderTargets[0] == 0 && m_pSRV[0] == 0, "OnResize() must be called first!");

    m_Width = width;
    m_Height = height;
    
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = m_Width;
    m_Viewport.Height = m_Height;
    m_Viewport.MinDepth = 0.f;
    m_Viewport.MaxDepth = 1.f;

    CreateColorMap(DeferredRenderMap_Color, DXGI_FORMAT_R8G8B8A8_UNORM); //R G B A
	CreateColorMap(DeferredRenderMap_Normal, DXGI_FORMAT_R16G16B16A16_FLOAT); //X Y Z Spec
    CreateColorMap(DeferredRenderMap_Position, DXGI_FORMAT_R16G16B16A16_FLOAT); //X Y Z gloss

    CreateDepthMap();
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
    return m_pPrevBackbuffer;
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


void DeferredRenderer::Begin()
{ 
    m_pDevice->OMGetRenderTargets(1, &m_pPrevBackbuffer, &m_pPrevDepthStencil);

    m_pDevice->OMSetRenderTargets(MAXRENDERTARGETS, m_RenderTargets, m_pDepthDSV);
    m_pDevice->RSSetViewports(1, &m_Viewport);

    m_pDevice->ClearDepthStencilView(m_pDepthDSV, D3D10_CLEAR_DEPTH, 1.0f, 0);

    float c[4] = { 0, 0, 0, 1 };
    for (int i = 0; i < MAXRENDERTARGETS; ++i)
        m_pDevice->ClearRenderTargetView(m_RenderTargets[i], c);
}

inline bool CullLight(Light* pLight, Camera* pCam)
{
    bool cull = false;
    Vector3 vec = pCam->GetPosition() - pLight->GetPosition();
    float len = vec.Length();
    cull |= len > 10000;
    vec /= len;
    cull |= len > pLight->GetAttenuationEnd() && vec.Dot(pCam->GetLook()) > 0;
    return cull;
}
void DeferredRenderer::End(const RenderContext* pRenderContext)
{ 
    ASSERT(m_pPrevBackbuffer != 0, "Begin has not been called or backbuffer got lost");
    ASSERT(m_pDepthDSV != 0, "");
    m_pDevice->OMSetRenderTargets(1, &m_pPrevBackbuffer, NULL); //depth = 0, no depthbuffer needed in postprocessing
    m_pDevice->RSSetViewports(1, &m_Viewport);

    //Clear Backbuffer
    float c[4] = { 0, 0, 0, 1 };
    m_pDevice->ClearRenderTargetView(m_pPrevBackbuffer, c);

    //Set vars needed for UNLIT && LIT
	m_pEffect->SetColorMap(m_pSRV[DeferredRenderMap_Color]);
    m_pEffect->SetCameraPosition(pRenderContext->GetCamera()->GetPosition());	
	
    m_pScreenMesh->SetIA(m_pEffect);

#pragma region LightMode LIT
	if(m_LightMode == LIGHT_MODE_LIT)
	{
        //Set vars needed for LIT
	    m_pEffect->SetNormalSpecMap(m_pSRV[DeferredRenderMap_Normal]);
	    m_pEffect->SetPosGlossMap(m_pSRV[DeferredRenderMap_Position]);
      
        //Loop Lights
	    for_each(pRenderContext->GetLightController()->GetLights().cbegin(), 
                 pRenderContext->GetLightController()->GetLights().cend(), 
                 [&](Light* pLight)
	    {
            //Check if on
			if (pLight->IsEnabled() == true && CullLight(pLight, pRenderContext->GetCamera()) == false)
            {
                //PointLight
                #pragma region
                if (pLight->GetType() == LightType_Point)
                {
                    PointLight* pl = dynamic_cast<PointLight*>(pLight);

			        m_pEffect->SetTechnique("tech_PointLightNoShadows");
                
		            m_pEffect->SetPointLight(pl->GetDesc());
                }
                #pragma endregion
                //SpotLight
                #pragma region
                else if (pLight->GetType() == LightType_Spot)
                {
                    SpotLight* sl = dynamic_cast<SpotLight*>(pLight);

                    if (sl->HasShadowMap())
			            m_pEffect->SetTechnique("tech_SpotLightShadows");
                    else
			            m_pEffect->SetTechnique("tech_SpotLightNoShadows");

		            m_pEffect->SetSpotLight(sl->GetDesc());
                }
                #pragma endregion
                else
                    ASSERT(false, "Lighttype not supported");
            
                if (pLight->HasShadowMap() == true)
                {
                    m_pEffect->SetShadowMap(pLight->GetShadowMap()->GetDepthMap());
                    m_pEffect->SetShadowWVP(pLight->GetShadowCamera()->GetViewProjection());
                    m_pEffect->SetShadowMapType(pLight->GetShadowMapType());
                }

                D3D10_RECT scissorRect = pLight->CalcScissorRect(pRenderContext->GetCamera(), m_Viewport.Width, m_Viewport.Height);
                //Calc scissor rect
		        BX2D->SetColor(255,255,255,1.0f);
		        BX2D->DrawRect((float)scissorRect.left, (float)scissorRect.top, (float)(scissorRect.right - scissorRect.left), (float)(scissorRect.bottom - scissorRect.top), 1.0f);

		        m_pDevice->RSSetScissorRects(1, &scissorRect);

			    m_pEffect->GetCurrentTechnique()->GetPassByIndex(0)->Apply(0);	
			    m_pDevice->DrawIndexed(6, 0, 0);
            }
	    });

	    m_pDevice->RSSetScissorRects(0, NULL);
	}
#pragma endregion
#pragma region LightMode UNLIT
	else
	{
		m_pEffect->SetTechnique("tech_UNLIT");
        m_pScreenMesh->Draw(m_pEffect);
	}
#pragma endregion

    ID3D10RenderTargetView* targets[MAXRENDERTARGETS] = { m_pPrevBackbuffer, 0, 0 };
    m_pDevice->OMSetRenderTargets(MAXRENDERTARGETS, targets, m_pPrevDepthStencil);

    SafeRelease(m_pPrevBackbuffer);
    SafeRelease(m_pPrevDepthStencil);

    m_pPrevBackbuffer = 0;
    m_pPrevDepthStencil = 0;
	m_pEffect->SetColorMap(0);
	m_pEffect->SetNormalSpecMap(0);
	m_pEffect->SetPosGlossMap(0);
    m_pEffect->SetShadowMap(0);
	m_pEffect->GetCurrentTechnique()->GetPassByIndex(0)->Apply(0); //unbind rendertargets
}

void DeferredRenderer::SetLightMode(LightMode lMode)
{
	m_LightMode = lMode;
}