#include "GameTimer.h"
#include <Windows.h>

GameTimer::GameTimer():
	_secondPerCount(0.0), _deltaTime(-1.0), _prevCount(0)
{
	__int64 countPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec); //초당 몇번 카운트를 세는지 가져옴
	_secondPerCount = 1.0 / (double)countPerSec; //카운트당 얼마의 시간이 흘렀는지 계산함
}

GameTimer::~GameTimer() {

}

void GameTimer::Reset() {
	__int64 currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
	_prevCount = currentCount;
}

void GameTimer::Update() {
	__int64 currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount); //윈도우에서 카운트 가져옴

	_deltaTime = (currentCount - _prevCount) * _secondPerCount;
	_prevCount = currentCount;
}

float GameTimer::GetDeltaTime() {
	return (float)_deltaTime;
}