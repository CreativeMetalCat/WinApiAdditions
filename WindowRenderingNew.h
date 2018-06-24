#pragma once
#include <Windows.h>
#include <functional>
#include <Windowsx.h>

struct MouseArgs
{
public:
	HWND hWnd;
	int x;
	int y;
	MouseArgs(int x, int y, HWND hWnd) { this->x = x; this->y = y; this->hWnd = hWnd; }
	MouseArgs() {};
};
struct MouseWheelArgs : public MouseArgs
{
public:
	int WheelDelta;
	int Button;
	MouseWheelArgs(int WheelDelta, int Button, int x, int y, HWND hWnd) :MouseArgs(x, y, hWnd)
	{
		this->WheelDelta = WheelDelta;
		this->Button = Button;
	}
	MouseWheelArgs() {  }
};
struct MouseClickArgs : MouseArgs
{
	int Button;
	MouseClickArgs(int Button, int x, int y, HWND hWnd) :MouseArgs(x, y, hWnd)
	{
		this->Button = Button;
	}
	MouseClickArgs() {}
};
struct MouseMoveArgs : MouseArgs
{
	int Button;
	MouseMoveArgs(int Button, int x, int y, HWND hWnd) : MouseArgs(x, y, hWnd)
	{
		this->Button = Button;
	}
	MouseMoveArgs() {}
};
struct KeyDownArgs
{
	HWND hWnd;
	int Key;
	KeyDownArgs(int Key, HWND hWnd) { this->Key = Key; this->hWnd = hWnd; }
	KeyDownArgs() {}
};
struct PaintArgs
{
public:
	HDC hdc;
	HWND hWnd;
	PaintArgs(HDC hdc, HWND hwnd) { this->hdc = hdc; this->hWnd = hwnd; }
	PaintArgs() {}
};

class EventListener
{
public:
	std::function<void(KeyDownArgs e)> OnKeyDown = [](KeyDownArgs e) {};
	std::function<void(MouseClickArgs e)> OnMouseClick = [](MouseClickArgs e) {};
	std::function<void(MouseWheelArgs e)>OnMouseWheel = [](MouseWheelArgs e) {};
	std::function<void(MouseMoveArgs e)>OnMouseMove = [](MouseMoveArgs e) {};
	std::function<void(PaintArgs e)> OnPaint = [](PaintArgs e) {};
	std::function<void(int CommandId)>OnCommand = [](int CommandId) {};
	std::function<void()> OnClose = []() {};
	std::function<void()>OnDestroy = []() {};
	EventListener() {}
	~EventListener()
	{
	}
};


struct WindowData
{
public:
	HINSTANCE hInstance;
	HWND Parent;
	HWND Window;
	LPCWSTR Name;
	LPCWSTR Type;
	DWORD Style;
	POINT Location;
	int Width;
	int Height;
	WindowData(HWND Parent, HINSTANCE hInst, LPCWSTR Name, LPCWSTR Type, DWORD Style, WNDPROC Proc, int x, int y, int width, int height);
	WindowData();
};

class WindowRendering
{
	WNDCLASSEX wce;
public:
	static EventListener * Listener;
	WindowData Data;
	HCURSOR Cursor;
	WindowRendering(WindowData Data, HCURSOR Cursor, LPCWSTR IconName, EventListener*Listener);
	WindowRendering(HINSTANCE hInst, HWND parent, LPCWSTR Name, DWORD Style, int x, int y, int width, int height, LPCWSTR IconName, HCURSOR Cursor, EventListener*Listener);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Show();
	~WindowRendering();
};

