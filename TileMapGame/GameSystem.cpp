#include "GameSystem.h"

// ù��° ������ �ڵ��� ���� ���� �������� �ڵ��� �Ѿ� ���� �������� �ִ�.
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

	_depthStencilBuffer->Release();
	_depthStencilBuffer = 0;

	_depthStencilView->Release();
	_depthStencilView = 0;
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
	wc.cbClsExtra = 0; //�߰� �޸� ����
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

	if (false == InitDirect3D())
		return false;

	return true;
}

int GameSystem::Update() {
	MSG msg = { 0 };
	//int ret = 1;
	////�޼����� �޴´�. ������ �޼����� ���� ��� GetMessage �Լ����� ������ �ʴ´�.
	//while(ret = GetMessage(&msg, 0, 0, 0)){
	////	�̺�Ʈ�� ������ �������� �ƴ϶� -1�� �°��� 
	////	�� ū������ �ֱ� ������ ���� �����϶�� ���̴�
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
			//������ ó��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//���� ó��

			float color[4];
			color[0] = 0.0f; //red
			color[1] = 0.0f; //green
			color[2] = 1.0f; //blue
			color[3] = 1.0f; //alpha

			_d3dDeviceContext->ClearRenderTargetView(_renderTargetView, color);
			_d3dDeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			// todo : ���� ���õ� ��ο�
			_swapChain->Present(0, 0);
		}
	}
	return (int)msg.wParam; //������ �ִ��� �������� �޼������� �޴´�.
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
		MessageBox(0, L"�������� �ʴ� �ϵ����", L"Error", MB_OK);
		return false;
	}

	hr = _d3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&_4xMsaaQuallity
	);

	if (FAILED(hr)) {
		MessageBox(0, L"�������� �ʴ� �ϵ����", L"Error", MB_OK);
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

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ������ �뵵�� �����ϴ� ����ü, ���۸� ������ ������� 
	sd.BufferCount = 1; // ��ȯ �罽���� ����� �ĸ� ������ ����, ���� ���۸������� �ϳ��� ���. Ʈ���� ����
	sd.OutputWindow = _hMainWnd; // ������ ����� ǥ���� â�� �ڵ�
	sd.Windowed = true; // ������ ���/��ü ���
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ��ȯ ȿ�� ����, ���� ���� ȿ������ ����� �����ϵ��� ����
	sd.Flags = 0;

	// 04. IDXGISwapChain �ν��Ͻ� ���� (��ȯ �罽 �ν��Ͻ�(�ĸ����))

	IDXGISwapChain* dxgiDevice = 0;
	hr = _d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(hr)) {
		MessageBox(0, L"IDXGIDevice Failed", 0, 0);
		return false;
	}

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
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

	// 05. ��ȯ �罽�� �ĸ� ���ۿ� ���� ���� ��� �� ����

	ID3D11Texture2D* backBuffer;
	hr = _swapChain->GetBuffer(
		0, // ����� �ϴ� ������� �ε���
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	if (FAILED(hr)) {
		MessageBox(0, L"backBuffer getIndex Failed", 0, 0);
		return false;
	}


	// �並 ����� ��
	hr = _d3dDevice->CreateRenderTargetView(
		backBuffer,
		0,
		&_renderTargetView
	);

	backBuffer->Release();
	backBuffer = 0;

	// 06. ����.���ٽ� ���ۿ� �׿� ����Ǵ� ���ٽ� �� ����

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = 1280;
	depthStencilDesc.Height = 800;
	depthStencilDesc.MipLevels = 1;	// �Ӹ��� ����
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (_isEnable4xMsaa) {
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = _4xMsaaQuallity;
	}
	else {
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; 
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; //CPU�� ���� �ϴ°��� ����
	depthStencilDesc.MiscFlags = 0;

	hr = _d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &_depthStencilBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateTexture2D Failed", 0, 0);
		return false;
	}

	hr = _d3dDevice->CreateDepthStencilView(_depthStencilBuffer, 0, &_depthStencilView);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateDepthStencilView Failed", 0, 0);
		return false;
	}

	// 07. ���� ��� ��� ����. ���ٽ� �並 Direct3D�� ����� �� �ֵ��� ������ ������������ ��� ���ձ⿡ ���´�.
	_d3dDeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);


	// 08. ����Ʈ ����
	
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	_screenViewport.Width = 1280.0f;
	_screenViewport.Height = 800.0f;
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f; //fps���� �־����� �Ⱥ��̴� �Ͱ� ���� ���̴� ������ ������ ���Ѵ�.

	_d3dDeviceContext->RSSetViewports(1, &_screenViewport);
	
	return true;
}