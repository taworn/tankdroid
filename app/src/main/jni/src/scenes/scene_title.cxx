/**
 * @file scene_title.cxx
 * @desc Title scene module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "scene.hxx"
#include "scene_title.hxx"

SceneTitle::~SceneTitle()
{
	delete menu;
	delete spriteTank;
	SDL_DestroyTexture(textureTitle);
	SDL_FreeSurface(surfaceTitle);
	SDL_Log("SceneTitle::~SceneTitle()");
}

SceneTitle::SceneTitle()
{
	SDL_Log("SceneTitle::SceneTitle()");
	Game *game = Game::instance();
#ifdef __ANDROID__
	surfaceTitle = TTF_RenderUTF8_Blended(game->getFontLarge(), "Tankdroid", { 0x80, 0xCC, 0x80 });
#else
	surfaceTitle = TTF_RenderUTF8_Blended(game->getFontLarge(), "Tankman", { 0x80, 0xCC, 0x80 });
#endif
	textureTitle = SDL_CreateTextureFromSurface(game->getRenderer(), surfaceTitle);

	spriteTank = new Sprite(game->getRenderer(), TANK_RES("tank.png"), 16, 16);

	aniTank.add(0, 134, 136, Animation::ON_END_CONTINUE, 250);
	aniTank.use(0);

	aniMenu.add(0, 6, 8, Animation::ON_END_CONTINUE, 250);
	aniMenu.use(0);
	const char *entries[] = {
		"Continue",
		"Start",
	};
	menu = new Menu(game->getRenderer(), game->getFontMedium(), { 0xFF, 0xFF, 0xFF }, entries, 2, 32, 48, spriteTank, &aniMenu, 2);

	imageX = 0;
}

bool SceneTitle::handleKey(SDL_KeyboardEvent key)
{
	SDL_Log("SceneTitle::handleKey(%d)", key.keysym.sym);
	bool handled = menu->handleKey(key);
	if (!handled && key.keysym.sym == SDLK_RETURN) {
		SDL_Log("menu selected %d", menu->getSelected());
		Game::instance()->changeScene(Game::SCENE_STAGE);
		return true;
	}
	return false;
}

void SceneTitle::render(unsigned int timeUsed)
{
	Game *game = Game::instance();
	SDL_Rect rect;
	int w, h;
	SDL_GetRendererOutputSize(game->getRenderer(), &w, &h);

	// draws title
	rect.x = (w - surfaceTitle->w) / 2;
	rect.y = (h - surfaceTitle->h) / 2 - surfaceTitle->h - 64;
	rect.w = surfaceTitle->w;
	rect.h = surfaceTitle->h;
	SDL_RenderCopy(game->getRenderer(), textureTitle, NULL, &rect);

	// draws image
	rect.x = imageX;
	rect.y = h / 2;
	rect.w = spriteTank->getWidth() * 4;
	rect.h = spriteTank->getHeight() * 4;
	aniTank.draw(game->getRenderer(), spriteTank, &rect);
	imageX += 4;
	if (imageX > w)
		imageX = 0;

	// draws menu
	rect.x = (w - menu->getWidth()) / 2;
	rect.y = (h - menu->getHeight()) / 2 + 160;
	rect.w = menu->getWidth();
	rect.h = menu->getHeight();
	menu->draw(game->getRenderer(), &rect);
}

