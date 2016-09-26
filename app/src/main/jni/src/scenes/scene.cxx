/**
 * @file scene.cxx
 * @desc Base game scene module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "scene.hxx"

Scene::~Scene()
{
	SDL_Log("Scene::~Scene()");
}

Scene::Scene()
{
	SDL_Log("Scene::Scene()");
}

bool Scene::handleKey(SDL_KeyboardEvent key)
{
	SDL_Log("Scene::handleKey(%d)", key.keysym.sym);
	return false;
}

void Scene::render(unsigned int timeUsed)
{
}

