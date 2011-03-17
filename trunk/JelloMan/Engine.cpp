//=======================================================================================
// d3dApp.cpp by Frank Luna (C) 2008 All Rights Reserved.
// Adapted for DAE GP1 by Bart Uyttenhove
//=======================================================================================
#include "Engine.h"

#include "MainGame.h"
#include "ContentManager.h"
#include "GameConfig.h"

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static Engine* app = 0;

	switch( msg )
	{
		case WM_CREATE:
		{
			// Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			app = (Engine*)cs->lpCreateParams;
			return 0;
		}
	}

	// Don't start processing messages until after WM_CREATE.
	if( app )
		return app->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

Engine::Engine(HINSTANCE hInstance) 
:m_hAppInst(hInstance)
,m_hMainWnd(0)
,m_AppPaused(false)
,m_Minimized(false)
,m_Maximized(false)
,m_Resizing(false)
,m_pGame(0)
,m_pDXDevice(0)
,m_pSwapChain(0)
,m_pDepthStencilBuffer(0)
,m_pRenderTargetView(0)
,m_pDepthStencilView(0)
,m_d3dDriverType( D3D10_DRIVER_TYPE_HARDWARE)
,m_ClearColor( D3DXCOLOR(0.0f, 0.0f, 0.4f, 1.0f))
,m_ClientWidth( 800)
,m_ClientHeight( 600)
,m_CurrentBackbufferWidth(0)
,m_CurrentBackbufferHeight(0)
,m_pContentManager(0)
,m_pD2DFactory(0)
,m_pBackBufferRT(0)
,m_pColorBrush(0)
,m_pDWriteFactory(0)
,m_pTextFormat(0)
,m_bInitialized(false)
{
	m_GameTimer.Reset();
}

Engine::~Engine()
{
	delete m_pGameConfig;

	delete CONTROLS;
	delete BLOX_2D;

	SafeDelete(m_pContentManager);

	if( m_pDXDevice )m_pDXDevice->ClearState();

	SafeRelease(m_pRenderTargetView);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pDepthStencilBuffer);
	SafeRelease(m_pDXDevice);
	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pBackBufferRT);
	SafeRelease(m_pColorBrush);
	SafeRelease(m_pDWriteFactory);
	SafeRelease(m_pTextFormat);
}

HINSTANCE Engine::GetAppInst()
{
	return m_hAppInst;
}

HWND Engine::GetMainWnd()
{
	return m_hMainWnd;
}

int Engine::Run()
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
				m_GameTimer.Tick();
				OnRender();

				if (!m_bInitialized)
				{
					m_pGame->LoadResources(m_pDXDevice);

					#if defined DEBUG || _DEBUG
					cout << "---------------------------\n";
                    cout << ":::Resources Initialized:::\n";
                    cout << "---------------------------\n";
                    cout << "\n";
                    cout << "\n";
					cout << "   GAME EVENTS   \n";
                    cout << "-----------------\n";
					#endif

					m_bInitialized = true;
				}
			}
			else
				Sleep(50);

        }
    }
	return static_cast<int>(msg.wParam);
}

void Engine::Initialize()
{
	// init main game
	m_pGameConfig = new GameConfig();
	m_pGame->Initialize(*m_pGameConfig);

	m_ClientWidth = static_cast<int>(m_pGameConfig->GetWindowSize().width);
	m_ClientHeight = static_cast<int>(m_pGameConfig->GetWindowSize().height);

	// init DirectX & Direct2D & open window
	CreateDeviceIndependentResources();
	InitMainWindow(m_pGameConfig->GetGameTitle());
	CreateDeviceResources();

	#if defined DEBUG || _DEBUG
	cout << "-Direct2D & DirectX initialized\n";
	#endif

	// init D2D engine
	BLOX_2D->SetParams(	m_pBackBufferRT,
						m_pD2DFactory,
						m_pDWriteFactory,
						m_pColorBrush,
						m_pTextFormat );

	#if defined DEBUG || _DEBUG
	cout << "-Blox2D Engine initialized\n";
	#endif
}

void Engine::OnRender()
{
	CreateDeviceResources();

	// clearing rendertarget for new frame
	//m_pDXDevice->ClearRenderTargetView(m_pRenderTargetView, m_ClearColor);
	//m_pDXDevice->ClearDepthStencilView(
     //               m_pDepthStencilView,
     //               D3D10_CLEAR_DEPTH,
     //               1,
     //               0
     //               );

	// main game cycle
	
	
	if (m_pBackBufferRT)
	{
		m_pBackBufferRT->BeginDraw();
		m_pGame->UpdateScene(m_GameTimer.GetDeltaTime());
		m_pGame->DrawScene();
		m_pBackBufferRT->EndDraw();
	}

	/*if (CONTROLS->LeftMBDown())
	{
		if (m_bMouseMoving)
		{
			POINT pt;
			pt.x = m_ClientWidth/2;
			pt.y = m_ClientHeight/2;
			ClientToScreen(GetMainWnd(),&pt);
			SetCursorPos(pt.x,pt.y);
			CONTROLS->SetOldMousePos(Point2F(m_ClientWidth/2,m_ClientHeight/2));
		}
	}*/

	// displaying backbuffer - vsync on
	m_pSwapChain->Present(1, 0);
}

LRESULT Engine::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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
			m_GameTimer.Stop();
		}
		else
		{
			m_AppPaused = false;
			m_GameTimer.Start();
		}
		return 0;

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_ClientWidth  = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);
		if( m_pDXDevice )
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
				RecreateSizedResources();
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				if( m_Minimized )
				{
					m_AppPaused = false;
					m_Minimized = false;
					RecreateSizedResources();
				}

				// Restoring from maximized state?
				else if( m_Maximized )
				{
					m_AppPaused = false;
					m_Maximized = false;
					RecreateSizedResources();
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
					RecreateSizedResources();
				}
			}
		}
		return 0;

	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing  = true;
		m_GameTimer.Stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing  = false;
		m_GameTimer.Start();
		RecreateSizedResources();
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
		CONTROLS->SetMousePos(Point2F(currentpos.x,currentpos.y));
		m_bMouseMoving = true;
		return 0;

	case WM_LBUTTONDOWN:
		CONTROLS->SetOldMousePos(CONTROLS->GetMousePos());
		CONTROLS->SetLeftMBDown(true);
		//ShowCursor(false);
		return 0;

	case WM_LBUTTONUP:
		CONTROLS->SetLeftMBClicked(true);
		CONTROLS->SetLeftMBDown(false);
		//ShowCursor(true);
		return 0;

	case WM_RBUTTONDOWN:
		CONTROLS->SetOldMousePos(CONTROLS->GetMousePos());
		CONTROLS->SetRightMBDown(true);
		return 0;

	case WM_RBUTTONUP:
		CONTROLS->SetRightMBClicked(true);
		CONTROLS->SetRightMBDown(false);
		return 0;

	case WM_MOUSEWHEEL:
		zPos = GET_WHEEL_DELTA_WPARAM(wParam);
		CONTROLS->SetMouseWheelPos(zPos);
		return 0;
	}
	m_bMouseMoving = false;
	return DefWindowProc(m_hMainWnd, msg, wParam, lParam);
}


void Engine::InitMainWindow(const tstring* pTitle)
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

	m_hMainWnd = CreateWindow(L"D3DWndClassName", pTitle->c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, this); 
	if( !m_hMainWnd )
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);
}

HRESULT Engine::CreateDeviceIndependentResources()
{
	static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 12;

    HRESULT hr;
    ID2D1GeometrySink *pSink = NULL;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

    if (SUCCEEDED(hr))
	{
        // Create a DirectWrite factory.
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_pDWriteFactory),
            reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
            );
    }
    if (SUCCEEDED(hr))
    {
        // Create a DirectWrite text format object.
        hr = m_pDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"", //locale
            &m_pTextFormat
            );
    }
    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    SafeRelease(pSink);

    return hr;
}

HRESULT Engine::CreateDeviceResources()
{
	HRESULT hr = S_OK;
    RECT rcClient;
    ID3D10Device1 *pDevice = NULL;
    IDXGIDevice *pDXGIDevice = NULL;
    IDXGIAdapter *pAdapter = NULL;
    IDXGIFactory *pDXGIFactory = NULL;
    IDXGISurface *pSurface = NULL;

    ASSERT(m_hMainWnd != 0);

    GetClientRect(m_hMainWnd, &rcClient);

    UINT nWidth = abs(rcClient.right - rcClient.left);
    UINT nHeight = abs(rcClient.bottom - rcClient.top);

    // If we don't have a device, need to create one now and all
    // accompanying D3D resources.
    if (!m_pDXDevice)
    {
        UINT nDeviceFlags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;
#if defined DEBUG || _DEBUG
        nDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
        // Create device
        hr = CreateD3DDevice(
            NULL,
            D3D10_DRIVER_TYPE_HARDWARE,
            nDeviceFlags,
			&pDevice
            );

		if (FAILED(hr))
        {
            hr = CreateD3DDevice(
                NULL,
                D3D10_DRIVER_TYPE_WARP,
                nDeviceFlags,
                &pDevice
                );
        }

        if (SUCCEEDED(hr))
        {
            hr = pDevice->QueryInterface(&m_pDXDevice);
        }
        if (SUCCEEDED(hr))
        {
            hr = pDevice->QueryInterface(&pDXGIDevice);
        }
        if (SUCCEEDED(hr))
        {
            hr = pDXGIDevice->GetAdapter(&pAdapter);
        }
        if (SUCCEEDED(hr))
        {
            hr = pAdapter->GetParent(IID_PPV_ARGS(&pDXGIFactory));
        }
        if (SUCCEEDED(hr))
        {
            DXGI_SWAP_CHAIN_DESC swapDesc;
            ::ZeroMemory(&swapDesc, sizeof(swapDesc));

            swapDesc.BufferDesc.Width = nWidth;
            swapDesc.BufferDesc.Height = nHeight;
            swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            swapDesc.BufferDesc.RefreshRate.Numerator = 60;
            swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	        swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	        swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
           
			swapDesc.SampleDesc.Count = 1;
            swapDesc.SampleDesc.Quality = 0;
           
			swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapDesc.BufferCount = 1;
            swapDesc.OutputWindow = m_hMainWnd;
            swapDesc.Windowed = TRUE;
	        swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	        swapDesc.Flags = 0;

            hr = pDXGIFactory->CreateSwapChain(m_pDXDevice, &swapDesc, &m_pSwapChain);
        }
        if (SUCCEEDED(hr))
        {
            hr = RecreateSizedResources();
			m_pGame->OnResize(m_pRenderTargetView);
        }
		if (SUCCEEDED(hr))
        {
            // Create a brush.
            hr = m_pBackBufferRT->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &m_pColorBrush
                );

			// anti-aliasing for direct2D
			if (m_pGameConfig->Blox2DAntiAliasing())
				m_pBackBufferRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			else
				m_pBackBufferRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        }
    }

    SafeRelease(pDevice);
    SafeRelease(pDXGIDevice);
    SafeRelease(pAdapter);
    SafeRelease(pDXGIFactory);
    SafeRelease(pSurface);

    return hr;
}

HRESULT Engine::RecreateSizedResources()
{
	m_pGame->Release();

	UINT nWidth = m_ClientWidth;
	UINT nHeight = m_ClientHeight;
    HRESULT hr = S_OK;
    IDXGISurface *pBackBuffer = NULL;
    ID3D10Resource *pBackBufferResource = NULL;
    ID3D10RenderTargetView *viewList[1] = {NULL};

    // Ensure that nobody is holding onto one of the old resources
    SafeRelease(m_pBackBufferRT);
    SafeRelease(m_pRenderTargetView);
    m_pDXDevice->OMSetRenderTargets(1, viewList, NULL);

    // Resize render target buffers
    hr = m_pSwapChain->ResizeBuffers(1, nWidth, nHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

    if (SUCCEEDED(hr))
    {
        D3D10_TEXTURE2D_DESC texDesc;
        texDesc.ArraySize = 1;
        texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;
        texDesc.Format = DXGI_FORMAT_D16_UNORM;
        texDesc.Height = nHeight;
        texDesc.Width = nWidth;
        texDesc.MipLevels = 1;
        texDesc.MiscFlags = 0;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D10_USAGE_DEFAULT;

		SafeRelease(m_pDepthStencilBuffer);
		hr = m_pDXDevice->CreateTexture2D(&texDesc, NULL, &m_pDepthStencilBuffer);
    }

    if (SUCCEEDED(hr))
    {
        // Create the render target view and set it on the device
        hr = m_pSwapChain->GetBuffer(
            0,
            IID_PPV_ARGS(&pBackBufferResource)
            );
    }
    if (SUCCEEDED(hr))
    {
        D3D10_RENDER_TARGET_VIEW_DESC renderDesc;
        renderDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        renderDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
        renderDesc.Texture2D.MipSlice = 0;

        SafeRelease(m_pRenderTargetView);
        hr = m_pDXDevice->CreateRenderTargetView(pBackBufferResource, &renderDesc, &m_pRenderTargetView);
    }
    if (SUCCEEDED(hr))
    {
        D3D10_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
        depthViewDesc.Format = DXGI_FORMAT_D16_UNORM;
        depthViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
        depthViewDesc.Texture2D.MipSlice = 0;

        SafeRelease(m_pDepthStencilView);
		hr = m_pDXDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthViewDesc, &m_pDepthStencilView);
    }
    if (SUCCEEDED(hr))
    {
        viewList[0] = m_pRenderTargetView;
        m_pDXDevice->OMSetRenderTargets(1, viewList, m_pDepthStencilView);

        // Set a new viewport based on the new dimensions
        D3D10_VIEWPORT viewport;
        viewport.Width = nWidth;
        viewport.Height = nHeight;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        m_pDXDevice->RSSetViewports(1, &viewport);

        // Get a surface in the swap chain
        hr = m_pSwapChain->GetBuffer(
            0,
            IID_PPV_ARGS(&pBackBuffer)
            );
    }

    if (SUCCEEDED(hr))
    {
        // Create the DXGI Surface Render Target.
        FLOAT dpiX = 96;
        FLOAT dpiY = 96;
        //m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

        D2D1_RENDER_TARGET_PROPERTIES props =
            D2D1::RenderTargetProperties(
                D2D1_RENDER_TARGET_TYPE_DEFAULT,
                D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
                dpiX,
                dpiY
                );

        // Create a Direct2D render target which can draw into the surface in the swap chain
        SafeRelease(m_pBackBufferRT);
        hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(
            pBackBuffer,
            &props,
            &m_pBackBufferRT
            );

		// anti-aliasing for direct2D
		if (m_pGameConfig->Blox2DAntiAliasing())
			m_pBackBufferRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		else
			m_pBackBufferRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		// sending new rendertarget to Blox2D
		BLOX_2D->OnResize(m_pBackBufferRT);
		m_pGame->OnResize(m_pRenderTargetView);
    }

    SafeRelease(pBackBuffer);
    SafeRelease(pBackBufferResource);

    return hr;
}


HRESULT Engine::CreateD3DDevice(
    IDXGIAdapter *pAdapter,
    D3D10_DRIVER_TYPE driverType,
    UINT flags,
    ID3D10Device1 **ppDevice
    )
{
    HRESULT hr = S_OK;

    static const D3D10_FEATURE_LEVEL1 levelAttempts[] =
    {
        D3D10_FEATURE_LEVEL_10_0,
        D3D10_FEATURE_LEVEL_9_3,
        D3D10_FEATURE_LEVEL_9_2,
        D3D10_FEATURE_LEVEL_9_1,
    };

    for (UINT level = 0; level < ARRAYSIZE(levelAttempts); level++)
    {
        ID3D10Device1 *pDevice = NULL;
        hr = D3D10CreateDevice1(
            pAdapter,
            driverType,
            NULL,
            flags,
            D3D10_FEATURE_LEVEL_10_0,
            D3D10_1_SDK_VERSION,
            &pDevice
            );

        if (SUCCEEDED(hr))
        {
            // transfer reference
            *ppDevice = pDevice;
            pDevice = NULL;
			break;
        }

    }

    return hr;
}