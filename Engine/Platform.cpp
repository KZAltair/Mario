#include "Platform.h"
#include <stdio.h>

GamePlatform::GamePlatform()
{
}

GamePlatform::~GamePlatform()
{
	
}

int GamePlatform::ClampScreen(int minScreen, int maxScreen, int val)
{
	if (val < minScreen) return minScreen;
	if (val > maxScreen) return maxScreen;
	return val;
}

void GamePlatform::MeasurePerformance()
{
	CyclesElapsed = EndCycleCount - LastCycleCount;
	CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
	MSPerFrame = (float32)((1000.0f * (float32)CounterElapsed) / (float32)PerfCountFrequency); //How many seconds the loop cycle took
	FPS = (float32)PerfCountFrequency / (float32)CounterElapsed;
	MCPerFrame = (float32)CyclesElapsed / (1000.0f * 1000.0f);

#if 0
	char Buffer[256];
	sprintf_s(Buffer, "%.02fms/f, %.02fFPS, %.02fmc/f\n", MSPerFrame, FPS, MCPerFrame);
	OutputDebugStringA(Buffer);
#endif
}

float32 GamePlatform::GetTimeFPS()
{
	return MCPerFrame;
}


