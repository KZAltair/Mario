#pragma once

#include "Animation.h"
#include "Vec2.h"
#include "RectF.h"
#include "Brick.h"

class Character
{
private:
	enum class Sequence
	{
		RunningLeft,
		RunningRight,
		JumpingLeft,
		JumpingRight,
		StandingLeft,
		StandingRight,
		Count
	};
public:
	Character(const Vec2& pos);
public:
	void Draw(Render& rd) const;
	void SetDirection(const Vec2& dir);
	void Update(float dt);
	const Vec2 GetPosition() const;
	bool DoBrickCollision(Brick& brick);
	void DrawBound(Render& rd, Color c);
	RectF GetRect()const;
	Vec2 GetCenter() const;
	Vec2 GetPos() const;
private:
	Surface sprite;
	Vec2 pos;
	Vec2 vel = { 0.0f, 0.0f };
	Vec2 speed = { 100.0f, 100.0f };
	RectF rect;
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingRight;
	float gravity = 5.0f;
	static constexpr float radius = 7.0f;
};

