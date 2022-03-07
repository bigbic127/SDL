#include <snakeGame/SnakeGame.hpp>
#include <time.h>
#include <math.h>
#include <stdlib.h>

SnakeGame::SnakeGame():window(nullptr),renderer(nullptr),tickCount(0),bIsRunning(true)
{
    //초기 위치값
    pos.x = 400;
    pos.y = 300;
    vec.x = 1.0f;
    vec.y = 0.0f;
    currentTime = 0.0f;
    drowTime = 0.0f;
    bIsItem = true;
    speed = 10.0f;
    bIsKey = false;
}

SnakeGame::~SnakeGame()
{

}

bool SnakeGame::Initialize()
{
    //SDL 초기화
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if(result)
    {
        SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
        return false;
    }
    //SDL 윈도우 생성
    window = SDL_CreateWindow("SnakeGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if(!window)
    {
        SDL_Log("Failed to create window : %s", SDL_GetError());
        return false;
    }
    //화면에 그리기위한 렌더러 생성 flags(하드웨어 가속/화면 새로고침 빈도와 동기화)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        SDL_Log("Failed to create renderer : %s", SDL_GetError());
        return false;
    }
    //초기화 성공
    SDL_Log("Success Initialize");
    return true;
}

void SnakeGame::Begin()
{
    //재시작
    poses.clear();
    poses.push_back(Vector2(400.0f,300.0f));
    poses.push_back(Vector2(400.0f,300.0f));
    poses.push_back(Vector2(400.0f,300.0f));
    OnItemPos();
    pos.x = 400;
    pos.y = 300;
    vec.x = 1.0f;
    vec.y = 0.0f;
    currentTime = 0.0f;
    drowTime = 0.0f;
    bIsItem = true;
    speed = 10.0f;
    bIsKey = false;
}

void SnakeGame::Start()
{
    //초기 캐릭터 및 아이템 위치 설정  
    poses.push_back(Vector2(400.0f,300.0f));
    poses.push_back(Vector2(400.0f,300.0f));
    poses.push_back(Vector2(400.0f,300.0f));
    OnItemPos();

    while(bIsRunning)
    {
        Update();
        Input();
        Draw();
    }
    ShutDown();
}

void SnakeGame::Update()
{
    //고정 프레임
    while(!SDL_TICKS_PASSED(SDL_GetTicks(),tickCount+16));
    float deltaTime = SDL_GetTicks() - tickCount / 1000.0f;
    if(deltaTime > 0.05f)
        deltaTime = 0.05f;
    tickCount = SDL_GetTicks();
    currentTime += speed*deltaTime;

    //방향키(Up,Down,Right,Left)로 움직임 설정 && 순차적인 움직임이 아닌 일정한 움직임으로 이동(10픽셀)
    if(int(currentTime)%10 == 0)
    {
        if(int(currentTime) > drowTime)
        {
            bIsKey = false;
            drowTime = currentTime + 1.0f;
            for(int i = poses.size(); i >= 0; i--)
            {
                Vector2& ppos = poses[i];
                if(i == 0)
                    ppos = pos;
                else
                    ppos = poses[i-1];
            }
            if(vec.x == 0)
                pos.x += 0;
            else if(vec.x > 0)
                pos.x += 10;
            else if(vec.x < 0)
                pos.x -= 10;
            if(vec.y == 0)
                pos.y += 0;
            else if(vec.y > 0)
                pos.y += 10;
            else if(vec.y < 0)
                pos.y -= 10;

            //게임오버
            if((pos.x > 790 || pos.x <= 10) || (pos.y <= 10 || pos.y > 590))
            {
                Begin();
                SDL_Log("GameOver");
            }
            if(currentTime>20.0f)
            {
                for(int i = 3; i < poses.size() ;i++)
                {
                    if(GetLength(pos,poses[i]) < 5.0)
                    {
                        Begin();
                        SDL_Log("GameOver %d",i);
                    }
                }
            }

            //아이템 위치 설정
            if((GetLength(pos,itemPos) < 5.0) && bIsItem)
            {
                bIsItem = false;
                Vector2 newPos = pos;
                poses.push_back(newPos);
                OnItemPos();
                bIsItem = true;
                if(speed < 50.0f)
                    speed +=1.0f;
            }
        }
    }
}

void SnakeGame::Input()
{    
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                if(!bIsKey)
                {
                    bIsKey = true;
                    if(event.key.keysym.sym == SDLK_UP)
                    {   
                        vec.x = 0.0f;
                        if(vec.y == 0)
                            vec.y = -1.0f;
                    }
                    else if(event.key.keysym.sym == SDLK_DOWN)
                    {
                        vec.x = 0.0f;
                        if(vec.y == 0)
                            vec.y = 1.0f;
                    }
                    else if(event.key.keysym.sym == SDLK_LEFT)
                    {
                        if(vec.x == 0)
                            vec.x = -1.0f;
                        vec.y = 0.0f;
                    }
                    else if(event.key.keysym.sym == SDLK_RIGHT)
                    {
                        if(vec.x == 0)
                            vec.x = 1.0f;
                        vec.y = 0.0f;
                    }
                }
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    bIsRunning = false;
                break;
            case SDL_QUIT:
                bIsRunning = false;
                break;
            default:
                break;
        }
    }
}

void SnakeGame::OnItemPos()
{
    srand (time(NULL));
    int x = (rand()%400)+ pos.x;
    x = 20 + (x%760);
    srand (time(NULL)+time(NULL));
    int y = (rand()%300)+ pos.y;
    y = 20 + (y%560);
    itemPos.x = x - (x%20);
    itemPos.y = y - (y%20);
}

void SnakeGame::Draw()
{
    //배경 초기화
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //화면 벽 그리기
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    int wallLength = 10;
    SDL_Rect wall{
        0, 0, 0, 0
    };
    int windowSizeX = 800, windowSizeY = 600;
    int windowPosX = 0, windowPosY = 0;
    
    //위쪽 벽 그리기
    wall.x = windowPosX;
    wall.y = windowPosY;
    wall.w = windowSizeX;
    wall.h = wallLength;
    SDL_RenderFillRect(renderer, &wall);
    //아래 벽 그리기
    wall.x = windowPosX;
    wall.y = windowSizeY - wallLength;
    wall.w = windowSizeX;
    wall.h = wallLength;
    SDL_RenderFillRect(renderer, &wall);
    //왼쪽 벽 그리기
    wall.x = windowPosX;
    wall.y = windowPosY;
    wall.w = wallLength;
    wall.h = windowSizeY;
    SDL_RenderFillRect(renderer, &wall);
    //오른쪽 벽 그리기
    wall.x = windowSizeX - wallLength;
    wall.y = windowPosY;
    wall.w = wallLength;
    wall.h = windowSizeY;
    SDL_RenderFillRect(renderer, &wall);

    if(bIsItem)
        DrawItem();

    DrawSnake();

    SDL_RenderPresent(renderer);
}

void SnakeGame::DrawSnake()
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect snake{
        0,0,0,0
    };

    snake.x = pos.x - 10;
    snake.y = pos.y - 10;
    snake.w = 10;
    snake.h = 10;
    SDL_RenderFillRect(renderer, &snake);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

    for(Vector2 ppos:poses)
    {
        snake.x = ppos.x - 10;
        snake.y = ppos.y - 10;
        snake.w = 10;
        snake.h = 10;
        SDL_RenderFillRect(renderer, &snake);
    }
}

void SnakeGame::DrawItem()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    SDL_Rect item{
        0,0,0,0
    };

    item.x = itemPos.x - 10;
    item.y = itemPos.y - 10;
    item.w = 10;
    item.h = 10;
    SDL_RenderFillRect(renderer, &item);
}

float SnakeGame::GetLength(Vector2 vec1, Vector2 vec2)
{
    float result = sqrt(pow(vec2.x - vec1.x,2) + pow(vec2.y - vec1.y,2));
    return abs(result);
}

void SnakeGame::ShutDown()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}