/**
 * @file scene_stage.cxx
 * @desc Stage scene module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "scene.hxx"
#include "scene_stage.hxx"
#include "../game/common.hxx"
#include <stdio.h>

SceneStage::~SceneStage()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_Log("SceneStage::~SceneStage()");
}

SceneStage::SceneStage()
	: timeTotal(0)
{
	SDL_Log("SceneStage::SceneStage()");
	Game *game = Game::instance();
	char buffer[16];
	sprintf(buffer, "Stage %d", game->getArena()->getStage() + 1);
	surface = TTF_RenderUTF8_Blended(game->getFontLarge(), buffer, { 0xFF, 0xFF, 0xFF });
	texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);
}

bool SceneStage::handleKey(SDL_KeyboardEvent key)
{
	SDL_Log("SceneStage::handleKey(%d)", key.keysym.sym);
	if (key.keysym.sym == SDLK_RETURN) {
		Game::instance()->changeScene(Game::SCENE_PLAY);
		return true;
	}
	return false;
}

void SceneStage::render(unsigned int timeUsed)
{
	Game *game = Game::instance();
	SDL_Rect rect;
	int w, h;
	SDL_GetRendererOutputSize(game->getRenderer(), &w, &h);

	rect.x = (w - surface->w) / 2;
	rect.y = (h - surface->h) / 2;
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_RenderCopy(game->getRenderer(), texture, NULL, &rect);

	timeTotal += timeUsed;
	if (timeTotal > 2500)
		game->changeScene(Game::SCENE_PLAY);
}

