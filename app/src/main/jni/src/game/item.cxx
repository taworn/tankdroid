/**
 * @file item.cxx
 * @desc Item module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "common.hxx"

Item::~Item()
{
}

Item::Item() : happen(false)
{
}

void Item::init(int x, int y, int imageIndex)
{
	this->happen = true;
	this->x = x;
	this->y = y;
	this->imageIndex = imageIndex;
}

void Item::draw(SDL_Renderer *renderer, Sprite *spriteMisc, SDL_Rect *viewport)
{
	if (happen) {
		SDL_Point point;
		point.x = x - viewport->x;
		point.y = y - viewport->y;

		SDL_Rect rect;
		rect.x = point.x;
		rect.y = point.y;
		rect.w = 64;
		rect.h = 64;
		spriteMisc->draw(renderer, imageIndex, &rect);
	}
}

