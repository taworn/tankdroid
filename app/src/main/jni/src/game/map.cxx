/**
 * @file map.cxx
 * @desc Map module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "common.hxx"
#include <stdio.h>

Map::~Map()
{
	if (imageMap)
		delete[] imageMap;
	if (blockMap)
		delete[] blockMap;
	if (spriteMisc)
		delete spriteMisc;
	if (spriteTank)
		delete spriteTank;
	if (spriteMap)
		delete spriteMap;
}

Map::Map()
	: spriteMap(NULL)
	, spriteTank(NULL)
	, spriteMisc(NULL)
	, width(), height()
	, blockMap(NULL), imageMap(NULL)
	, countTank(0)
	, movHero()
{
	SDL_Renderer *renderer = Game::instance()->getRenderer();
	spriteMap = new Sprite(renderer, TANK_RES("map.png"), 8, 8);
	spriteTank = new Sprite(renderer, TANK_RES("tank.png"), 16, 16);
	spriteMisc = new Sprite(renderer, TANK_RES("tank.png"), 1, 1);
}

bool Map::load(const char *fileName)
{
	// opens file
	SDL_RWops *fp = SDL_RWFromFile(fileName, "rb");
	if (!fp) {
		SDL_Log("Cannot open file.");
		return false;
	}

	// checks file header, 8 bytes
	// header=TANK####, # = ASCII 0
	char check[8] = { 0x54, 0x41, 0x4E, 0x4B, 0x00, 0x00, 0x00, 0x00, };
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

	// reads header information
	// 2 int = width, height
	int32_t buffer[2] = { 0 };
	if (SDL_RWread(fp, buffer, sizeof(int32_t), 2) != 2) {
		SDL_Log("Not enough data to read.");
		SDL_RWclose(fp);
		return false;
	}
	int w = buffer[0];
	int h = buffer[1];

	// reads block data
	size_t size = w * h;
	int32_t *blockRaw = new int32_t[size];
	if (SDL_RWread(fp, blockRaw, sizeof(int32_t), size) != size) {
		delete[] blockRaw;
		SDL_Log("Not enough data to read.");
		SDL_RWclose(fp);
		return false;
	}

	// closes file
	SDL_RWclose(fp);

	// copying
	this->width = w;
	this->height = h;
	if (this->blockMap)
		delete[] this->blockMap;
	if (this->imageMap)
		delete[] this->imageMap;
	this->blockMap = new char[size * 4];
	this->imageMap = new int[size * 4];
	this->countTank = 0;

	// build map with unit as 32x32
	w = width * 2;
	h = height * 2;
	for (int j = 0; j < height; j++) {
		int v = j * 2;
		for (int i = 0; i < width; i++) {
			int u = i * 2;
			char block;
			int image;
			rawToMap(blockRaw[j * width + i], &block, &image);

			int s = image % 4;
			int t = image / 4;
			s *= 2;
			t *= 2;

			blockMap[v * w + u] = block;
			blockMap[v * w + u + 1] = block;
			blockMap[(v + 1) * w + u] = block;
			blockMap[(v + 1) * w + u + 1] = block;
			imageMap[v * w + u] = t * 8 + s;
			imageMap[v * w + u + 1] = t * 8 + s + 1;
			imageMap[(v + 1) * w + u] = (t + 1) * 8 + s;
			imageMap[(v + 1) * w + u + 1] = (t + 1) * 8 + s + 1;
		}
	}

	// add unit tank
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int block = blockRaw[j * width + i];
			if (block >= 100 && block < 140) {
				int x = i * 2 * 32;
				int y = j * 2 * 32;
				int tank = (block % 10) & 0x07;
				int color = (block % 100) / 10;
				movTanks[countTank++].init(x, y, tank, color);
			}
			else if (block == BLOCK_HERO) {
				int x = i * 2 * 32;
				int y = j * 2 * 32;
				movHero.init(x, y);
			}
		}
	}

	if (blockRaw)
		delete[] blockRaw;
	return true;
}

bool Map::canMove(Movable *movable, int direction, SDL_Point *pt)
{
	if (direction == Movable::MOVE_LEFT) {
		int current = movable->getX() / 32;
		int unitY = movable->getY() / 32;
		int next = current - 1;
		if (next >= 0) {
			if (blockIsPass(next, unitY) && blockIsPass(next, unitY + 1)) {
				SDL_Rect rect = movable->getRect();
				rect.x = next * 32;
				if (!blockHasEnemy(&rect)) {
					pt->x = movable->getX() - 32;
					pt->y = movable->getY();
					return true;
				}
			}
		}
	}
	else if (direction == Movable::MOVE_RIGHT) {
		int current = (movable->getX() / 32) + 1;
		int unitY = movable->getY() / 32;
		int next = current + 1;
		if (next < getUnitWidth()) {
			if (blockIsPass(next, unitY) && blockIsPass(next, unitY + 1)) {
				SDL_Rect rect = movable->getRect();
				rect.x = next * 32;
				if (!blockHasEnemy(&rect)) {
					pt->x = movable->getX() + 32;
					pt->y = movable->getY();
					return true;
				}
			}
		}
	}
	else if (direction == Movable::MOVE_UP) {
		int unitX = movable->getX() / 32;
		int current = movable->getY() / 32;
		int next = current - 1;
		if (next >= 0) {
			if (blockIsPass(unitX, next) && blockIsPass(unitX + 1, next)) {
				SDL_Rect rect = movable->getRect();
				rect.y = next * 32;
				if (!blockHasEnemy(&rect)) {
					pt->x = movable->getX();
					pt->y = movable->getY() - 32;
					return true;
				}
			}
		}
	}
	else if (direction == Movable::MOVE_DOWN) {
		int unitX = movable->getX() / 32;
		int current = (movable->getY() / 32) + 1;
		int next = current + 1;
		if (next < getUnitHeight()) {
			if (blockIsPass(unitX, next) && blockIsPass(unitX + 1, next)) {
				SDL_Rect rect = movable->getRect();
				rect.y = next * 32;
				if (!blockHasEnemy(&rect)) {
					pt->x = movable->getX();
					pt->y = movable->getY() + 32;
					return true;
				}
			}
		}
	}
	return false;
}

void Map::draw(SDL_Renderer *renderer, int timeUsed)
{
	SDL_Rect viewport;
	viewport.x = 0;
	viewport.y = 0;
	SDL_GetRendererOutputSize(renderer, &viewport.w, &viewport.h);
	int w = viewport.w / 64 * 64;
	int h = viewport.h / 64 * 64;
	viewport.x = 0;
	viewport.y = 0;
	viewport.w = w;
	viewport.h = h;

	// computes viewport and window
	int x = movHero.getX();
	int y = movHero.getY();
	if (viewport.w < getUnitWidth() * 32) {
		if (x >= (viewport.x + viewport.w / 2 - 32)) {
			viewport.x = x - (viewport.w / 2 - 32);
			if (viewport.x + viewport.w > getUnitWidth() * 32)
				viewport.x = getUnitWidth() * 32 - viewport.w;
		}
	}
	else {
		w = viewport.w - getUnitWidth() * 32;
		viewport.x -= w / 2;

	}
	if (viewport.h < getUnitHeight() * 32) {
		if (y >= (viewport.y + viewport.h / 2 - 32)) {
			viewport.y = y - (viewport.h / 2 - 32);
			if (viewport.y + viewport.h > getUnitHeight() * 32)
				viewport.y = getUnitHeight() * 32 - viewport.h;
		}
	}
	else {
		h = viewport.h - getUnitHeight() * 32;
		viewport.y -= h / 2;
	}

	// draws map
	for (int j = 0; j < getUnitHeight(); j++) {
		for (int i = 0; i < getUnitWidth(); i++) {
			SDL_Rect rect;
			rect.x = i * 32 - viewport.x;
			rect.y = j * 32 - viewport.y;
			rect.w = 32;
			rect.h = 32;
			spriteMap->draw(renderer, imageMap[j * getUnitWidth() + i], &rect);
		}
	}

	// draws enemy tanks
	for (int i = 0; i < countTank; i++) {
		movTanks[i].play(timeUsed);
		movTanks[i].draw(renderer, spriteTank, spriteMisc, &viewport);
	}

	// draws hero
	movHero.play(timeUsed);
	movHero.draw(renderer, spriteTank, spriteMisc, &viewport);

	// draws layer map
	for (int j = 0; j < getUnitHeight(); j++) {
		for (int i = 0; i < getUnitWidth(); i++) {
			int block = blockMap[j * getUnitWidth() + i];
			if (block == BLOCK_TREE) {
				SDL_Rect rect;
				rect.x = i * 32 - viewport.x;
				rect.y = j * 32 - viewport.y;
				rect.w = 32;
				rect.h = 32;
				spriteMap->draw(renderer, imageMap[j * getUnitWidth() + i], &rect);
			}
		}
	}
}

void Map::rawToMap(int raw, char *block, int *image)
{
	switch (raw) {
	default:
	case BLOCK_PASS:
		*block = BLOCK_PASS;
		*image = 0;
		break;
	case BLOCK_TREE:
		*block = BLOCK_TREE;
		*image = 4;
		break;
	case BLOCK_BRICK:
		*block = BLOCK_BRICK;
		*image = 5;
		break;
	case BLOCK_STEEL:
		*block = BLOCK_STEEL;
		*image = 6;
		break;
	case BLOCK_WATER:
		*block = BLOCK_WATER;
		*image = 7;
		break;
	case BLOCK_EAGLE:
		*block = BLOCK_EAGLE;
		*image = 2;
		break;
	}
}

bool Map::blockIsPass(int unitX, int unitY)
{
	int block = blockMap[unitY * getUnitWidth() + unitX];
	switch (block) {
	case BLOCK_PASS:
	case BLOCK_TREE:
		return true;
	default:
		return false;
	}
}

bool Map::blockHasEnemy(SDL_Rect *rect)
{
	int i = 0;
	while (i < countTank) {
		SDL_Rect enemy = movTanks[i].getRect();
		SDL_Rect result;
		if (SDL_IntersectRect(rect, &enemy, &result))
			return true;
		i++;
	}
	return false;
}

