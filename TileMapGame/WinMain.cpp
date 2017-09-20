#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msa, WPARAM wParam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow){
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
		return 0;
	}

	HWND hWnd = CreateWindow(L"Base", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//메세지를 받는다. 하지만 메세지가 없을 경우 GetMessage 함수에서 나오지 않는다.
	//int ret = 1;
	MSG msg = { 0 };
	//while(ret = GetMessage(&msg, 0, 0, 0)){
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			//윈도우 처리
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			//게임 처리

		}
		//이벤트의 유무에 따른것이 아니라 -1이 온것은 
		//더 큰문제가 있기 때문에 앱을 종료하라는 뜻이다
		/*if (-1 == ret) {
			break;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
	}
	return (int)msg.wParam; //문제가 있는지 없는지를 메세지에서 받는다.
}

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