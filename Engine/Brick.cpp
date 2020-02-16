#include "Brick.h"

Brick::Brick(const Vec2& pos, float radius)
	:
	pos(pos),
	bounds(bounds),
	radius(radius)
{
}

void Brick::Draw(Render& rd, Color c) const
{
	const RectF rect = GetRect();
	const int in_x = (int)rect.left;
	const int in_y = (int)rect.top;
	const int width = (int)rect.GetWidth();
	const int height = (int)rect.GetHeight();
	for (int y = in_y; y <= in_y + height; y++)
	{
		for (int x = in_x; x <= in_x + width; x++)
		{
			rd.PutPixel(x, y, c);
		}
	}
	rd.PutPixel(rect.left, rect.top, Color(0, 255, 0));
	rd.PutPixel(rect.right, rect.top, Color(0, 255, 0));
	rd.PutPixel(rect.left, rect.bottom, Color(0, 255, 0));
	rd.PutPixel(rect.right, rect.bottom, Color(0, 255, 0));

}

RectF Brick::GetRect() const
{
	return RectF::FromCenter(pos, radius, radius);
}
