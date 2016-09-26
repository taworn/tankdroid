/**
 * @file sprite.cxx
 * @desc Sprite module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include "sprite.hxx"

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

Sprite::Sprite(SDL_Renderer *renderer, const char *imageFileName, int sliceHorz, int sliceVert)
	: sliceHorz(0), sliceVert(0), width(0), height(0)
{
	surface = IMG_Load(imageFileName);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_assert(surface != NULL);
	SDL_assert(texture != NULL);
	this->sliceHorz = sliceHorz;
	this->sliceVert = sliceVert;
	this->width = surface->w / this->sliceHorz;
	this->height = surface->h / this->sliceVert;
}

void Sprite::draw(SDL_Renderer *renderer, int imageIndex, SDL_Rect *rectTarget)
{
	SDL_Rect rectSource;
	rectSource.x = (imageIndex % sliceHorz) * width;
	rectSource.y = (imageIndex / sliceVert) * height;
	rectSource.w = width;
	rectSource.h = height;
	SDL_RenderCopy(renderer, texture, &rectSource, rectTarget);
}

