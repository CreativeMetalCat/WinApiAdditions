#include "WindowRenderingNew.h"

POINT CreatePoint(int x, int y)
{
	POINT r;
	r.x = x;
	r.y = y;
	return r;
}
#pragma region WindowRendering
EventListener*WindowRendering::Listener = new EventListener();
WindowRendering::WindowRendering(WindowData Data, HCURSOR Cursor, LPCWSTR IconName, EventListener*Listener)
{
	this->Listener = new EventListener(*Listener);
	this->Data = Data;
	this->Cursor = Cursor;
	wce.cbSize = sizeof(WNDCLASSEXW);
	wce.style = CS_HREDRAW;
	wce.cbClsExtra = 0;
	wce.cbWndExtra = 0;
	wce.hInstance = Data.hInstance;
	wce.hIcon = LoadIcon(Data.hInstance, IconName);
	wce.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wce.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wce.lpszMenuName = Data.Menu;
	wce.lpszClassName = Data.Name;
	wce.hIconSm = LoadIcon(Data.hInstance, IconName);
	wce.lpfnWndProc = this->WndProc;

}
LRESULT CALLBACK WindowRendering::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		Listener->OnCommand(CommandArgs(hWnd,wmId));
	}
	break;
	case WM_MOUSEMOVE:
	{
		MouseMoveArgs e(GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd);
		Listener->OnMouseMove(e);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		Listener->OnMouseWheel(MouseWheelArgs(GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_LPARAM(lParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd));
		break;
	}
	case WM_LBUTTONDOWN:
	{
		Listener->OnMouseClick(MouseClickArgs(GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd));
		break;
	}
	case WM_MBUTTONDOWN:
	{
		Listener->OnMouseClick(MouseClickArgs(GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		Listener->OnMouseClick(MouseClickArgs(GET_KEYSTATE_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd));
		break;
	}
	case WM_KEYDOWN:
	{
		Listener->OnKeyDown(KeyDownArgs(GET_KEYSTATE_WPARAM(wParam), hWnd));
		UpdateWindow(hWnd);
		InvalidateRect(hWnd, 0, 0);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Listener->OnPaint(PaintArgs(hdc, hWnd));
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CLOSE:
		Listener->OnClose();
	case WM_DESTROY:
		Listener->OnDestroy();
		PostQuitMessage(0);
		break;
	default:

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
WindowRendering::WindowRendering(HINSTANCE hInst, HWND parent, LPCWSTR Name,LPCWSTR Menu, DWORD Style, int x, int y, int width, int height, LPCWSTR IconName, HCURSOR Cursor, EventListener*Listener)
{
	this->Listener = new EventListener(*Listener);

	//Window Data Setup
	Data.hInstance = hInst;
	Data.Parent = parent;
	Data.Name = Name;
	Data.Location.x = x;
	Data.Location.y = y;
	Data.Width = width;
	Data.Height = height;
	//WNDCLASSEX Setup
	this->Cursor = Cursor;
	wce.cbSize = sizeof(WNDCLASSEXW);
	wce.style = CS_HREDRAW;
	wce.cbClsExtra = 0;
	wce.cbWndExtra = 0;
	wce.hInstance = hInst;
	wce.hIcon = LoadIcon(hInst, IconName);
	wce.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wce.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wce.lpszMenuName = Menu;
	wce.lpszClassName = Name;
	wce.hIconSm = LoadIcon(Data.hInstance, IconName);
	wce.lpfnWndProc = this->WndProc;

}

BOOL WindowRendering::Show()
{
	if (RegisterClassEx(&wce))
	{
		Data.Window = CreateWindowW(Data.Name, Data.Name, Data.Style, Data.Location.x, Data.Location.y, Data.Width, Data.Height, Data.Parent, 0, Data.hInstance, 0);
		if (Data.Window)
		{

			ShowWindow(Data.Window, SW_SHOWDEFAULT);
			UpdateWindow(Data.Window);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

WindowRendering::~WindowRendering()
{
	DestroyCursor(this->Cursor);
	DestroyWindow(Data.Window);
}
#pragma endregion


WindowData::WindowData()
{

}

WindowData::WindowData(HWND Parent, HINSTANCE hInst, LPCWSTR Name, LPCWSTR Type, LPCWSTR Menu, DWORD Style, WNDPROC Proc, int x, int y, int width, int height)
{
	this->Type = Type;
	this->Name = Name;
	this->Style = Style;
	Location.x = x;
	Location.y = y;
	this->Width = width;
	this->Height = height;
	this->Menu = Menu;
}
