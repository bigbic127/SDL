#include <pingpong/Pingpong.hpp>

int main(int argc, char** argv)
{
	PingPong game;
	bool result = game.Initialize();
	if(result)
		game.Loop();
	return 0;
}