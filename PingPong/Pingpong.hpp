#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

struct Vector2
{
	float x,y;
};

struct Ball
{
	Vector2 pos;
	Vector2 vec;
	bool visible;
};

class PingPong
{
	public:
		PingPong();
		~PingPong();
		bool Initialize();
		void Loop();
		void Update();
		void Input();
		void DrawRender();
		void ShutDown();

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		bool mIsRunning;
		int tickCount;
		SDL_Event event;
		Vector2 pointPos;
		Vector2 pointVec;
		float stickPosY;
		int stickDir;
		float stickPosY2;
		int stickDir2;
		std::vector<Ball> balls;

};