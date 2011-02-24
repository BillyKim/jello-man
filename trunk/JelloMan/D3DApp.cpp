//=======================================================================================
// d3dApp.cpp by Frank Luna (C) 2008 All Rights Reserved.
// Adapted for DAE GP1 by Bart Uyttenhove
//=======================================================================================
#include "StdAfx.h"
#include "d3dApp.h"
#include <sstream>
#include "ContentManager.h"
#include "InputStateManager.h"

//#include "ObjMesh.h"
//#include "IndexedGeometry.h"
//#include "IndexBufferMaterial.h"
//#include "VertexList.h"


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static D3DApp* app = 0;

	switch( msg )
	{
		case WM_CREATE:
		{
			// Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			app = (D3DApp*)cs->lpCreateParams;
			return 0;
		}
	}

	// Don't start processing messages until after WM_CREATE.
	if( app )
		return app->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance) 
:m_hAppInst(hInstance)
,m_hMainWnd(0)
,m_AppPaused(false)
,m_Minimized(false)
,m_Maximized(false)
,m_Resizing(false)
,m_FrameStats(L"")
,m_pD3DDevice(0)
,m_pSwapChain(0)
,m_pDepthStencilBuffer(0)
,m_pRenderTargetView(0)
,m_pDepthStencilView(0)
,m_pFont(0)
,m_pInputStateManager(0)
,m_MainWndCaption( L"The Happy Engine")
,m_d3dDriverType( D3D10_DRIVER_TYPE_HARDWARE)
,m_ClearColor( D3DXCOLOR(0.0f, 0.0f, 0.2f, 1.0f))
,m_ClientWidth( 800)
,m_ClientHeight( 600)
,m_CurrentBackbufferWidth(0)
,m_CurrentBackbufferHeight(0)
,m_pContentManager(0)
{
}

D3DApp::~D3DApp()
{	
	SafeDelete(m_pInputStateManager);
	SafeDelete(m_pContentManager);

	if( m_pD3DDevice )m_pD3DDevice->ClearState();

	SafeRelease(m_pRenderTargetView);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pDepthStencilBuffer);
	SafeRelease(m_pFont);
	SafeRelease(m_pD3DDevice);
}

HINSTANCE D3DApp::GetAppInst()
{
	return m_hAppInst;
}

HWND D3DApp::GetMainWnd()
{
	return m_hMainWnd;
}

int D3DApp::Run()
{
	MSG msg = {0};
 
	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			if( !m_AppPaused )
			{
				InputState refInputState =  m_pInputStateManager->GenerateInputState();
				//Get InputState once per frame
				UpdateScene(refInputState);	
				ClearRenderTargets();
				DrawScene();
				PresentScene();
			}
			else
				Sleep(50);

        }
    }
	return (int)msg.wParam;
}

void D3DApp::InitApp()
{
	//InputStateManager holds input data from keyboard and mouse
	//create this first, windows messages need it to set mouse state
	//Contentmanager loads effects, textures
	m_pContentManager = new ContentManager();
	m_pInputStateManager = new InputStateManager();
	InitMainWindow();
	InitializeDirectX();
	
	

	CreateFont();

}

void D3DApp::UpdateScene(const InputState & refInputState)
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.

	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (m_pInputStateManager->GetGameTime() - t_base) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << L"FPS: " << fps << L"\n" 
			 << "Milliseconds: Per Frame: " << mspf;
		m_FrameStats = outs.str();
		
		// Reset for next average.
		frameCnt = 0;
		t_base  += 1.0f;
	}
}

void D3DApp::ClearRenderTargets()
{
	m_pD3DDevice->ClearRenderTargetView(m_pRenderTargetView, m_ClearColor);
	m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
}

void D3DApp::DrawScene()
{
	//DrawString(m_pFont,  m_FrameStats.c_str(), 5,5);
}

void D3DApp::PresentScene()
{
	// swap front and backbuffer: Display next rendered frame
	m_pSwapChain->Present(0, 0);
}

LRESULT D3DApp::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINTS currentpos;
	short zPos=0;

	switch( msg )
	{
	// WM_ACTIVATE is sent when the window is activated or deactivated.  
	// We pause the game when the window is deactivated and unpause it 
	// when it becomes active.  
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			m_AppPaused = true;
			m_pInputStateManager->Stop();
		}
		else
		{
			m_AppPaused = false;
			m_pInputStateManager->Start();
		}
		return 0;

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_ClientWidth  = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);
		if( m_pD3DDevice )
		{
			if( wParam == SIZE_MINIMIZED )
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				OnResize();
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				if( m_Minimized )
				{
					m_AppPaused = false;
					m_Minimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if( m_Maximized )
				{
					m_AppPaused = false;
					m_Maximized = false;
					OnResize();
				}
				else if( m_Resizing )
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing  = true;
		m_pInputStateManager->Stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing  = false;
		m_pInputStateManager->Start();
		OnResize();
		return 0;
 
	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
	// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);

	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
		return 0;

	//mousemoves for input state
	case WM_MOUSEMOVE:
		currentpos = MAKEPOINTS(lParam);
		m_pInputStateManager->SetMousePos(currentpos);
		return 0;

	case WM_LBUTTONDOWN:
		m_pInputStateManager->SetLMousePressed(true);
		return 0;

	case WM_LBUTTONUP:
		m_pInputStateManager->SetLMousePressed(false);
		return 0;

	case WM_RBUTTONDOWN:
		m_pInputStateManager->SetRMousePressed(true);
		return 0;

	case WM_RBUTTONUP:
		m_pInputStateManager->SetRMousePressed(false);
		return 0;

	case WM_MOUSEWHEEL:
		zPos = GET_WHEEL_DELTA_WPARAM(wParam);
		m_pInputStateManager->SetWheelPos(zPos);
		return 0;
	}
	return DefWindowProc(m_hMainWnd, msg, wParam, lParam);
}


void D3DApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hAppInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	m_hMainWnd = CreateWindow(L"D3DWndClassName", m_MainWndCaption.c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, this); 
	if( !m_hMainWnd )
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);
}

void D3DApp::InitializeDirectX()
{
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = m_ClientWidth;
	sd.BufferDesc.Height = m_ClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// No multisampling.
	sd.SampleDesc.Count   = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = m_hMainWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;


	// Create the device.
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	HR( D3D10CreateDeviceAndSwapChain(
			0,                 //default adapter
			m_d3dDriverType,
			0,                 // no software device
			createDeviceFlags, 
			D3D10_SDK_VERSION,
			&sd,
			&m_pSwapChain,
			&m_pD3DDevice) );


	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the onResize method here to avoid code duplication.
	
	OnResize();
}

void D3DApp::OnResize()
{
	//only if client size has changed
	if((m_CurrentBackbufferWidth != m_ClientWidth)||(m_CurrentBackbufferHeight!=m_ClientHeight))
	{
		m_CurrentBackbufferWidth = m_ClientWidth;
		m_CurrentBackbufferHeight=m_ClientHeight;
		// Release the old views, as they hold references to the buffers we
		// will be destroying.  Also release the old depth/stencil buffer.
		SafeRelease(m_pRenderTargetView);
		SafeRelease(m_pDepthStencilView);
		SafeRelease(m_pDepthStencilBuffer);

		// Resize the swap chain and recreate the render target view.
		HR(m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		ID3D10Texture2D* pBackBuffer=0;
		HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
		HR(m_pD3DDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView));
		SafeRelease(pBackBuffer);

		// Create the depth/stencil buffer and view.
		D3D10_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width     = m_ClientWidth;
		depthStencilDesc.Height    = m_ClientHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
		depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
		depthStencilDesc.Usage          = D3D10_USAGE_DEFAULT;
		depthStencilDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0; 
		depthStencilDesc.MiscFlags      = 0;

		HR(m_pD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer));
		HR(m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));

		// Bind the render target view and depth/stencil view to the pipeline.
		m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	

		// Set the viewport transform.
		D3D10_VIEWPORT vp;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width    = m_ClientWidth;
		vp.Height   = m_ClientHeight;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pD3DDevice->RSSetViewports(1, &vp);
	}
}
void D3DApp::CreateFont()
{
	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    wcscpy_s(fontDesc.FaceName,32,L"Times New Roman");

	D3DX10CreateFontIndirect(m_pD3DDevice, &fontDesc, &m_pFont);
}
void D3DApp::DrawString(ID3DX10Font* pFont, const tstring& Text, int xpos, int ypos)const
{
	RECT r = {xpos, ypos, 0, 0};
	if(pFont) pFont->DrawText(0, Text.c_str(), -1, &r, DT_NOCLIP, D3DXCOLOR(255, 255, 202, 255));
}