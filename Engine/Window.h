#pragma once
#include "Render.h"
#include "Keyboard.h"
#include "Mouse.h"
// for granting special access to hWnd only for Graphics constructor
// for granting special access to hWnd only for Graphics constructor
class HWNDKey
{
public:
	HWNDKey(const HWNDKey&) = delete;
	HWNDKey& operator=(HWNDKey&) = delete;
protected:
	HWNDKey() = default;
protected:
	HWND hWnd = nullptr;
};

class Window : public HWNDKey
{
public:
	Window(HINSTANCE hInst);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();

	// returns false if quitting
	bool ProcessMessage();
	HWND GetWindowHandle();
private:
	static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void ResizeDIBSection();
public:
	void UpdateWindowGraphics();
	game_bitmap_struct GetBitmapStruct();
	static RectI GetScreenRect();
private:
	static constexpr const wchar_t* wndClassName = L"RayCast Engine Window Class";
	HINSTANCE hInst = nullptr;

	HDC DeviceContext;
	BITMAPINFO BitmapInfo;
	game_bitmap_struct GameBitmapStruct;

public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
	GamePlatform gp;

	Keyboard kbd;
	Mouse mouse;

};
