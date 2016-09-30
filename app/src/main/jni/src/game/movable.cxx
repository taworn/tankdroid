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
	, direction(0), nextDirection(0)
	, timePerDead(250), timePerMove(150)
	, timeUsed(0)
	, lock()
	, ani()
{
	rect.x = 0;
	rect.y = 0;
	rect.w = 64;
	rect.h = 64;

	ani.add(ACTION_DEAD, 0, 2, Animation::ON_END_HIDDEN, timePerDead);
	ani.add(ACTION_IDLE, 0, 2, Animation::ON_END_CONTINUE, timePerMove);
	ani.add(ACTION_MOVE_LEFT, 2, 4, Animation::ON_END_CONTINUE, timePerMove);
	ani.add(ACTION_MOVE_RIGHT, 6, 8, Animation::ON_END_CONTINUE, timePerMove);
	ani.add(ACTION_MOVE_UP, 0, 2, Animation::ON_END_CONTINUE, timePerMove);
	ani.add(ACTION_MOVE_DOWN, 4, 6, Animation::ON_END_CONTINUE, timePerMove);
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
		nextDirection = 0;
	}
	else {
#ifdef __ANDROID__
		nextDirection = 0;
#else
		nextDirection = dir;
#endif
	}
}

void Movable::play(int timeUsed)
{
	if (!lock)
		return;
	if (isMovingAction()) {
		if (this->timeUsed + timeUsed < timePerMove) {
			int dx = timeUsed * distance.x / timePerMove;
			int dy = timeUsed * distance.y / timePerMove;
			rect.x += dx;
			rect.y += dy;
			this->timeUsed += timeUsed;
		}
		else {
			rect.x = target.x;
			rect.y = target.y;
			lock = false;
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
	ani.draw(renderer, spriteTank, &rect);
}

