/**
 * @file arena.cxx
 * @desc Battle area module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "common.hxx"

Arena::~Arena()
{
	if (map != NULL)
		delete map;
	SDL_Log("Arena::~Arena()");
}

Arena::Arena()
	: score(0), stage(0)
	, firepower(false)
	, map(NULL)
{
	SDL_Log("Arena::Arena()");
}

void Arena::pickItem(int item)
{
	if (item == ITEM_GUN) {
		firepower = true;
		firepowerTime = ITEM_GUN_TIME;
		SDL_Log("now your firepower is increase 4x, limit time = %d", ITEM_GUN_TIME);
	}
	else if (item == ITEM_HELMET) {
		getMap()->getHero()->decreaseHP(-1);
		SDL_Log("now HP = %d", getMap()->getHero()->getHP());
	}
	else if (item == ITEM_STAR) {
		score += 1000;
		SDL_Log("now score = %d", score);
	}
}

void Arena::startBattle()
{
	if (map != NULL)
		delete map;
	firepower = false;
	map = new Map();
	map->load(TANK_RES("test.map"));
}

void Arena::draw(int timeUsed)
{
	map->draw(Game::instance()->getRenderer(), timeUsed);

	if (firepower) {
		firepowerTime -= timeUsed;
		if (firepowerTime <= 0) {
			firepower = false;
			SDL_Log("now your firepower is normal");
		}
	}
}

