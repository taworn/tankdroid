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
	delete drawNumber;
	SDL_DestroyTexture(textureHP);
	SDL_FreeSurface(surfaceHP);
	SDL_DestroyTexture(textureScore);
	SDL_FreeSurface(surfaceScore);
	SDL_Log("Arena::~Arena()");
}

Arena::Arena()
	: score(0), stage(0)
	, firepower(false)
	, map(NULL)
{
	SDL_Log("Arena::Arena()");
	Game *game = Game::instance();
	surfaceScore = TTF_RenderUTF8_Blended(game->getFontMedium(), "Score: ", { 0xFF, 0xFF, 0xFF });
	textureScore = SDL_CreateTextureFromSurface(game->getRenderer(), surfaceScore);
	surfaceHP = TTF_RenderUTF8_Blended(game->getFontMedium(), "HP: ", { 0xFF, 0xFF, 0xFF });
	textureHP = SDL_CreateTextureFromSurface(game->getRenderer(), surfaceHP);
	drawNumber = new DrawNumber(game->getRenderer(), game->getFontMedium(), { 0xFF, 0xFF, 0xFF });
}

void Arena::restart()
{
	score = 0;
	stage = 0;
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

int Arena::endBattle()
{
	if (!map->isEnd()) {
		return 0;
	}
	else {
		if (!map->isWin())
			return -1;
		else
			return 1;
	}
}

void Arena::draw(int timeUsed)
{
	Game *game = Game::instance();
	SDL_Rect rect;
	SDL_Point point;
	int w, h;
	SDL_GetRendererOutputSize(game->getRenderer(), &w, &h);

	if (!map->isEnd()) {
		map->draw(game->getRenderer(), timeUsed);

		if (firepower) {
			firepowerTime -= timeUsed;
			if (firepowerTime <= 0) {
				firepower = false;
				SDL_Log("now your firepower is normal");
			}
		}
	}

	// draws score
	rect.x = 0;
	rect.y = 0;
	rect.w = surfaceScore->w;
	rect.h = surfaceScore->h;
	SDL_RenderCopy(game->getRenderer(), textureScore, NULL, &rect);
	point.x = surfaceScore->w;
	point.y = 0;
	drawNumber->draw(game->getRenderer(), getScore(), 7, point);

	// draws HP
	point.x = w - (drawNumber->getWidth() * 3);
	point.y = 0;
	drawNumber->draw(game->getRenderer(), map->getHero()->getHP(), 3, point);
	rect.x = point.x - surfaceHP->w;
	rect.y = 0;
	rect.w = surfaceHP->w;
	rect.h = surfaceHP->h;
	SDL_RenderCopy(game->getRenderer(), textureHP, NULL, &rect);
}

