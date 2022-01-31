#include "pingpong/Pingpong.hpp"
#include "stdlib.h"
#include "time.h"

PingPong::PingPong():window(nullptr),renderer(nullptr),mIsRunning(true),tickCount(0)
{
}

PingPong::~PingPong()
{
}

bool PingPong::Initialize()
{
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if(result)
    {
        SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("PingPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,0);
    if(!window)
    {
        SDL_Log("Faield to create window : %s", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        SDL_Log("Faield to create renderer : %s", SDL_GetError());
        return false;
    }
    pointPos.x = 800/2;
    pointPos.y = 600/2; 
    srand (time(NULL));
    int x = 50+rand()%50;
    srand (time(NULL)+time(NULL));
    int y = 30+rand()%50;
    pointVec.x = x>50?x:x*-1;
    pointVec.y = y>40?y:y*-1;
    stickPosY = 600/2;
    stickDir = 0;
    stickPosY2 = 600/2;
    stickDir2 = 0;

    for(int i = 0;i<5;i++)
    {   
        Ball ball;
        ball.pos.x = 800/2;
        ball.pos.y = 600/2;
        srand (time(NULL)*(i+1));
        int x = 50+rand()%50;
        srand (time(NULL)+time(NULL)*(i+1));
        int y = 30+rand()%50;
        ball.vec.x = x>75?x:x*-1;
        ball.vec.y = y>55?y:y*-1;
        ball.visible = true;
        balls.push_back(ball);
    }
    return true;
}

void PingPong::Loop()
{
    while(mIsRunning)
    {
        Input();
        Update();
        DrawRender();
    }
    ShutDown();
}

void PingPong::Update()
{
    while(!SDL_TICKS_PASSED(SDL_GetTicks(),tickCount + 16));
    float deltaTime = SDL_GetTicks() - tickCount / 1000.f;
    if(deltaTime > 0.05f)
        deltaTime = 0.05f;
    tickCount = SDL_GetTicks();

    if(stickDir>0 && stickPosY > 80)
        stickPosY -= deltaTime * 80;
    else if(stickDir<0 && stickPosY < 520)
        stickPosY += deltaTime * 80;

    if(stickDir2>0 && stickPosY2 > 80)
        stickPosY2 -= deltaTime * 80;
    else if(stickDir2<0 && stickPosY2 < 520)
        stickPosY2 += deltaTime * 80;

    int ballCount = 0;
    for(Ball &ball : balls)
    {
        if(!ball.visible)
        {
            ballCount++;
            continue;
        }
        ball.pos.x += ball.vec.x * deltaTime;
        ball.pos.y += ball.vec.y * deltaTime;

        if((ball.pos.x < 30 && (ball.pos.y + 10 < stickPosY - 60 || ball.pos.y - 10 > stickPosY + 60)) || (ball.pos.x > 770 && (ball.pos.y + 10 < stickPosY2 - 60 || ball.pos.y - 10 > stickPosY2 + 60)) )
                ball.visible = false;

        if(ball.pos.x > 800 - 30 || ball.pos.x < 0 + 30)
            ball.vec.x *= -1.02f;
        else if(ball.pos.y > 600 -30 || ball.pos.y < 0 + 30)
            ball.vec.y *= -1.02f;
    }

    if (ballCount ==5)
    {
        for(int i = 0;i<5;i++)
        {   
            Ball &ball = balls[i];
            ball.pos.x = 800/2;
            ball.pos.y = 600/2;
            srand (time(NULL)*(i+1));
            int x = 50+rand()%50;
            srand (time(NULL)+time(NULL)*(i+1));
            int y = 30+rand()%50;
            ball.vec.x = x>75?x:x*-1;
            ball.vec.y = y>55?y:y*-1;
            ball.visible = true;
        }
    }
}

void PingPong::Input()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    mIsRunning = false;
                    break;
            default:
                break;
        }
    }
    stickDir = 0;
    stickDir2 = 0;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W])
        stickDir = 1;
    if(state[SDL_SCANCODE_S])
        stickDir = -1;
    if(state[SDL_SCANCODE_UP])
        stickDir2 = 1;
    if(state[SDL_SCANCODE_DOWN])
        stickDir2 = -1;

}

void PingPong::DrawRender()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_Rect wall{
        0,
        0,
        0,
        0
    };
    wall.x = 0;
    wall.y = 580;
    wall.w = 800;
    wall.h = 20;
    SDL_RenderFillRect(renderer,&wall);
    wall.x = 0;
    wall.y = 0;
    wall.w = 800;
    wall.h = 20;
    SDL_RenderFillRect(renderer,&wall);

    SDL_SetRenderDrawColor(renderer, 255,0,255,255);
    SDL_Rect point{
        0,
        0,
        20,
        20
    };
    for(Ball ball : balls)
    {
        if(!ball.visible)
            continue;
        point.x = ball.pos.x - 10;
        point.y = ball.pos.y - 10;
        SDL_RenderFillRect(renderer,&point);
    }
    
    SDL_SetRenderDrawColor(renderer, 0,255,255,255);
    SDL_Rect stick{
        0,
        0,
        20,
        120
    };
    stick.y = static_cast<int>(stickPosY - 60);
    SDL_RenderFillRect(renderer,&stick);

    SDL_SetRenderDrawColor(renderer, 255,255,0,255);
    stick.x = 780;
    stick.y =static_cast<int>(stickPosY2 - 60);
    stick.w = 20;
    SDL_RenderFillRect(renderer,&stick);

    SDL_RenderPresent(renderer);
}

void PingPong::ShutDown()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}