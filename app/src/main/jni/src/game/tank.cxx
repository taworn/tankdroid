/**
 * @file tank.cxx
 * @desc Tank module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "common.hxx"

Tank::Tank()
{
}

void Tank::init(int x, int y, int tank, int color)
{
	setXY(x, y);
	setTimes(250, 250);
	int base = 0;

	if (color == 3)
		base = 136;
	else if (color == 2)
		base = 8;
	else if (color == 1)
		base = 128;
	base += tank * 16;

	getAni()->add(ACTION_DEAD, base + 4, base + 6, Animation::ON_END_HIDDEN, getTimePerDead());
	getAni()->add(ACTION_IDLE, base + 4, base + 6, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->add(ACTION_MOVE_LEFT, base + 2, base + 4, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->add(ACTION_MOVE_RIGHT, base + 6, base + 8, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->add(ACTION_MOVE_UP, base + 0, base + 2, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->add(ACTION_MOVE_DOWN, base + 4, base + 6, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->use(ACTION_IDLE);
}

