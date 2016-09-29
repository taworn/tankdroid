/**
 * @file arena.cxx
 * @desc Battle area module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
	, map(NULL)
{
	SDL_Log("Arena::Arena()");
}

void Arena::startBattle()
{
	if (map != NULL)
		delete map;
	map = new Map();
	map->load(TANK_RES("debug3.map"));
}

void Arena::draw(unsigned int timeUsed)
{
	map->draw(Game::instance()->getRenderer(), timeUsed);
}

