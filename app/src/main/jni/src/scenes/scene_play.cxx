/**
 * @file scene_play.cxx
 * @desc Play scene module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "scene.hxx"
#include "scene_play.hxx"
#include "../game/common.hxx"

ScenePlay::~ScenePlay()
{
	SDL_Log("ScenePlay::~ScenePlay()");
}

ScenePlay::ScenePlay()
{
	SDL_Log("ScenePlay::ScenePlay()");

	Arena *arena = Game::instance()->getArena();
	arena->startBattle();
}

bool ScenePlay::handleKey(SDL_KeyboardEvent key)
{
	SDL_Log("ScenePlay::handleKey(%d)", key.keysym.sym);
	if (key.keysym.sym == SDLK_l) {
		Game::instance()->changeScene(Game::SCENE_LOST);
		return true;
	}
	else if (key.keysym.sym == SDLK_a || key.keysym.sym == SDLK_LEFT) {
		SDL_Log("press left");
		Game::instance()->getArena()->getMap()->getHero()->move(Movable::MOVE_LEFT);
		return true;
	}
	else if (key.keysym.sym == SDLK_d || key.keysym.sym == SDLK_RIGHT) {
		SDL_Log("press right");
		Game::instance()->getArena()->getMap()->getHero()->move(Movable::MOVE_RIGHT);
		return true;
	}
	else if (key.keysym.sym == SDLK_w || key.keysym.sym == SDLK_UP) {
		SDL_Log("press up");
		Game::instance()->getArena()->getMap()->getHero()->move(Movable::MOVE_UP);
		return true;
	}
	else if (key.keysym.sym == SDLK_s || key.keysym.sym == SDLK_DOWN) {
		SDL_Log("press down");
		Game::instance()->getArena()->getMap()->getHero()->move(Movable::MOVE_DOWN);
		return true;
	}
	return false;
}

void ScenePlay::render(unsigned int timeUsed)
{
	Game *game = Game::instance();
	game->getArena()->draw(timeUsed);
}

