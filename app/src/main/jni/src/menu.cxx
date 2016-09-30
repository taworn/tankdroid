/**
 * @file menu.cxx
 * @desc Simple menu module.
 */
#include <SDL.h>
#include <SDL_ttf.h>
#include "sprite.hxx"
#include "animation.hxx"
#include "menu.hxx"
#include <string.h>

Menu::~Menu()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

Menu::Menu(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *entries[], int count, int vspace, int hspace, Sprite *sprite, Animation *animation, int scale)
{
	SDL_Surface *surface[16] = { NULL };  // max 16 entries

	for (int i = 0; i < count; i++)
		surface[i] = TTF_RenderUTF8_Blended(font, entries[i], color);

	int maxLength = 0;
	int maxHeight = 0;
	for (int i = 0; i < count; i++) {
		if (maxLength < surface[i]->w)
			maxLength = surface[i]->w;
		maxHeight += surface[i]->h;
	}
	maxHeight += vspace * (count - 1);

	Uint32 rmask, gmask, bmask, amask;
	// SDL interprets each pixel as a 32-bit number, so our masks must depend on the endianness (byte order) of the machine
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	this->surface = SDL_CreateRGBSurface(0, maxLength, maxHeight, 32, rmask, gmask, bmask, amask);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	for (int i = 0; i < count; i++) {
		rect.w = surface[i]->w;
		rect.h = surface[i]->h;
		SDL_BlitSurface(surface[i], NULL, this->surface, &rect);
		int d = (rect.h - sprite->getHeight() * scale) / 2;
		selectedY[i] = rect.y + d;
		rect.y += surface[i]->h + vspace;
	}
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	this->sprite = sprite;
	this->animation = animation;
	this->count = count;
	this->hspace = hspace;
	this->scale = scale;
	this->selected = 0;

	for (int i = 0; i < count; i++)
		SDL_FreeSurface(surface[i]);
}

bool Menu::handleKey(SDL_KeyboardEvent key)
{
	if (key.keysym.sym == SDLK_UP) {
		selected--;
		if (selected < 0)
			selected = count - 1;
		return true;
	}
	else if (key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_TAB) {
		selected++;
		if (selected >= count)
			selected = 0;
		return true;
	}
	return false;
}

void Menu::draw(SDL_Renderer *renderer, SDL_Rect *rectTarget)
{
	SDL_RenderCopy(renderer, texture, NULL, rectTarget);
	SDL_Rect rect;
	rect.x = rectTarget->x - hspace;
	rect.y = rectTarget->y + selectedY[selected];
	rect.w = sprite->getWidth() * scale;
	rect.h = sprite->getHeight() * scale;
	animation->draw(renderer, sprite, &rect);
}

