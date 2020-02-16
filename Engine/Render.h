#pragma once
#include "Platform.h"
#include "Color.h"
#include "Surface.h"
#include "RectI.h"
class Render
{
public:
	Render();
	~Render();

	void DrawIntRect(int x0, int y0, int x1, int y1, uint32 color, game_bitmap_struct* GameBitmapStruct);
	void DrawFloatRect(game_vector2 v1, game_vector2 half_size, uint32 color, game_bitmap_struct* GameBitmapStruct);
	void DrawPixel(int dx, int dy, uint8 dR, uint8 dG, uint8 dB, uint8 dA, game_bitmap_struct* GameBitmapStruct);
	void PutPixel(int x, int y, Color c);
	void DrawImage(int x0, int y0, int x1, int y1, game_bitmap_struct* GameBitmapStruct, uint32* Pixels, uint16 BytesPerPixel);
	void DrawSpriteNonChroma(int x, int y, const Surface& s);
	void DrawSpriteNonChroma(int x, int y, const RectI& srcRect, const Surface& s);
	void DrawSpriteNonChroma(int x, int y, RectI srcRect, const RectI& clip, const Surface& s);
	void DrawSprite(int x, int y, RectI srcRect, const RectI& clip, const Surface& s, Color chroma = Colors::Magenta);
	void DrawSprite(int x, int y, const Surface& s, Color chroma = Colors::Magenta);
	void DrawSprite(int x, int y, RectI srcRect, const Surface& s, Color chroma = Colors::Magenta);
	void ClearScreen(Color c);
	void BeginFrame();
	void EndFrame(game_bitmap_struct* GameBitmapStruct);

private:
	Color* pColorBuffer = nullptr;
};