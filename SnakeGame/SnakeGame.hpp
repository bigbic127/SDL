#pragma once
#include <SDL2/SDL.h>
#include <vector>

struct Vector2
{
    Vector2(float v1 = 0.0f, float v2 = 0.0f):x{v1},y{v2}{}
    float x,y;
};

class SnakeGame
{
    public:
        SnakeGame();
        ~SnakeGame();

    public:
        bool Initialize();
        void Begin();
        void Start();
        void Update();
        void Draw();
        void OnItemPos();
        void DrawSnake();
        float GetLength(Vector2 vec1, Vector2 vec2);
        void DrawItem();
        void ShutDown();
        void Input();
    private:
        bool bIsRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;
        int tickCount;
        SDL_Event event;
        Vector2 vec;
        float drowTime;
        float currentTime;
        Vector2 pos;
        std::vector<Vector2> poses;
        bool bIsItem;
        Vector2 itemPos;
        float speed;
        bool bIsKey;
};
