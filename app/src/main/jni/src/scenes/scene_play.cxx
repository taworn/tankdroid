/**
 * @file scene_play.cxx
 * @desc Play scene module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "scene.hxx"
#include "scene_play.hxx"
#include "../game/common.hxx"

ScenePlay::~ScenePlay()
{
	SDL_Log("ScenePlay::~ScenePlay()");
}

ScenePlay::ScenePlay()
	: arena(Game::instance()->getArena())
{
	SDL_Log("ScenePlay::ScenePlay()");
	arena->startBattle();
}

bool ScenePlay::handleKey(SDL_KeyboardEvent key)
{
	SDL_Log("ScenePlay::handleKey(%d)", key.keysym.sym);
	if (key.keysym.sym == SDLK_SPACE || key.keysym.sym == SDLK_RETURN) {
		arena->getMap()->getHero()->fire();
		return true;
	}
	else if (key.keysym.sym == SDLK_a || key.keysym.sym == SDLK_LEFT) {
		SDL_Log("press left");
		arena->getMap()->getHero()->move(Movable::MOVE_LEFT);
		return true;
	}
	else if (key.keysym.sym == SDLK_d || key.keysym.sym == SDLK_RIGHT) {
		SDL_Log("press right");
		arena->getMap()->getHero()->move(Movable::MOVE_RIGHT);
		return true;
	}
	else if (key.keysym.sym == SDLK_w || key.keysym.sym == SDLK_UP) {
		SDL_Log("press up");
		arena->getMap()->getHero()->move(Movable::MOVE_UP);
		return true;
	}
	else if (key.keysym.sym == SDLK_s || key.keysym.sym == SDLK_DOWN) {
		SDL_Log("press down");
		arena->getMap()->getHero()->move(Movable::MOVE_DOWN);
		return true;
	}
	return false;
}

void ScenePlay::render(int timeUsed)
{
	arena->draw(timeUsed);
}

