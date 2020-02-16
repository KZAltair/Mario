#include "Surface.h"
#include <assert.h>
#include "Platform.h"
#include <fstream>

Surface::Surface(int width, int height)
	:
	width(width),
	height(height),
	pPixels(new Color[width * height])
{
}

Surface::Surface(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	assert(file);
	bitmap_header_24bit bmpHeader;
	file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));

	assert(bmpHeader.BitesPerPixel == 24 || bmpHeader.BitesPerPixel == 32);

	const bool is32b = bmpHeader.BitesPerPixel == 32;

	width = bmpHeader.Width;

	//test for reverse row order and control loop accordingly
	int yStart = 0;
	int yEnd = 0;
	int dy = 0; // direction of height row
	if (bmpHeader.Height < 0)
	{
		height = -bmpHeader.Height;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmpHeader.Height;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}
	

	pPixels = new Color[width * height];

	file.seekg(bmpHeader.FileOffsetToPixels);

	//Padding is for 24 bits only
	const int padding = (4 - (width * 3) % 4) % 4;

	for (int y = yStart; y != yEnd; y+=dy)
	{
		for (int x = 0; x < width; x++)
		{
			PutPixel(x, y, Color(file.get(), file.get(), file.get()));
			if (is32b)
			{
				file.seekg(1, std::ios::cur);
			}
		}
		if (!is32b)
		{
			file.seekg(padding, std::ios::cur);
		}
	}
}

Surface::Surface(const Surface& rhs)
	:
	Surface(rhs.width, rhs.height)
{
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++)
	{
		pPixels[i] = rhs.pPixels[i];
	}
}

Surface::~Surface()
{
	delete[] pPixels;
	pPixels = nullptr;
}

Surface& Surface::operator=(const Surface& rhs)
{
	width = rhs.width;
	height = rhs.height;
	delete[] pPixels;
	pPixels = new Color[width * height];

	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++)
	{
		pPixels[i] = rhs.pPixels[i];
	}
	return *this;
}

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	pPixels[y * width + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	return pPixels[y * width + x];
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return {0, width, 0, height};
}
