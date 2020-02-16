#pragma once
#include "Color.h"
#include <string>
#include "RectI.h"

class Surface
{
public:
	Surface(int width, int height);
	Surface(const std::string& filename);
	Surface(const Surface& s);
	~Surface();
	Surface& operator=(const Surface& rhs);
	void PutPixel(int x, int y, Color c);
	Color GetPixel(int x, int y) const;
	int GetWidth() const;
	int GetHeight() const;
	RectI GetRect() const;

private:
	Color* pPixels = nullptr;
	int width = 0;
	int height = 0;
};
