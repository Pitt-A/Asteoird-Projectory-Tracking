#include "Game.h"

int main() //Create the game and execute it
{
	Game *game = new Game;
	game->Execute(); //Execute will only exit when the user presses escape or the close button
	delete game;
	return 0;
}