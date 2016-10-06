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
#ifndef __ANDROID__
#include <windows.h>
#include <shlobj.h>
#endif

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
	load();
}

void Arena::save()
{
#ifdef __ANDROID__
	char file[256] = { 0 };
	strcpy(file, "tankdroid.sav");
#else
	char file[MAX_PATH] = { 0 };
	SHGetFolderPathA(0, CSIDL_LOCAL_APPDATA, 0, 0, file);
	strcat(file, "\\tankman.sav");
#endif
	SDL_RWops *fp = SDL_RWFromFile(file, "wb");
	if (!fp) {
		SDL_Log("Cannot open file.");
		return;
	}

	// writes header, 8 bytes
	// header=TANKSAVE
	char check[8] = { 0x54, 0x41, 0x4E, 0x4B, 0x53, 0x41, 0x56, 0x45, };
	SDL_RWwrite(fp, check, 1, 8);

	// writes data
	SDL_RWwrite(fp, &score, sizeof(int32_t), 1);
	SDL_RWwrite(fp, &stage, sizeof(int32_t), 1);

	// closes file
	SDL_RWclose(fp);
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

	char buffer[64] = { 0 };
	SDL_snprintf(buffer, 63, TANK_RES("debug0.map"), stage);
	//SDL_snprintf(buffer, 63, TANK_RES("%04d.map"), stage);
	map = new Map();
	map->load(buffer);
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

bool Arena::load()
{
#ifdef __ANDROID__
	char file[256] = { 0 };
	strcpy(file, "tankdroid.sav");
#else
	char file[MAX_PATH] = { 0 };
	SHGetFolderPathA(0, CSIDL_LOCAL_APPDATA, 0, 0, file);
	strcat(file, "\\tankman.sav");
#endif
	SDL_RWops *fp = SDL_RWFromFile(file, "rb");
	if (!fp) {
		SDL_Log("Cannot open file.");
		return false;
	}

	// checks file header, 8 bytes
	// header=TANKSAVE
	char check[8] = { 0x54, 0x41, 0x4E, 0x4B, 0x53, 0x41, 0x56, 0x45, };
	char header[8] = { 0 };
	if (SDL_RWread(fp, header, 1, 8) != 8) {
		SDL_Log("Header is not acceptable.");
		SDL_RWclose(fp);
		return false;
	}
	for (int i = 0; i < 8; i++) {
		if (header[i] != check[i]) {
			SDL_Log("Header is not acceptable.");
			SDL_RWclose(fp);
			return false;
		}
	}

	// reads data
	int32_t buffer[2] = { 0 };
	if (SDL_RWread(fp, buffer, sizeof(int32_t), 2) != 2) {
		SDL_Log("Not enough data to read.");
		SDL_RWclose(fp);
		return false;
	}
	int score = buffer[0];
	int stage = buffer[1];

	// closes file
	SDL_RWclose(fp);

	// checks ranges
	if (score < 0)
		score = 0;
	else if (score > 9999999)
		score = 9999999;
	if (stage < 0)
		stage = 0;
	else if (stage >= STAGE_COUNT)
		stage = STAGE_COUNT - 1;

	// copies
	this->score = score;
	this->stage = stage;
	return true;
}

