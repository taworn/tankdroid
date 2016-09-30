/**
 * @file scene_lost.cxx
 * @desc Lost scene module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "scene.hxx"
#include "scene_lost.hxx"

SceneLost::~SceneLost()
{
	delete menu;
	delete spriteTank;
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_Log("SceneLost::~SceneLost()");
}

SceneLost::SceneLost()
{
	SDL_Log("SceneLost::SceneLost()");
	Game *game = Game::instance();
	surface = TTF_RenderUTF8_Blended(game->getFontLarge(), "Game Over", { 0xFF, 0x80, 0x40 });
	texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);

	spriteTank = new Sprite(game->getRenderer(), TANK_RES("tank.png"), 16, 16);
	aniMenu.add(0, 6, 8, Animation::ON_END_CONTINUE, 250);
	aniMenu.use(0);
	const char *entries[] = {
		"Continue",
		"Restart",
	};
	menu = new Menu(game->getRenderer(), game->getFontMedium(), { 0xFF, 0xFF, 0xFF }, entries, 2, 32, 48, spriteTank, &aniMenu, 2);
}

bool SceneLost::handleKey(SDL_KeyboardEvent key)
{
	SDL_Log("SceneLost::handleKey(%d)", key.keysym.sym);
	bool handled = menu->handleKey(key);
	if (!handled && key.keysym.sym == SDLK_RETURN) {
		SDL_Log("menu selected %d", menu->getSelected());
		if (menu->getSelected() == 0)
			Game::instance()->changeScene(Game::SCENE_STAGE);
		else if (menu->getSelected() == 1)
			Game::instance()->changeScene(Game::SCENE_TITLE);
		return true;
	}
	return false;
}

void SceneLost::render(int timeUsed)
{
	Game *game = Game::instance();
	SDL_Rect rect;
	int w, h;
	SDL_GetRendererOutputSize(game->getRenderer(), &w, &h);

	rect.x = (w - surface->w) / 2;
	rect.y = (h - surface->h) / 2 - surface->h - 64;
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_RenderCopy(game->getRenderer(), texture, NULL, &rect);

	rect.x = (w - menu->getWidth()) / 2;
	rect.y = (h - menu->getHeight()) / 2 + 64;
	rect.w = menu->getWidth();
	rect.h = menu->getHeight();
	menu->draw(game->getRenderer(), &rect);
}

