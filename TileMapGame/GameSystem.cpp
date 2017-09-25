#include "GameSystem.h"

// 첫번째 윈도우 핸들은 현제 나의 윈도우의 핸들이 넘어 오지 않을때가 있다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World!", L"Hello", MB_OK);

	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam) {

			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lparam);
}

GameSystem* GameSystem::_instance = NULL;

GameSystem::GameSystem(){
	_isEnable4xMsaa = false;
}

GameSystem::~GameSystem(){
	_swapChain->Release();
	_swapChain = 0;

	_d3dDeviceContext->Release();
	_d3dDeviceContext = 0;

	_d3dDevice->Release();
	_d3dDevice = 0;

	_renderTargetView->Release();
	_renderTargetView = 0;
}

// Singleton
GameSystem& GameSystem::GetInstance(){
	if (NULL == _instance)
	{
		_instance = new GameSystem();
	}
	return *_instance;
}

bool GameSystem::InitSystem(HINSTANCE hInstance, int nCmdShow) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0; //추가 메모리 슬롯
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Base";

	if (!RegisterClass(&wc)) {
		return false;
	}

	_hMainWnd = CreateWindow(L"Base", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);

	ShowWindow(_hMainWnd, nCmdShow);
	UpdateWindow(_hMainWnd);

	return true;
}

int GameSystem::Update() {
	MSG msg = { 0 };
	//int ret = 1;
	////메세지를 받는다. 하지만 메세지가 없을 경우 GetMessage 함수에서 나오지 않는다.
	//while(ret = GetMessage(&msg, 0, 0, 0)){
	////	이벤트의 유무에 따른것이 아니라 -1이 온것은 
	////	더 큰문제가 있기 때문에 앱을 종료하라는 뜻이다
	//	if (-1 == ret) {
	//	break;
	//	}
	//	else {
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//	}
	//}

	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			//윈도우 처리
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//게임 처리

		}
	}
	return (int)msg.wParam; //문제가 있는지 없는지를 메세지에서 받는다.
}

bool GameSystem::InitDirect3D() {
	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		0, 
		0,
		D3D11_SDK_VERSION,
		&_d3dDevice,
		&featureLevel,
		&_d3dDeviceContext
	);

	if (FAILED(hr)) {
		MessageBox(0, L"지원하지 않는 하드웨어", L"Error", MB_OK);
		return false;
	}

	hr = _d3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&_4xMsaaQuallity
	);

	if (FAILED(hr)) {
		MessageBox(0, L"지원하지 않는 하드웨어", L"Error", MB_OK);
		return false;
	}

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 1280;
	sd.BufferDesc.Height = 800;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	if (_isEnable4xMsaa) {
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = _4xMsaaQuallity - 1;
	}
	else {
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼의 용도를 서술하는 구조체, 버퍼를 랜더를 대상으로 
	sd.BufferCount = 1; // 교환 사슬에서 사용할 후면 버퍼의 갯수, 더블 버퍼링에서는 하나만 사용. 트리플 버퍼
	sd.OutputWindow = _hMainWnd; // 렌더링 결과를 표시할 창의 핸들
	sd.Windowed = true; // 윈도우 모드/전체 모드
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 교환 효과 지정, 현제 가장 효율적인 방법을 선택하도록 지정
	sd.Flags = 0;

	// 04. IDXGISwapChain 인스턴스 생성 (교환 사슬 인스턴스(후면버퍼))

	IDXGISwapChain* dxgiDevice = 0;
	hr = _d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(hr)) {
		MessageBox(0, L"IDXGIDevice Failed", 0, 0);
		return false;
	}

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiDevice);
	if (FAILED(hr)) {
		MessageBox(0, L"IDXGIAdapter Failed", 0, 0);
		return false;
	}

	IDXGIFactory* dxgiFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr)) {
		MessageBox(0, L"IDXGIFactory Failed", 0, 0);
		return false;
	}

	hr = dxgiFactory->CreateSwapChain(_d3dDevice, &sd, &_swapChain);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateSwapChain Failed", 0, 0);
		return false;
	}

	dxgiDevice->Release();
	dxgiDevice = 0;

	dxgiAdapter->Release();
	dxgiAdapter = 0;

	dxgiFactory->Release();
	dxgiFactory = 0;

	// 05. 교환 사슬의 후면 버퍼에 대한 렌더 대상 뷰 생성

	ID3D11Texture2D* backBuffer;
	hr = _swapChain->GetBuffer(
		0, // 얻고자 하는 백버퍼의 인덱스
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	if (FAILED(hr)) {
		MessageBox(0, L"backBuffer getIndex Failed", 0, 0);
		return false;
	}


	// 뷰를 만들어 줌
	hr = _d3dDevice->CreateRenderTargetView(
		backBuffer,
		0,
		&_renderTargetView
	);

	backBuffer->Release();
	backBuffer = 0;

	// 06. 깊이.스텐실 버퍼와 그에 연결되는 스텐실 뷰 생성

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = 1280;
	depthStencilDesc.Height = 800;
	depthStencilDesc.MipLevels = 1;	// 밉맵의 수준
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 07. 렌더 대상 뷰와 깊이. 스텐실 뷰를 Direct3D가 사용할 수 있도록 렌더링 파이프라인

	// 08. 뷰포트 설정

	return false;
}