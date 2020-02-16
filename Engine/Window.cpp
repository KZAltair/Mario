#include "Window.h"
#include <assert.h>
#include <algorithm>

Window::Window(HINSTANCE hInst)
	:
	hInst(hInst)
{
	QueryPerformanceFrequency(&gp.PerCountFrequencyResult);
	gp.PerfCountFrequency = gp.PerCountFrequencyResult.QuadPart;
	//Init crucial vars
	BitmapInfo = {};
	GameBitmapStruct = {};

	// register window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,_HandleMsgSetup,0,0,
		hInst,nullptr,nullptr,nullptr,nullptr,
		wndClassName,nullptr };
	wc.hIconSm = 0; //TODO: Make icon for engine
	wc.hIcon = 0; //TODO: Make icon for engine
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClassEx(&wc);

	// create window & get hWnd
	RECT wr;
	wr.left = 350;
	wr.right = ScreenWidth + wr.left;
	wr.top = 100;
	wr.bottom = ScreenHeight + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	hWnd = CreateWindow(wndClassName, L"RayCast v.001",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, hInst, this);
	// show and update

	DeviceContext = GetDC(hWnd);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
}

Window::~Window()
{
	if (GameBitmapStruct.BitmapMemory)
	{
		VirtualFree(GameBitmapStruct.BitmapMemory, 0, MEM_RELEASE);
	}
	// unregister window class
	UnregisterClass(wndClassName, hInst);
}


bool Window::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}

	return true;
}

HWND Window::GetWindowHandle()
{
	return hWnd;
}

LRESULT WINAPI Window::_HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = reinterpret_cast<Window*>(pCreate->lpCreateParams);
		// sanity check
		assert(pWnd != nullptr);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::_HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::_HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

		// ************ KEYBOARD MESSAGES ************ //
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // no thank you on the autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		if (pt.x > 0 && pt.x < ScreenWidth && pt.y > 0 && pt.y < ScreenHeight)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				pt.x = std::max(short(0), pt.x);
				pt.x = std::min(short(ScreenWidth - 1), pt.x);
				pt.y = std::max(short(0), pt.y);
				pt.y = std::min(short(ScreenHeight - 1), pt.y);
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
				mouse.OnLeftReleased(pt.x, pt.y);
				mouse.OnRightReleased(pt.x, pt.y);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		SetForegroundWindow(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.OnWheelUp(pt.x, pt.y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse.OnWheelDown(pt.x, pt.y);
		}
		break;
	}
	// ************ END MOUSE MESSAGES ************ //
	//case WM_PAINT:
	//{
		//PAINTSTRUCT paint;
		//HDC DeviceContext = BeginPaint(hWnd, &paint);
		//int x = paint.rcPaint.left = 0;
		//int y = paint.rcPaint.top = 0;
		//int Width = paint.rcPaint.right - paint.rcPaint.left;
		//int Height = paint.rcPaint.bottom - paint.rcPaint.top;
		//gfx.UpdateWindowGraphics(DeviceContext);
		//EndPaint(hWnd, &paint);
	//}
	case WM_SIZE:
	{
		ResizeDIBSection();
	}break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

RectI Window::GetScreenRect()
{
	return {0, ScreenWidth, 0, ScreenHeight};
}

void Window::UpdateWindowGraphics()
{
	StretchDIBits(DeviceContext,
		0, 0, GameBitmapStruct.BitmapWidth, GameBitmapStruct.BitmapHeight,
		0, 0, ScreenWidth, ScreenHeight,
		GameBitmapStruct.BitmapMemory,
		&BitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY);
}

game_bitmap_struct Window::GetBitmapStruct()
{
	return GameBitmapStruct;
}

void Window::ResizeDIBSection()
{
	if (GameBitmapStruct.BitmapMemory)
	{
		VirtualFree(GameBitmapStruct.BitmapMemory, 0, MEM_RELEASE);
	}

	GameBitmapStruct.BitmapWidth = ScreenWidth;
	GameBitmapStruct.BitmapHeight = ScreenHeight;

	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = GameBitmapStruct.BitmapWidth;
	BitmapInfo.bmiHeader.biHeight = -GameBitmapStruct.BitmapHeight;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	int BytesPerPixel = 4;
	int BitmapMemorySize = (GameBitmapStruct.BitmapWidth * GameBitmapStruct.BitmapHeight) * BytesPerPixel;
	GameBitmapStruct.BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}
