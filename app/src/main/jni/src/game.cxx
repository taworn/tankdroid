/**
 * @file game.cxx
 * @desc Game framework module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "game.hxx"
#include "scenes/scene.hxx"
#include "scenes/scene_title.hxx"
#include "scenes/scene_stage.hxx"
#include "scenes/scene_play.hxx"
#include "scenes/scene_lost.hxx"
#include "scenes/scene_win.hxx"
#include "game/common.hxx"
#include <stdio.h>

Game *Game::singleton = NULL;

Game::~Game()
{
	// deletes current scene
	delete scene;

	// deletes arena
	delete arena;

	// deletes draw number
	delete drawNumberSmall;

	// deletes musics
	Mix_FreeMusic(musicOpen);

	// deletes sound effects
	Mix_FreeChunk(sfxItem);
	Mix_FreeChunk(sfxBomb);
	Mix_FreeChunk(sfxShot);

	// closes fonts
	TTF_CloseFont(fontLarge);
	TTF_CloseFont(fontMedium);
	TTF_CloseFont(fontSmall);

	// destroys renderer
	SDL_DestroyRenderer(renderer);

	// destroys window
	SDL_DestroyWindow(window);
}

Game::Game()
	: window(NULL), renderer(NULL), surfaceWindow(NULL)
	, fontSmall(NULL), fontMedium(NULL), fontLarge(NULL)
	, sfxShot(NULL), sfxBomb(NULL), sfxItem(NULL)
	, musicOpen(NULL)
	, drawNumberSmall(NULL)
	, scene(NULL), nextSceneId(SCENE_TITLE)
{
	SDL_assert(singleton == NULL);
	singleton = this;

	// creates window
#ifdef __ANDROID__
	window = SDL_CreateWindow("Tankdroid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);
#else
	window = SDL_CreateWindow("Tankman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);
#endif

	// creates renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// gets the window surface, this code return NULL on Android
	surfaceWindow = SDL_GetWindowSurface(window);

	// opens fonts
	fontSmall = TTF_OpenFont(TANK_RES_("freefont", "FreeSans.ttf"), 24);
	fontMedium = TTF_OpenFont(TANK_RES_("freefont", "FreeSans.ttf"), 32);
	fontLarge = TTF_OpenFont(TANK_RES_("freefont", "FreeSerifBold.ttf"), 64);

	// loads sound effects
	sfxShot = Mix_LoadWAV(TANK_RES("shot.wav"));
	sfxBomb = Mix_LoadWAV(TANK_RES("bomb.wav"));
	sfxItem = Mix_LoadWAV(TANK_RES("item.wav"));

	// loads musics
	musicOpen = Mix_LoadMUS(TANK_RES("open.wav"));
	Mix_PlayMusic(musicOpen, 0);

	// creates draw number
	drawNumberSmall = new DrawNumber(renderer, fontSmall, { 0x80, 0x80, 0x80 });

	// creates arena
	arena = new Arena();
}

void Game::playShot()
{
	Mix_PlayChannel(-1, sfxShot, 0);
}

void Game::playBomb()
{
	Mix_PlayChannel(-1, sfxBomb, 0);
}

void Game::playItem()
{
	Mix_PlayChannel(-1, sfxItem, 0);
}

void Game::changeScene(int sceneId)
{
	SDL_Log("changeScene() called, sceneId = %d", sceneId);
	nextSceneId = sceneId;
}

void Game::switchScene()
{
	SDL_Log("perform switchScene() called, sceneId = %d", nextSceneId);
	if (scene)
		delete scene;

	switch (nextSceneId) {
	default:
	case SCENE_DEFAULT:
		scene = new Scene();
		break;
	case SCENE_TITLE:
		scene = new SceneTitle();
		break;
	case SCENE_STAGE:
		scene = new SceneStage();
		break;
	case SCENE_PLAY:
		scene = new ScenePlay();
		break;
	case SCENE_LOST:
		scene = new SceneLost();
		break;
	case SCENE_WIN:
		scene = new SceneWin();
		break;
	}
	nextSceneId = -1;
}

void Game::run()
{
	SDL_Event e;
	unsigned int fpsLast = SDL_GetTicks();
	unsigned int fpsTime = 0;
	unsigned int timeLast = SDL_GetTicks();
	unsigned int timeUsed = SDL_GetTicks() - timeLast;
#ifdef __ANDROID__
	bool active = true;
#else
	bool active = false;
#endif
	bool exit = false;

	SDL_Log("start");

	while (!exit) {
		if (active) {
			// clears screen
			int w, h;
			SDL_GetRendererOutputSize(renderer, &w, &h);
			SDL_RenderClear(renderer);

			// render current scene
			timeUsed = SDL_GetTicks() - timeLast;
			timeLast = SDL_GetTicks();
			if (nextSceneId < 0)
				scene->render((int)timeUsed);
			else
				switchScene();

			// computes FPS
			fps = SDL_GetTicks() - fpsLast;
			fps = fps > 0 ? 1000 / fps : 1000;
			fpsLast = SDL_GetTicks();
			if (fpsLast - fpsTime > 1000) {
				SDL_Log("%u fps", fps);
				fpsTime = fpsLast;
			}

			// draws FPS
			SDL_Point point = {
				w - (drawNumberSmall->getWidth() * 4),
				h - drawNumberSmall->getHeight()
			};
			drawNumberSmall->draw(renderer, fps, 4, point);

			// outputs to screen
			SDL_RenderPresent(renderer);
		}

		// polls events until queue empty
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				bool handled = false;
				if (scene)
					handled = scene->handleKey(e.key);
				if (!handled) {
					if (e.key.keysym.sym == SDLK_ESCAPE)
						exit = true;
				}
			}
			else if (e.type == SDL_WINDOWEVENT) {
				if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
					SDL_Log("window gained focus");
					active = true;
					Mix_ResumeMusic();
					timeLast = SDL_GetTicks();
				}
				else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
					SDL_Log("window lost focus");
					active = false;
					Mix_PauseMusic();
				}
			}
			else if (e.type == SDL_QUIT) {
				exit = true;
			}
		}
	}

	SDL_Log("exit");
}

