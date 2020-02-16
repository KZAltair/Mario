#include "Window.h"
#include "GameLoop.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
	Window wnd(hInstance);
	Game theGame(wnd);
	QueryPerformanceCounter(&wnd.gp.LastCounter);
	wnd.gp.LastCycleCount = __rdtsc();
	while (wnd.ProcessMessage())
	{
		theGame.Go();
	}
	return 0;
}