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
	, keyTime(0)
{
	SDL_Log("ScenePlay::ScenePlay()");
	arena->startBattle();
}

bool ScenePlay::handleKey(SDL_KeyboardEvent key)
{
#ifdef __ANDROID__
	if (key.keysym.sym == SDLK_SPACE || key.keysym.sym == SDLK_RETURN) {
		arena->getMap()->getHero()->fire();
		return true;
	}
	if (key.keysym.sym == SDLK_a || key.keysym.sym == SDLK_LEFT) {
		arena->getMap()->getHero()->move(Movable::MOVE_LEFT);
		return true;
	}
	else if (key.keysym.sym == SDLK_d || key.keysym.sym == SDLK_RIGHT) {
		arena->getMap()->getHero()->move(Movable::MOVE_RIGHT);
		return true;
	}
	else if (key.keysym.sym == SDLK_w || key.keysym.sym == SDLK_UP) {
		arena->getMap()->getHero()->move(Movable::MOVE_UP);
		return true;
	}
	else if (key.keysym.sym == SDLK_s || key.keysym.sym == SDLK_DOWN) {
		arena->getMap()->getHero()->move(Movable::MOVE_DOWN);
		return true;
	}
#endif
#ifdef _DEBUG
	if (key.keysym.sym == SDLK_z) {
		arena->endBattle();
		Game::instance()->changeScene(Game::SCENE_WIN);
		return true;
	}
	else if (key.keysym.sym == SDLK_c) {
		arena->endBattle();
		Game::instance()->changeScene(Game::SCENE_LOST);
		return true;
	}
#endif
	return false;
}

void ScenePlay::render(int timeUsed)
{
#ifndef __ANDROID__
	const Uint8 *states = SDL_GetKeyboardState(NULL);
	keyTime += timeUsed;
	if (keyTime > 100) {
		if (states[SDL_SCANCODE_SPACE] || states[SDL_SCANCODE_RETURN]) {
			arena->getMap()->getHero()->fire();
		}
		if (states[SDL_SCANCODE_A] || states[SDL_SCANCODE_LEFT]) {
			arena->getMap()->getHero()->move(Movable::MOVE_LEFT);
			keyTime = 0;
		}
		else if (states[SDL_SCANCODE_D] || states[SDL_SCANCODE_RIGHT]) {
			arena->getMap()->getHero()->move(Movable::MOVE_RIGHT);
			keyTime = 0;
		}
		else if (states[SDL_SCANCODE_W] || states[SDL_SCANCODE_UP]) {
			arena->getMap()->getHero()->move(Movable::MOVE_UP);
			keyTime = 0;
		}
		else if (states[SDL_SCANCODE_S] || states[SDL_SCANCODE_DOWN]) {
			arena->getMap()->getHero()->move(Movable::MOVE_DOWN);
			keyTime = 0;
		}
	}
#endif

	arena->draw(timeUsed);
}

