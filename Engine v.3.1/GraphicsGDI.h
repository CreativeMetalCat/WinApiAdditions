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
	class Image
	{
		HBITMAP bitmap;
	public:
		POINT Location;
		Image(HBITMAP bitmap, POINT pt);
		Image(HINSTANCE hInst, LPCWSTR filename, POINT pt);
		Image(HINSTANCE hInst, std::string filename, POINT location);
		HBITMAP GetBitmap()const;
		void SetBitmap(HBITMAP bitmap);
	};

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
		Pen_Solid = PS_SOLID,
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
		bool operator==(Rectangle rect);
		bool operator==(Rectangle* rect);
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

	HBITMAP LoadBitmapFromFile(HINSTANCE hInst, LPCWSTR name)
	{
		return (HBITMAP)LoadImageW(hInst, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
	HBITMAP LoadBitmapFromFile(HINSTANCE hInst, std::string name)
	{
		std::wstring stemp = std::wstring(name.begin(), name.end());
		LPCWSTR sw = stemp.c_str();
		return (HBITMAP)LoadImageW(hInst, sw, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
	HBRUSH CreateBrushFromImage(GraphicsGDI::Image*image)
	{
		return CreatePatternBrush(image->GetBitmap());
	}
	HBRUSH CreateBrushFromColor(COLORREF color, GraphicsGDI::BrushType type)
	{
		if (type != GraphicsGDI::BrushType::Solid)
		{
			return CreateHatchBrush(type, color);
		}
		else
		{
			return CreateSolidBrush(color);
		}
	}
	HBRUSH CreateBrushFromBitmap(HBITMAP bitmap)
	{
		return CreatePatternBrush(bitmap);
	}
	HBRUSH CreateBrushFromImage(LPCWSTR filename, HINSTANCE hInst)
	{

		return CreatePatternBrush(LoadBitmapFromFile(hInst, filename));
	}
	HBRUSH CreateBrushFromImage(std::string filename, HINSTANCE hInst)
	{
		return CreatePatternBrush(LoadBitmapFromFile(hInst, filename));
	}
}