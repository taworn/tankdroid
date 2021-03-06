/**
 * @file map.cxx
 * @desc Map module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "common.hxx"
#include <stdio.h>

Map::~Map()
{
	for (int i = 0; i < countBullets; i++)
		delete bullets[i];
	for (int i = 0; i < width * height; i++) {
		if (dataMap[i] != NULL) {
			delete (EAGLE*)dataMap[i];
		}
	}
	if (imageMap)
		delete[] imageMap;
	if (blockMap)
		delete[] blockMap;
	if (dataMap)
		delete[] dataMap;
	if (spriteMiscBig)
		delete spriteMiscBig;
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
	, dataMap(NULL), blockMap(NULL), imageMap(NULL)
	, countTank(0), countBullets(0), countItems(0)
	, movHero()
	, ending(0)
	, arena(Game::instance()->getArena())
{
	SDL_Renderer *renderer = Game::instance()->getRenderer();
	spriteMap = new Sprite(renderer, TANK_RES("map.png"), 8, 8);
	spriteTank = new Sprite(renderer, TANK_RES("tank.png"), 16, 16);
	spriteMisc = new Sprite(renderer, TANK_RES("misc.png"), 8, 8);
	spriteMiscBig = new Sprite(renderer, TANK_RES("misc.png"), 4, 4);
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
	for (int i = 0; i < width * height; i++) {
		if (dataMap[i] != NULL) {
			delete (EAGLE*)dataMap[i];
		}
	}
	if (this->dataMap)
		delete[] this->dataMap;
	if (this->blockMap)
		delete[] this->blockMap;
	if (this->imageMap)
		delete[] this->imageMap;
	for (int i = 0; i < countBullets; i++)
		delete bullets[i];
	for (int i = 0; i < 4; i++)
		items[i].done();
	this->width = w;
	this->height = h;
	this->dataMap = new void*[size];
	this->blockMap = new char[size * 4];
	this->imageMap = new int[size * 4];
	this->countTank = 0;
	this->countBullets = 0;
	this->countItems = 0;
	this->countEagles = 0;
	this->ending = 0;

	// build map with unit as 32x32
	w = width * 2;
	h = height * 2;
	SDL_memset(dataMap, 0, sizeof(void*) * size);
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
			else if (block == BLOCK_EAGLE) {
				EAGLE *e = new EAGLE;
				e->hp = EAGLE_HP;
				e->start = false;
				e->point.x = i * 2 * 32 - 32;
				e->point.y = j * 2 * 32 - 32;
				e->ani.add(0, 4, 6, Animation::ON_END_HIDDEN, 400);
				e->ani.use(0);
				dataMap[j * width + i] = e;
				countEagles++;
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
				if (!blockHasEnemy(movable, &rect)) {
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
				rect.x = (next - 1) * 32;
				if (!blockHasEnemy(movable, &rect)) {
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
				if (!blockHasEnemy(movable, &rect)) {
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
				rect.y = (next - 1) * 32;
				if (!blockHasEnemy(movable, &rect)) {
					pt->x = movable->getX();
					pt->y = movable->getY() + 32;
					return true;
				}
			}
		}
	}
	return false;
}

bool Map::canShot(Bullet *bullet, SDL_Point *pt)
{
	int a = bullet->getAction();
	int x = bullet->getX();
	int y = bullet->getY();
	if (a == Movable::ACTION_MOVE_LEFT) {
		int current = x / 32;
		int unitY = y / 32;
		int next = current - 1;
		if (next >= 0) {
			if (!blockIsShootPass(bullet, next, unitY - 1))
				return false;
			if (!blockIsShootPass(bullet, next, unitY))
				return false;
		}
	}
	else if (a == Movable::ACTION_MOVE_RIGHT) {
		int current = x / 32;
		int unitY = y / 32;
		int next = current + 1;
		if (next < getUnitWidth()) {
			if (!blockIsShootPass(bullet, next, unitY - 1))
				return false;
			if (!blockIsShootPass(bullet, next, unitY))
				return false;
		}
	}
	else if (a == Movable::ACTION_MOVE_UP) {
		int unitX = x / 32;
		int current = y / 32;
		int next = current - 1;
		if (next >= 0) {
			if (!blockIsShootPass(bullet, unitX - 1, next))
				return false;
			if (!blockIsShootPass(bullet, unitX, next))
				return false;
		}
	}
	else if (a == Movable::ACTION_MOVE_DOWN) {
		int unitX = x / 32;
		int current = y / 32;
		int next = current + 1;
		if (next < getUnitHeight()) {
			if (!blockIsShootPass(bullet, unitX - 1, next))
				return false;
			if (!blockIsShootPass(bullet, unitX, next))
				return false;
		}
	}
	if (blockShootEnemy(bullet, x, y))
		return false;
	return true;
}

bool Map::addBullet(int x, int y, int action, bool hero)
{
	if (countBullets >= 64)
		return false;
	bullets[countBullets++] = new Bullet(x, y, action, hero);
	Game::instance()->playShot();
	return true;
}

bool Map::addItem(int x, int y, int imageIndex)
{
	if (countItems >= 4)
		return false;
	int i = 0;
	while (i < 4) {
		if (!items[i].isHappen())
			break;
		else
			i++;
	}
	items[i].init(x, y, imageIndex);
	countItems++;
	return true;
}

bool Map::checkItems(SDL_Rect *rect)
{
	int i = 0;
	while (i < 4) {
		if (items[i].isHappen()) {
			SDL_Rect r;
			r.x = items[i].getX();
			r.y = items[i].getY();
			r.w = 64;
			r.h = 64;
			r.x += 8;
			r.y += 8;
			r.w -= 16;
			r.h -= 16;
			SDL_Rect result;
			if (SDL_IntersectRect(rect, &r, &result)) {
				int item = items[i].getImageIndex() - 4;
				items[i].done();
				countItems--;
				arena->pickItem(item);
				Game::instance()->playItem();
				return true;
			}
		}
		i++;
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

	// draws eagles
	for (int i = 0; i < width * height; i++) {
		if (dataMap[i] != NULL) {
			EAGLE *e = (EAGLE*)dataMap[i];
			if (e->start) {
				SDL_Rect rect;
				rect.x = e->point.x - viewport.x;
				rect.y = e->point.y - viewport.y;
				rect.w = 128;
				rect.h = 128;
				e->ani.draw(renderer, spriteMiscBig, &rect);
			}
		}
	}

	// draws items
	for (int i = 0; i < 4; i++) {
		items[i].draw(renderer, spriteMisc, &viewport);
	}

	// draws enemy tanks
	for (int i = 0; i < countTank; i++) {
		movTanks[i].ai();
		movTanks[i].play(timeUsed);
		movTanks[i].draw(renderer, spriteTank, spriteMisc, &viewport, timeUsed);
	}

	// draws hero
	movHero.play(timeUsed);
	movHero.draw(renderer, spriteTank, spriteMisc, &viewport, timeUsed);

	// draws bullets
	for (int i = 0; i < countBullets; i++) {
		// move bullet
		if (bullets[i]->check()) {
			bullets[i]->play(timeUsed);
			bullets[i]->draw(renderer, spriteMisc, &viewport, timeUsed);
		}
	}
	int i = countBullets;
	while (i > 0) {
		i--;
		if (bullets[i]->shouldBeDelete()) {
			delete bullets[i];
			for (int j = i; j < countBullets - 1; j++)
				bullets[j] = bullets[j + 1];
			countBullets--;
		}
	}

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

	if (!movHero.isAlive())
		ending = -1;
	if (ending)
		endingTime += timeUsed;
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

bool Map::blockHasEnemy(Movable *movable, SDL_Rect *rect)
{
	if (!movable->isHero()) {
		SDL_Rect enemy = movHero.getRect();
		SDL_Rect result;
		if (SDL_IntersectRect(rect, &enemy, &result))
			return true;
		int i = 0;
		while (i < countTank) {
			if ((Movable*)&movTanks[i] != movable) {
				if (movTanks[i].isAlive()) {
					enemy = movTanks[i].getRect();
					if (SDL_IntersectRect(rect, &enemy, &result))
						return true;
				}
			}
			i++;
		}
	}
	else {
		int i = 0;
		while (i < countTank) {
			if (movTanks[i].isAlive()) {
				SDL_Rect enemy = movTanks[i].getRect();
				SDL_Rect result;
				if (SDL_IntersectRect(rect, &enemy, &result))
					return true;
			}
			i++;
		}
	}
	return false;
}

bool Map::blockIsShootPass(Bullet *bullet, int unitX, int unitY)
{
	int block = blockMap[unitY * getUnitWidth() + unitX];
	switch (block) {
	default:
	case BLOCK_PASS:
	case BLOCK_TREE:
	case BLOCK_WATER:
		return true;

	case BLOCK_BRICK:
		// remove block
		blockMap[unitY * getUnitWidth() + unitX] = BLOCK_PASS;
		imageMap[unitY * getUnitWidth() + unitX] = 0;
		return false;

	case BLOCK_STEEL:
		return false;

	case BLOCK_EAGLE:
		if (bullet->isFromHero()) {
			// eagle reduce HP
			int mapBlock = (unitY / 2) * width + (unitX / 2);
			EAGLE *e = (EAGLE*)dataMap[mapBlock];
			int dec = arena->boostFirepower() ? 4 : 1;
			e->hp -= dec;
			if (e->hp <= 0) {
				int x = (unitX / 2) * 2;
				int y = (unitY / 2) * 2;
				int s = 3 % 4;
				int t = 3 / 4;
				s *= 2;
				t *= 2;
				blockMap[y * getUnitWidth() + x] = BLOCK_PASS;
				blockMap[y * getUnitWidth() + x + 1] = BLOCK_PASS;
				blockMap[(y + 1) * getUnitWidth() + x] = BLOCK_PASS;
				blockMap[(y + 1) * getUnitWidth() + x + 1] = BLOCK_PASS;
				imageMap[y * getUnitWidth() + x] = t * 8 + s;
				imageMap[y * getUnitWidth() + x + 1] = t * 8 + s + 1;
				imageMap[(y + 1) * getUnitWidth() + x] = (t + 1) * 8 + s;
				imageMap[(y + 1) * getUnitWidth() + x + 1] = (t + 1) * 8 + s + 1;
				arena->addScore(1000);
				e->start = true;
				countEagles--;
				SDL_Log("eagle is dead, count left: %d", countEagles);
				if (countEagles <= 0)
					endingScene();
			}
		}
		return false;
	}
}

bool Map::blockShootEnemy(Bullet *bullet, int x, int y)
{
	if (!bullet->isFromHero()) {
		SDL_Rect enemy = movHero.getRect();
		enemy.x += 16;
		enemy.y += 16;
		enemy.w -= 32;
		enemy.h -= 32;
		if (x >= enemy.x && x < enemy.x + enemy.w && y >= enemy.y && y < enemy.y + enemy.h) {
			movHero.decreaseHP(1);
			if (movHero.getHP() <= 0)
				movHero.dead();
			return true;
		}
	}
	else {
		int i = 0;
		while (i < countTank) {
			if (movTanks[i].isAlive()) {
				SDL_Rect enemy = movTanks[i].getRect();
				enemy.x += 16;
				enemy.y += 16;
				enemy.w -= 32;
				enemy.h -= 32;
				if (x >= enemy.x && x < enemy.x + enemy.w && y >= enemy.y && y < enemy.y + enemy.h) {
					int dec = arena->boostFirepower() ? 4 : 1;
					movTanks[i].decreaseHP(dec);
					if (movTanks[i].getHP() <= 0)
						movTanks[i].dead();
					return true;
				}
			}
			i++;
		}
	}
	return false;
}

void Map::endingScene()
{
	int i = 0;
	while (i < countTank) {
		if (movTanks[i].isAlive()) {
			movTanks[i].decreaseHP(999);
			movTanks[i].dead();
		}
		i++;
	}

	SDL_Log("all of enemy tanks is died :)");

	ending = 1;
	endingTime = 0;
}

