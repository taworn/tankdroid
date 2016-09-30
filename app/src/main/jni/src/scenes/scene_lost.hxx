/**
 * @file scene_lost.hxx
 * @desc Lost scene header.
 */
#ifndef SCENES_SCENE_LOST_HXX
#define SCENES_SCENE_LOST_HXX

/**
 * Lost scene.
 */
class SceneLost : public Scene
{
public:
	virtual ~SceneLost();
	SceneLost();

	virtual bool handleKey(SDL_KeyboardEvent key);
	virtual void render(int timeUsed);

private:
	SDL_Surface *surface;
	SDL_Texture *texture;
	Sprite *spriteTank;
	Animation aniMenu;
	Menu *menu;
};

#endif // SCENES_SCENE_LOST_HXX

