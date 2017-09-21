#pragma once

#include <Windows.h>

class GameSystem {
private:
	static GameSystem* _instance;

public:
	static GameSystem& GetInstance();

private:
	GameSystem();		// �ٸ� ������ �Ǽ��� �������� �ʵ���, �����ڸ� �����̺����� �����.

public:
	~GameSystem();

public:
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	int Update();
};