#include "GraphicsGDI.h"


namespace GraphicsGDI
{
	Image::Image(HBITMAP bitmap, POINT pt)
	{
		this->bitmap = bitmap;
		Location = pt;
	}
	Image::Image(HINSTANCE hInst, LPCWSTR filename, POINT pt)
	{
		this->bitmap = (HBITMAP)LoadImageW(hInst, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		Location = pt;
	}
	Image::Image(HINSTANCE hInst, std::string filename, POINT location)
	{
		std::wstring stemp = std::wstring(filename.begin(), filename.end());
		LPCWSTR sw = stemp.c_str();
		this->bitmap = (HBITMAP)LoadImageW(hInst, sw, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		this->Location = location;
	}
	HBITMAP Image::GetBitmap()const
	{
		return bitmap;
	}

	void Image::SetBitmap(HBITMAP bitmap)
	{
		this->bitmap = bitmap;
	}

	void DrawColorObject(HDC hdc, std::vector<POINT>*&Points, COLORREF Color, BrushType brush, PenStyle BorderStyle, int BorderWidth, bool EmrtyInside)
	{
		HPEN newPen = CreatePen(BorderStyle, BorderWidth, Color);
		SelectObject(hdc, newPen);
		HBRUSH newBrush;
		if (brush != BrushType::Solid)
		{
			newBrush = CreateHatchBrush(brush, Color);
		}
		else
		{
			newBrush = CreateSolidBrush(Color);
		}
		SelectBrush(hdc, newBrush);
		POINT* points = new POINT[Points->size()];
		for (int i = 0; i < Points->size(); i++)
		{
			points[i] = Points->at(i);
		}
		if (EmrtyInside == false)
		{
			Polygon(hdc, points, Points->size());
		}
		else
		{
			Polyline(hdc, points, Points->size());
		}
		delete[] points;
		DeleteObject(newBrush);
		DeleteObject(newPen);
	}
	void DrawColorTile(HDC hdc, ColorTile*&tile)
	{
		HPEN newPen = CreatePen(tile->BorderStyle, tile->BorderWidth, tile->Color);
		SelectObject(hdc, newPen);
		HBRUSH newBrush;
		if (tile->Brush != BrushType::Solid)
		{
			newBrush = CreateHatchBrush(tile->Brush, tile->Color);
		}
		else
		{
			newBrush = CreateSolidBrush(tile->Color);
		}
		POINT* points = new POINT[tile->Points->size()];
		SelectBrush(hdc, newBrush);
		for (int i = 0; i < tile->Points->size(); i++)
		{
			points[i] = tile->Points->at(i);
		}
		if (tile->EmptyInside == false)
		{
			Polygon(hdc, points, tile->Points->size());
		}
		else
		{
			Polyline(hdc, points, tile->Points->size());
		}
		delete[] points;
		DeleteObject(newBrush);
		DeleteObject(newPen);
	}
	void DrawImage(HDC hdc, HBITMAP hBitmap, POINT location)
	{
		HDC memDCExercising;
		memDCExercising = CreateCompatibleDC(hdc);
		SelectObject(memDCExercising, hBitmap);
		BitBlt(hdc, location.x, location.y, 5000, 5000, memDCExercising, 0, 0, SRCCOPY);
		DeleteDC(memDCExercising);
	}
	void DrawImage(HDC hdc, HINSTANCE hInst, std::string filename, POINT location)
	{

		std::wstring stemp = std::wstring(filename.begin(), filename.end());
		LPCWSTR sw = stemp.c_str();
		HBITMAP bmp;
		HDC memDCExercising;
		bmp = (HBITMAP)LoadImage(hInst, sw, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		memDCExercising = CreateCompatibleDC(hdc);
		SelectObject(memDCExercising, bmp);
		BitBlt(hdc, location.x, location.y, 5000, 5000, memDCExercising, 0, 0, SRCCOPY);
		DeleteDC(memDCExercising);
		DeleteObject(bmp);

	}
	void DrawImage(HDC hdc, HINSTANCE hInst, LPCWSTR filename, POINT location)
	{
		HBITMAP bmp;
		HDC memDCExercising;
		bmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		memDCExercising = CreateCompatibleDC(hdc);
		SelectObject(memDCExercising, bmp);
		BitBlt(hdc, location.x, location.y, 5000, 5000, memDCExercising, 0, 0, SRCCOPY);
		DeleteDC(memDCExercising);
		DeleteObject(bmp);
	}


	TileBase::TileBase(POINT Location, int Layer, Rectangle Collison)
	{
		this->Location = Location;
		this->Layer = Layer;
		this->Collison = Collison;
	}
	TileBase::TileBase(POINT Location, int Layer)
	{
		this->Location = Location;
		this->Layer = Layer;
	}

	ColorTile::ColorTile(COLORREF Color, BrushType Brush, PenStyle BorderStyle, int BorderWidth, std::vector<POINT>*&Points, int CountPolygons, int Layer) : TileBase(Points->at(0), Layer)
	{
		this->Color = Color;
		this->Brush = Brush;
		this->Points = Points;
		this->BorderStyle = BorderStyle;
		this->BorderWidth = BorderWidth;
	}
	ColorTile::~ColorTile()
	{
		this->Points->~vector();
	}



	ImageTile::ImageTile(HBITMAP Image, POINT Location, int Layer) :TileBase(Location, Layer)
	{
		this->Image = Image;
	}
	ImageTile::~ImageTile()
	{

	}
	Rectangle::Rectangle(int Width, int Height, int X, int Y)
	{
		this->Width = Width;
		this->Height = Height;
		this->X = X;
		this->Y = Y;
	}

	Rectangle::Rectangle()
	{
	}

	bool Rectangle::IsInside(POINT pt)
	{
		for (int i = 0; i < Width; i++)
		{
			for (int o = 0; o < Height; o++)
			{
				if (pt.x == i + this->X && pt.y == o + this->Y)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool Rectangle::operator==(Rectangle rect)
	{
		if (rect.Width != this->Width || rect.Height != this->Height||rect.X!=this->X||rect.Y!=this->Y)
		{
			return false;
		}
		else
		{
			return true;
		}
		
	}
	bool Rectangle::operator==(Rectangle* rect)
	{
		if (rect->Width != this->Width || rect->Height != this->Height || rect->X != this->X || rect->Y != this->Y)
		{
			return false;
		}
		else
		{
			return true;
		}

	}
}