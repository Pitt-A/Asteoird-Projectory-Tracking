#include "Game.h"

int main()
{
	Game *game = new Game;
	game->Execute();
	delete game;
	return 0;
}