/**
 * @file bullet.cxx
 * @desc Bullet module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "common.hxx"

Bullet::~Bullet()
{
}

Bullet::Bullet(int x, int y, int action)
	: direction(action)
	, timePerMove(50)
	, timeUsed(0)
	, deleted(false)
	, arena(Game::instance()->getArena())
{
	point.x = x;
	point.y = y;
	distance.x = 0;
	distance.y = 0;
	int d = 32;
	if (direction == Movable::ACTION_MOVE_LEFT) {
		imageIndex = 1;
		distance.x -= d;
	}
	else if (direction == Movable::ACTION_MOVE_RIGHT) {
		imageIndex = 3;
		distance.x += d;
	}
	else if (direction == Movable::ACTION_MOVE_UP) {
		imageIndex = 0;
		distance.y -= d;
	}
	else if (direction == Movable::ACTION_MOVE_DOWN) {
		imageIndex = 2;
		distance.y += d;
	}
	target.x = point.x + distance.x;
	target.y = point.y + distance.y;
}

bool Bullet::shouldBeDelete()
{
	if (deleted)
		return true;
	if (!arena->getMap()->inBounds(point.x, point.y))
		return true;
	return false;
}

bool Bullet::check()
{
	if (arena->getMap()->canShot(point.x, point.y, direction, &target)) {
		return true;
	}
	else {
		deleted = true;
		return false;
	}
}

void Bullet::play(int timeUsed)
{
	if (this->timeUsed + timeUsed < timePerMove) {
		int dx = timeUsed * distance.x / timePerMove;
		int dy = timeUsed * distance.y / timePerMove;
		point.x += dx;
		point.y += dy;
		this->timeUsed += timeUsed;
	}
	else {
		point.x = target.x;
		point.y = target.y;
		target.x = point.x + distance.x;
		target.y = point.y + distance.y;
		this->timeUsed = 0;
	}
}

void Bullet::draw(SDL_Renderer *renderer, Sprite *spriteMisc, SDL_Rect *viewport, int timeUsed)
{
	SDL_Point point;
	point.x = this->point.x - viewport->x;
	point.y = this->point.y - viewport->y;

	SDL_Rect rect;
	rect.x = point.x - 16;
	rect.y = point.y - 16;
	rect.w = 32;
	rect.h = 32;
	spriteMisc->draw(renderer, imageIndex, &rect);
}

