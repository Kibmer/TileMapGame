#pragma once

#include <Windows.h>

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
	int Update();
};