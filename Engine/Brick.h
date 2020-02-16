#pragma once
#include "RectF.h"
#include "Vec2.h"
#include "Render.h"

class Brick
{
public:
	Brick(const Vec2& pos, float radius);
public:
	void Draw(Render& rd, Color c) const;
	RectF GetRect()const;

private:
	float radius;
	Vec2 pos;
	RectF bounds;
};