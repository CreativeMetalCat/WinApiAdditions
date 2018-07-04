#pragma once
#include <Windows.h>
#include <functional>
#include <Windowsx.h>
#include <fstream>
#include <iostream>
#include <vector>
POINT CreatePoint(int x, int y);
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
struct CommandArgs
{
	HWND hWnd;
	int CommandId;
	CommandArgs(){}
	CommandArgs(HWND hWnd, int id)
	{
		this->hWnd = hWnd;
		this->CommandId = id;
	}
};
class EventListener
{
public:
	std::function<void(KeyDownArgs e)> OnKeyDown = [](KeyDownArgs e) {};
	std::function<void(MouseClickArgs e)> OnMouseClick = [](MouseClickArgs e) {};
	std::function<void(MouseWheelArgs e)>OnMouseWheel = [](MouseWheelArgs e) {};
	std::function<void(MouseMoveArgs e)>OnMouseMove = [](MouseMoveArgs e) {};
	std::function<void(PaintArgs e)> OnPaint = [](PaintArgs e) {};
	std::function<void( CommandArgs e )>OnCommand = [](CommandArgs e) {};
	std::function<void()> OnClose = []() {};
	std::function<void()> OnDestroy = []() {};
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
	LPCWSTR Menu;
	DWORD Style;
	POINT Location;
	int Width;
	int Height;
	WindowData(HWND Parent, HINSTANCE hInst, LPCWSTR Name, LPCWSTR Type, LPCWSTR Menu, DWORD Style, WNDPROC Proc, int x, int y, int width, int height);
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
	WindowRendering(HINSTANCE hInst, HWND parent, LPCWSTR Name, LPCWSTR Menu, DWORD Style, int x, int y, int width, int height, LPCWSTR IconName, HCURSOR Cursor, EventListener*Listener);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Show();
	~WindowRendering();
};

class ErrorLogger
{
	std::vector<std::string>*errors;
public:

	std::string filename;
	ErrorLogger(std::vector<std::string>*errors, std::string filename) { this->errors = errors, this->filename = filename; }
	ErrorLogger(std::string filename) { errors = new std::vector<std::string>(); this->filename = filename; }
	ErrorLogger() { errors = new std::vector<std::string>(); filename = "errors"; }
	void LogErrors()
	{
		std::ofstream file;
		file.open(filename + ".txt");
		for (unsigned i = 0; i < errors->size(); i++)
		{
			file << errors->at(i).c_str() << std::endl;
		}
		file.close();
	}
	void AddError(std::string error) { errors->push_back(error); }
	std::vector<std::string>*GetErros()const { return this->errors; }
};