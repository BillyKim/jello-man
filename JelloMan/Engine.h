//=======================================================================================
// d3dApp.h by Frank Luna (C) 2008 All Rights Reserved.

// Simple Direct3D demo application class.  
// Make sure you link: d3d10.lib d3dx10d.lib dxerr.lib dxguid.lib.
// Link d3dx10.lib for release mode builds instead of d3dx10d.lib.
//
// Adapted for DAE GP1 by Bart Uyttenhove
//=======================================================================================

#pragma once

#include "d3dUtil.h"
#include "GameTimer.h"

class KeyboardState;
class InputStateManager;
class ContentManager;
class MainGame;
class GameConfig;
class Blox2D;
class Controls;

class Engine
{
public:
	Engine(HINSTANCE hInstance);
	virtual ~Engine();

	HINSTANCE GetAppInst();
	HWND      GetMainWnd();

	int Run();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void Initialize();
	void OnRender();

	void SetGame(MainGame* pGame) { m_pGame = pGame; }

private:
	void InitMainWindow(const tstring* pTitle);

	HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    HRESULT RecreateSizedResources();

	HRESULT CreateD3DDevice(
    IDXGIAdapter *pAdapter,
    D3D10_DRIVER_TYPE driverType,
    UINT flags,
    ID3D10Device1 **ppDevice
    );

	HINSTANCE m_hAppInst;
	HWND      m_hMainWnd;
	bool      m_AppPaused;
	bool      m_Minimized;
	bool      m_Maximized;
	bool      m_Resizing;

	MainGame* m_pGame;
 
	ID3D10Device*    m_pD3DDevice;
	IDXGISwapChain*  m_pSwapChain;
	ID3D10Texture2D* m_pDepthStencilBuffer;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;

	Controls* m_pControls;

	D3D10_DRIVER_TYPE m_d3dDriverType;
	D3DXCOLOR m_ClearColor;

	//holds the client size of the window, changes when draging or resizing the window
	int m_ClientWidth;
	int m_ClientHeight;

	//holds the size of the current backbuffer
	int m_CurrentBackbufferWidth;
	int m_CurrentBackbufferHeight;

	ContentManager *m_pContentManager;

	// Direct2D
	ID2D1Factory* m_pD2DFactory;
    ID2D1RenderTarget* m_pBackBufferRT;
	ID2D1SolidColorBrush* m_pColorBrush;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	GameConfig* m_pGameConfig;
	Blox2D* m_pBlox2D;

	GameTimer m_GameTimer;

private:
	Engine(const Engine& t);
	Engine& operator=(const Engine& t);
};
