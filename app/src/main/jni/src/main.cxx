#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "game.hxx"

int main(int argc, char *argv[])
{
	// initializes SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	// run game framework
	Game *game = new Game();
	game->run();
	delete game;

	// uninitializes SDL
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}

