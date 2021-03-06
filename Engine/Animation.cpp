#include "Animation.h"

Animation::Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime, Color chroma)
	:
	sprite(sprite),
	holdTime(holdTime),
	chroma(chroma)
{
	for (int i = 0; i < count; i++)
	{
		frames.emplace_back(x + i * width, x + (i + 1) * width, y, y + height);
	}
}

void Animation::Draw(const Vei2& pos, Render& rd) const
{
	rd.DrawSprite(pos.x, pos.y, frames[iCurFrame], sprite, chroma);
}

void Animation::Draw(const Vei2& pos, Render& rd, const RectI& clip) const
{
	rd.DrawSprite(pos.x, pos.y, frames[iCurFrame], clip, sprite, chroma);
}

void Animation::Update(float dt)
{
	curFrameTime += dt;
	while (curFrameTime >= holdTime)
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Animation::UpdateOnce(float dt)
{
	curFrameTime += dt;
	while (curFrameTime >= holdTime)
	{
		AdvanceOnce();
		curFrameTime -= holdTime;
	}
}

void Animation::Advance()
{
	if (++iCurFrame >= frames.size())
	{
		iCurFrame = 0;
	}
}

void Animation::AdvanceOnce()
{
	if (++iCurFrame >= frames.size())
	{
		iCurFrame = frames.size() - 1;
	}
}
