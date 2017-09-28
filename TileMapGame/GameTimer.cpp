#include "GameTimer.h"
#include <Windows.h>

GameTimer::GameTimer():
	_secondPerCount(0.0), _deltaTime(-1.0), _prevCount(0)
{
	__int64 countPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec); //�ʴ� ��� ī��Ʈ�� ������ ������
	_secondPerCount = 1.0 / (double)countPerSec; //ī��Ʈ�� ���� �ð��� �귶���� �����
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
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount); //�����쿡�� ī��Ʈ ������

	_deltaTime = (currentCount - _prevCount) * _secondPerCount;
	_prevCount = currentCount;
}

float GameTimer::GetDeltaTime() {
	return (float)_deltaTime;
}