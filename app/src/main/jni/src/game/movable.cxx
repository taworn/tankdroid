/**
 * @file movable.cxx
 * @desc Movable module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "common.hxx"

Movable::~Movable()
{
}

Movable::Movable()
	: action(ACTION_IDLE)
	, direction(0)
	, lock(false)
	, timeBase(250), timePerDistance(100)
	, ani()
{
	rect.x = 0;
	rect.y = 0;
	rect.w = 64;
	rect.h = 64;

	ani.add(ACTION_DEAD, 0, 2, Animation::ON_END_HIDDEN, timeBase);
	ani.add(ACTION_DYING, 0, 2, Animation::ON_END_KEEP_LAST_FRAME, timeBase);
	ani.add(ACTION_IDLE, 0, 2, Animation::ON_END_CONTINUE, timeBase);
	ani.add(ACTION_MOVE_LEFT, 2, 4, Animation::ON_END_CONTINUE, timePerDistance);
	ani.add(ACTION_MOVE_RIGHT, 6, 8, Animation::ON_END_CONTINUE, timePerDistance);
	ani.add(ACTION_MOVE_UP, 0, 2, Animation::ON_END_CONTINUE, timePerDistance);
	ani.add(ACTION_MOVE_DOWN, 4, 6, Animation::ON_END_CONTINUE, timePerDistance);
	ani.use(ACTION_IDLE);
}

void Movable::move(int dir)
{
	if (!isAlive())
		return;
	if (!lock) {
		distance.x = 0;
		distance.y = 0;
		if (dir == MOVE_LEFT) {
			action = ACTION_MOVE_LEFT;
			ani.use(ACTION_MOVE_LEFT);
			distance.x = -32;
		}
		else if (dir == MOVE_RIGHT) {
			action = ACTION_MOVE_RIGHT;
			ani.use(ACTION_MOVE_RIGHT);
			distance.x = 32;
		}
		else if (dir == MOVE_UP) {
			action = ACTION_MOVE_UP;
			ani.use(ACTION_MOVE_UP);
			distance.y = -32;
		}
		else if (dir == MOVE_DOWN) {
			action = ACTION_MOVE_DOWN;
			ani.use(ACTION_MOVE_DOWN);
			distance.y = 32;
		}
		target.x = rect.x + distance.x;
		target.y = rect.y + distance.y;
		timeUsed = 0;
		lock = true;

		// can move
		//Map *map = Game::instance()->getArena()->getMap();

		// for now, move at warp speed ^_^
		//rect.x += distance.x;
		//rect.y += distance.y;
		nextDirection = 0;
	}
	else {
		nextDirection = dir;
	}
}

void Movable::play(unsigned int timeUsed)
{
	if (!lock)
		return;
	if (action >= ACTION_MOVE_LEFT && action <= ACTION_MOVE_DOWN) {
		if (this->timeUsed + (int)timeUsed < timePerDistance) {
			int dx = (int)timeUsed * distance.x / (int)timePerDistance;
			int dy = (int)timeUsed * distance.y / (int)timePerDistance;
			rect.x += dx;
			rect.y += dy;
			this->timeUsed += (int)timeUsed;
		}
		else {
			rect.x = target.x;
			rect.y = target.y;
			lock = false;
			action = ACTION_IDLE;
			if (nextDirection > 0)
				move(nextDirection);
		}
	}
}

void Movable::draw(SDL_Renderer *renderer, Sprite *spriteTank, Sprite *spriteMisc, SDL_Rect *viewport)
{
	SDL_Point point;
	point.x = this->rect.x - viewport->x;
	point.y = this->rect.y - viewport->y;
	SDL_Rect rect;
	rect.x = point.x;
	rect.y = point.y;
	rect.w = 64;
	rect.h = 64;
	//spriteTank->draw(renderer, 0, &rect);
	ani.draw(renderer, spriteTank, &rect);
}

