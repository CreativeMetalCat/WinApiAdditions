#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <vector>
#include <functional>
#include <Windowsx.h>

namespace GraphicsGDI
{
	
	enum BrushType
	{
		LeftToRightDiagonal = HS_BDIAGONAL,
		RightToLeftDiagonal = HS_FDIAGONAL,
		Cross = HS_CROSS,
		Horizontal = HS_HORIZONTAL,
		DiagonalCross = HS_DIAGCROSS,
		Vertical = HS_VERTICAL,
		Solid = 0x06
	};
	enum PenStyle
	{
		PSolid = PS_SOLID,
		Dash = PS_DASH,
		Dash_Dot = PS_DASHDOT,
		Dash_Dot_Dot = PS_DASHDOTDOT,
		Emrty = PS_NULL,
		Inside_Frame = PS_INSIDEFRAME,
	};
	struct Rectangle
	{
	public:
		int Width;
		int Height;
		int X;
		int Y;
		Rectangle(int Width, int Height, int X, int Y);
		Rectangle();
		bool IsInside(POINT pt);
	};
	class TileBase
	{
	public:
		POINT Location;
		int Layer;
		Rectangle Collison;
		TileBase(POINT Location, int Layer, Rectangle Collison);
		TileBase(POINT Location, int Layer);
		virtual void Draw(HDC hdc) = 0;
	};

	class ColorTile : public TileBase
	{
	public:
		COLORREF Color;
		BrushType Brush;
		PenStyle BorderStyle;
		int BorderWidth;
		int size;
		bool EmptyInside = false;
		std::vector<POINT>*Points;
		ColorTile(COLORREF Color, BrushType Brush, PenStyle BorderStyle, int BorderWidth, std::vector<POINT>*&Points, int CountPolygons, int Layer);
		~ColorTile();
		virtual void Draw(HDC hdc) {/*never used. only for dynamic_cast*/ }
	};
	class ImageTile : public TileBase
	{
	public:

		HBITMAP Image;
		ImageTile(HBITMAP Image, POINT Location, int Layer);
		~ImageTile();
		virtual void Draw(HDC hdc) {/*never used. only for dynamic_cast*/ }
	};
	void DrawColorObject(HDC hdc, std::vector<POINT>*&Points, COLORREF Color, BrushType brush, PenStyle BorderStyle, int BorderWidth, bool EmrtyInside);
	void DrawColorTile(HDC hdc, ColorTile*&tile);
	void DrawImage(HDC hdc, HBITMAP hBitmap, POINT location);
	void DrawImage(HDC hdc, HINSTANCE hInst, std::string filename, POINT location);
	void DrawImage(HDC hdc, HINSTANCE hInst, LPCWSTR filename, POINT location);
}