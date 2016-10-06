/**
 * @file tank.cxx
 * @desc Tank module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "common.hxx"

Tank::Tank()
{
}

void Tank::init(int x, int y, int tank, int color)
{
	int base = 0;
	if (color == 3)
		base = 136;
	else if (color == 2)
		base = 8;
	else if (color == 1)
		base = 128;
	base += tank * 16;

	setAction(ACTION_MOVE_DOWN);
	setXY(x, y);
	setROF(1000);
	switch (tank) {
	case 0:
		setTimes(250, 300);
		setHP(1);
		setScore(100);
		break;
	case 1:
		setTimes(250, 300);
		setHP(2);
		setScore(200);
		break;
	case 2:
		setTimes(250, 350);
		setHP(4);
		setScore(300);
		break;
	case 3:
		setTimes(250, 400);
		setHP(8);
		setScore(400);
		break;
	case 4:
		setTimes(250, 300);
		setHP(2);
		setScore(200);
		break;
	case 5:
		setTimes(250, 180);
		setHP(1);
		setScore(250);
		break;
	case 6:
		setTimes(250, 350);
		setHP(3);
		setScore(350);
		break;
	case 7:
		setTimes(250, 500);
		setHP(10);
		setScore(500);
		break;
	}

	getAni()->add(ACTION_MOVE_LEFT, base + 2, base + 4, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->add(ACTION_MOVE_RIGHT, base + 6, base + 8, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->add(ACTION_MOVE_UP, base + 0, base + 2, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->add(ACTION_MOVE_DOWN, base + 4, base + 6, Animation::ON_END_CONTINUE, getTimePerMove());
	getAni()->use(ACTION_MOVE_DOWN);
}

void Tank::ai()
{
	unsigned int t = SDL_GetTicks() % 4;
	t = 1 << t;
	move(t);

	if (canFire())
		fire();
}

void Tank::dead()
{
	Movable::dead();
	getArena()->addScore(getScore());

	int t = (int)SDL_GetTicks() % ITEM_DROP_RATE;
	if (t != 0) {
		SDL_Log("item not drop");
	}
	else {
		int item = (int)SDL_GetTicks() % 3;
		if (getArena()->getMap()->addItem(getX(), getY(), 4 + item))
			SDL_Log("item drop");
		else
			SDL_Log("item will drop, but maximum drop items is limit");
	}
}

