#include "Window.h"
#include "GameLoop.h"

Game::Game(Window& wnd)
	:
	wnd(wnd),
	mario(Vec2(100.0f, 100.0f)),
	brick(Vec2(200.0f, 100.0f), 20.0f),
	brick1(Vec2(160.0f, 140.0f), 20.0f)
{
	//bmp = {};
	//bmp.BitmapMemory = wnd.gp.DebugLoadBMP("h01.bmp");
	//bmp.PixelPointer = wnd.gp.GetPixelPointer();
	
}

void Game::Go()
{
	UpdateModel(wnd.gp.GetTimeFPS());
	render.BeginFrame();
	ComposeFrame();
	render.EndFrame(&wnd.GetBitmapStruct());
	wnd.gp.EndCycleCount = __rdtsc();
	QueryPerformanceCounter(&wnd.gp.EndCounter);
	wnd.gp.MeasurePerformance();

	//Look into the value between loop cycles
	wnd.gp.LastCounter = wnd.gp.EndCounter;
	wnd.gp.LastCycleCount = wnd.gp.EndCycleCount;
}

void Game::UpdateModel(float32 delta)
{
	Vec2 dir = { 0.0f, 0.0f };

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		dir.x -= 1.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		dir.x += 1.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		prevKeyState = false;
		curKeyState = true;
	}
	if (curKeyState)
	{
		prevKeyState = true;
		curKeyState = false;
	}
	if (prevKeyState)
	{
		dir.y -= 1.0f;
	}
	dir.Normalize();
	mario.SetDirection(dir);
	mario.DoBrickCollision(brick);
	mario.DoBrickCollision(brick1);
	mario.Update(ft.Mark());

}

void Game::ComposeFrame()
{
	const float dt = ft.Mark();
	//Clearing the screen background
	render.ClearScreen(Color(125,125,125));
	//render.DrawImage(0,0, 457, 406, &wnd.GetBitmapStruct(), bmp.PixelPointer, wnd.gp.BytesPerPixel);
	//wnd.gfx.DrawPixel(x, y, 255, 0, 0, 0);
	//render.DrawIntRect(x, y, x+100, y+100, 0xffff00, &bitmapStruct);
	//render.DrawSprite(Player.x, Player.y, { 210, 225, 0, 16 }, wnd.GetScreenRect(), surf);
	//render.DrawFloatRect(Player, game_vector2{ 10,10 }, 0x00ff00, &wnd.GetBitmapStruct());
	brick.Draw(render, Color(255, 0, 0));
	brick1.Draw(render, Color(255, 0, 0));
	mario.Draw(render);
	mario.DrawBound(render, Color(255, 0, 0));

	wnd.UpdateWindowGraphics(); //Leave this untouched
}