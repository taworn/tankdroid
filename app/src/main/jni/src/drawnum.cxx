/**
 * @file drawnum.cxx
 * @desc Simple integer draw module.
 */
#include <SDL.h>
#include <SDL_ttf.h>
#include "drawnum.hxx"

DrawNumber::~DrawNumber()
{
	for (int i = 0; i < 10; i++) {
		SDL_DestroyTexture(texture[i]);
		SDL_FreeSurface(surface[i]);
	}
}

DrawNumber::DrawNumber(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
	: maxDigitWidth(0), maxDigitHeight(0)
{
	char sz[2] = { 0 };
	for (int i = 0; i < 10; i++) {
		sz[0] = i + 48;
		surface[i] = TTF_RenderUTF8_Blended(font, sz, color);
		texture[i] = SDL_CreateTextureFromSurface(renderer, surface[i]);
		if (maxDigitWidth < surface[i]->w)
			maxDigitWidth = surface[i]->w;
		if (maxDigitHeight < surface[i]->h)
			maxDigitHeight = surface[i]->h;
	}
}

void DrawNumber::draw(SDL_Renderer *renderer, int value, int limit, SDL_Point point)
{
	if (value < 0)
		value = -value;
	int number[32];
	int count = 0;
	do {
		int v = value % 10;
		value /= 10;
		number[count++] = v;
	} while (value != 0);

	SDL_Rect rect;
	int i = 0;
	rect.x = point.x + (limit - 1) * maxDigitWidth;
	rect.y = point.y;
	while (1) {
		int v = number[i];
		rect.w = surface[v]->w;
		rect.h = surface[v]->h;
		SDL_RenderCopy(renderer, texture[v], NULL, &rect);
		limit--;
		i++;
		if (i >= count || limit <= 0)
			break;
		rect.x -= maxDigitWidth;
	}
}

