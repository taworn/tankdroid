/**
 * @file scene_win.cxx
 * @desc Win scene module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../game.hxx"
#include "scene.hxx"
#include "scene_win.hxx"

SceneWin::~SceneWin()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_Log("SceneWin::~SceneWin()");
}

SceneWin::SceneWin()
{
	SDL_Log("SceneWin::SceneWin()");
	Game *game = Game::instance();
	surface = TTF_RenderUTF8_Blended(game->getFontLarge(), "You Win", { 0xFF, 0xFF, 0x80 });
	texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);
	surfaceBrought = TTF_RenderUTF8_Blended(game->getFontMedium(), "Brought to you by Taworn T. (Oo+)", { 0xFF, 0xFF, 0xFF });
	textureBrought = SDL_CreateTextureFromSurface(game->getRenderer(), surfaceBrought);
}

bool SceneWin::handleKey(SDL_KeyboardEvent key)
{
	SDL_Log("SceneWin::handleKey(%d)", key.keysym.sym);
	return false;
}

void SceneWin::render(int timeUsed)
{
	Game *game = Game::instance();
	SDL_Rect rect;
	int w, h;
	SDL_GetRendererOutputSize(game->getRenderer(), &w, &h);

	rect.x = (w - surface->w) / 2;
	rect.y = (h - surface->h) / 2 - 64;
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_RenderCopy(game->getRenderer(), texture, NULL, &rect);

	rect.x = (w - surfaceBrought->w) / 2;
	rect.y = (h - surfaceBrought->h) / 2 + 64;
	rect.w = surfaceBrought->w;
	rect.h = surfaceBrought->h;
	SDL_RenderCopy(game->getRenderer(), textureBrought, NULL, &rect);
}

