#pragma once
#include <Windows.h>

class GameTimer{
public:
	GameTimer();
	~GameTimer();

public:
	void Reset();
	void Update();
	
	float GetDeltaTime();

private:
	__int64 _prevCount;
	double _deltaTime;
	double _secondPerCount;
};