/**
 * @file scene_title.hxx
 * @desc Title scene header.
 */
#ifndef SCENES_SCENE_TITLE_HXX
#define SCENES_SCENE_TITLE_HXX

/**
 * Title scene.
 */
class SceneTitle : public Scene
{
public:
	virtual ~SceneTitle();
	SceneTitle();

	virtual bool handleKey(SDL_KeyboardEvent key);
	virtual void render(int timeUsed);

private:
	SDL_Surface *surfaceTitle;
	SDL_Texture *textureTitle;
	Sprite *spriteTank;
	Animation aniTank;
	Animation aniMenu;
	Menu *menu;
	int imageX;
};

#endif // SCENES_SCENE_TITLE_HXX

