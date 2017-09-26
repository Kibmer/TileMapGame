#pragma once

#include <Windows.h>
#include <d3dx11.h>

class GameSystem {
private:
	static GameSystem* _instance;

public:
	static GameSystem& GetInstance();

private:
	GameSystem();		// 다른 곳에서 실수로 생성하지 않도록, 생성자를 프라이빗으로 만든다.

public:
	~GameSystem();

public:
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	bool InitDirect3D();
	int Update();

private:
	ID3D11Device* _d3dDevice;
	ID3D11DeviceContext* _d3dDeviceContext;
	
	IDXGISwapChain* _swapChain;

	ID3D11RenderTargetView* _renderTargetView;
	
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	
	D3D11_VIEWPORT _screenViewport;

	UINT _4xMsaaQuallity;
	
	bool _isEnable4xMsaa;
	HWND _hMainWnd;
};