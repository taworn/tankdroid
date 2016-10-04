/**
 * @file movable.cxx
 * @desc Movable module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "common.hxx"

Movable::~Movable()
{
}

Movable::Movable()
	: action(ACTION_DEAD)
	, direction(0), nextDirection(0)
	, timePerDead(250), timePerMove(250)
	, timeUsed(0)
	, lock()
	, hp(0), score(0)
	, rof(250), fireTime(250), fireAfterLock(false)
	, hero(false)
	, ani()
	, arena(Game::instance()->getArena())
{
	rect.x = 0;
	rect.y = 0;
	rect.w = 64;
	rect.h = 64;
	ani.add(ACTION_DEAD, 8, 11, Animation::ON_END_HIDDEN, timePerDead);
}

void Movable::move(int dir)
{
	if (!isAlive())
		return;
	if (!lock) {
		if (dir == MOVE_LEFT) {
			action = ACTION_MOVE_LEFT;
			ani.use(ACTION_MOVE_LEFT);
		}
		else if (dir == MOVE_RIGHT) {
			action = ACTION_MOVE_RIGHT;
			ani.use(ACTION_MOVE_RIGHT);
		}
		else if (dir == MOVE_UP) {
			action = ACTION_MOVE_UP;
			ani.use(ACTION_MOVE_UP);
		}
		else if (dir == MOVE_DOWN) {
			action = ACTION_MOVE_DOWN;
			ani.use(ACTION_MOVE_DOWN);
		}

		if (getArena()->getMap()->canMove(this, dir, &target)) {
			distance.x = target.x - rect.x;
			distance.y = target.y - rect.y;
			timeUsed = 0;
			lock = true;
			nextDirection = 0;
		}
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
			if (isHero()) {
				SDL_Rect rect = getRect();
				getArena()->getMap()->checkItems(&rect);
			}
		}
		else {
			rect.x = target.x;
			rect.y = target.y;
			lock = false;
			if (fireAfterLock) {
				fireAfterLock = false;
				fire();
			}
			if (nextDirection > 0)
				move(nextDirection);
		}
	}
}

void Movable::fire()
{
	if (lock) {
		fireAfterLock = true;
		return;
	}

	// not enough fire time?
	if (fireTime < rof)
		return;

	// check direction
	int x, y;
	if (action == ACTION_MOVE_LEFT) {
		x = getX();
		y = getY() + 32;
	}
	else if (action == ACTION_MOVE_RIGHT) {
		x = getX() + 63;
		y = getY() + 32;
	}
	else if (action == ACTION_MOVE_UP) {
		x = getX() + 32;
		y = getY();
	}
	else if (action == ACTION_MOVE_DOWN) {
		x = getX() + 32;
		y = getY() + 63;
	}
	else
		return;

	// if bullets is not full maximum limit?
	if (getArena()->getMap()->addBullet(x, y, action))
		fireTime = 0;
}

void Movable::dead()
{
	action = ACTION_DEAD;
	ani.use(ACTION_DEAD);
}

void Movable::draw(SDL_Renderer *renderer, Sprite *spriteTank, Sprite *spriteMisc, SDL_Rect *viewport, int timeUsed)
{
	SDL_Point point;
	point.x = this->rect.x - viewport->x;
	point.y = this->rect.y - viewport->y;

	SDL_Rect rect;
	rect.x = point.x;
	rect.y = point.y;
	rect.w = 64;
	rect.h = 64;
	if (isAlive())
		ani.draw(renderer, spriteTank, &rect);
	else
		ani.draw(renderer, spriteMisc, &rect);

	fireTime += timeUsed;
}

