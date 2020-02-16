#pragma once
#include "FrameTimer.h"
#include "Animation.h"
#include "Character.h"
#include "Brick.h"

class Game
{
public:
	Game(class Window& wnd);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel(float32 dt);
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	Window& wnd;
	FrameTimer ft;
	game_bitmap_struct bmp;
	/********************************/
	/*  User Variables              */
	/********************************/
	game_vector2 Player = {0};
	const float playerSpeed = 2000.0f;
	Render render;
	Character mario;
	Brick brick;
	Brick brick1;
	bool curKeyState = false;
	bool prevKeyState = false;
};