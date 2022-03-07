#include <snakeGame/SnakeGame.hpp>

int main(int argc, char** argv)
{
    SnakeGame game;
    bool result = game.Initialize();
    if(result)
        game.Start();
    return 0;
}