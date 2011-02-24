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
#include <string>

class InputState;
class InputStateManager;
class DX10ObjMesh;
class ObjMesh;
class VertexList;
class DX10MaterialIndexBuffer;
class IndexBufferMaterial;
class ContentManager;


class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE GetAppInst();
	HWND      GetMainWnd();

	int Run();
	virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void DrawString(ID3DX10Font* pFont, const tstring& Text, int xpos, int ypos)const;

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.
	virtual void InitApp();
	virtual void OnResize();// reset projection/etc
	virtual void UpdateScene(const InputState & pInputState);
	virtual void DrawScene();

protected:
	void InitMainWindow();
	void InitializeDirectX();
	void CreateFont();
	void ClearRenderTargets();
	void PresentScene();

	HINSTANCE m_hAppInst;
	HWND      m_hMainWnd;
	bool      m_AppPaused;
	bool      m_Minimized;
	bool      m_Maximized;
	bool      m_Resizing;

	std::wstring m_FrameStats;
 
	ID3D10Device*    m_pD3DDevice;
	IDXGISwapChain*  m_pSwapChain;
	ID3D10Texture2D* m_pDepthStencilBuffer;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
	ID3DX10Font* m_pFont;

	InputStateManager *m_pInputStateManager;

	// Derived class should set these in derived constructor to customize starting values.
	std::wstring m_MainWndCaption;
	D3D10_DRIVER_TYPE m_d3dDriverType;
	D3DXCOLOR m_ClearColor;

	//holds the client size of the window, changes when draging or resizing the window
	int m_ClientWidth;
	int m_ClientHeight;

	//holds the size of the current backbuffer
	int m_CurrentBackbufferWidth;
	int m_CurrentBackbufferHeight;

	ContentManager *m_pContentManager;

private:
	D3DApp(const D3DApp& t);
	D3DApp& operator=(const D3DApp& t);
};
