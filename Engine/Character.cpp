#include "Character.h"
#include <cmath>

Character::Character(const Vec2& pos)
	:
	sprite("mario01.bmp"),
	pos(pos)
{
	for (int i = 0; i < (int)Sequence::Count; i++)
	{
		if (i == 0)
		{
			animations.emplace_back(Animation(120, 0, 30, 30, 3, sprite, 0.15f));
		}
		else if (i == 1)
		{
			animations.emplace_back(Animation(240, 0, 30, 30, 3, sprite, 0.15f));
		}
		else if (i == 2)
		{
			animations.emplace_back(Animation(30, 0, 30, 30, 2, sprite, 0.15f));
		}
		else if (i == 3)
		{
			animations.emplace_back(Animation(330, 0, 30, 30, 2, sprite, 0.15f));
		}
		else if (i == 4)
		{
			animations.emplace_back(Animation(180, 0, 30, 30, 1, sprite, 0.15f));
		}
		else if (i == 5)
		{
			animations.emplace_back(Animation(210, 0, 30, 30, 1, sprite, 0.15f));
		}
	}
}

void Character::Draw(Render& rd) const
{
	//TODO: make screen rect reference
	animations[(int)iCurSequence].Draw((Vei2)pos, rd, {0,800,0,600});
}

void Character::SetDirection(const Vec2& dir)
{
	if (dir.x > 0.0f)
	{
		iCurSequence = Sequence::RunningRight;
	}
	else if (dir.x < 0.0f)
	{
		iCurSequence = Sequence::RunningLeft;
	}
	else if (dir.y < 0.0f)
	{
		iCurSequence = Sequence::JumpingRight;
		speed.y -= gravity;

	}
	else if (dir.y > 0.0f)
	{
		iCurSequence = Sequence::JumpingLeft;
	}
	else
	{
		if (vel.x > 0.0f)
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if (vel.x < 0.0f)
		{
			iCurSequence = Sequence::StandingLeft;
		}
		else if (vel.y > 0.0f)
		{
			iCurSequence = Sequence::JumpingRight;
		}
		else if (vel.y < 0.0f)
		{
			iCurSequence = Sequence::JumpingLeft;
		}
	}
	vel.x = dir.x * speed.x;
	vel.y = dir.y * speed.y;
	
}

void Character::Update(float dt)
{
	if (iCurSequence == Sequence::JumpingRight || iCurSequence == Sequence::JumpingLeft)
	{
		animations[(int)iCurSequence].UpdateOnce(dt);
	}
	else
	{
		animations[(int)iCurSequence].Update(dt);
	}
	pos += vel * dt;

}

const Vec2 Character::GetPosition() const
{
	return pos;
}

bool Character::DoBrickCollision(Brick& brick)
{
	bool collided = false;
	if (rect.IsOverlappingWith(brick.GetRect()))
	{
		if (!std::signbit(vel.y))
		{
			pos.y = brick.GetRect().top - rect.GetHeight();
			collided = true;
		}
		else if (std::signbit(vel.y))
		{
			pos.y = brick.GetRect().bottom + rect.GetHeight();
			collided = true;
		}
		else if (!std::signbit(vel.x))
		{
			pos.x = brick.GetRect().left - rect.GetWidth();
			collided = true;
		}
		else if (std::signbit(vel.x))
		{
			pos.x = brick.GetRect().right + rect.GetWidth();
			collided = true;
		}
	}
		
	return collided;
}

void Character::DrawBound(Render& rd, Color c)
{
	rect = GetRect();
	const int in_x = (int)rect.left;
	const int in_y = (int)rect.top;
	const int width = (int)rect.GetWidth();
	const int height = (int)rect.GetHeight();
	for (int y = in_y; y <= in_y + height; y++)
	{
		for (int x = in_x; x <= in_x + width; x++)
		{
			if (y == in_y || y == in_y + height || x == in_x || x == in_x + width)
			{
				rd.PutPixel(x, y, c);
			}
		}
	}
	rd.PutPixel(rect.left, rect.top, Color(0, 255, 0));
	rd.PutPixel(rect.right, rect.top, Color(0, 255, 0));
	rd.PutPixel(rect.left, rect.bottom, Color(0, 255, 0));
	rd.PutPixel(rect.right, rect.bottom, Color(0, 255, 0));
}

RectF Character::GetRect() const
{
	return RectF::FromCenter(GetCenter(), radius, radius);
}

Vec2 Character::GetCenter() const
{
	return Vec2(pos.x + radius, pos.y + radius);
}

Vec2 Character::GetPos() const
{
	return pos;
}

