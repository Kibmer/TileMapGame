#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msa, WPARAM wParam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow){
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
		return 0;
	}

	HWND hWnd = CreateWindow(L"Base", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//�޼����� �޴´�. ������ �޼����� ���� ��� GetMessage �Լ����� ������ �ʴ´�.
	//int ret = 1;
	MSG msg = { 0 };
	//while(ret = GetMessage(&msg, 0, 0, 0)){
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			//������ ó��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			//���� ó��

		}
		//�̺�Ʈ�� ������ �������� �ƴ϶� -1�� �°��� 
		//�� ū������ �ֱ� ������ ���� �����϶�� ���̴�
		/*if (-1 == ret) {
			break;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
	}
	return (int)msg.wParam; //������ �ִ��� �������� �޼������� �޴´�.
}

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