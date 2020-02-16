#pragma once
#include "Surface.h"
#include "Render.h"
#include <vector>

class Animation
{
public:
	Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime, Color chroma = Colors::Magenta);
public:
	void Draw(const Vei2& pos, Render& rd) const;
	void Draw(const Vei2& pos, Render& rd, const RectI& clip) const;
	void Update(float dt);
	void UpdateOnce(float dt);
private:
	void Advance();
	void AdvanceOnce();
private:
	Color chroma;
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime = 0.0f;
	float curFrameTime = 0.0f;

};