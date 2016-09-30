/**
 * @file scene_win.hxx
 * @desc Win scene header.
 */
#ifndef SCENES_SCENE_WIN_HXX
#define SCENES_SCENE_WIN_HXX

/**
 * Win scene.
 */
class SceneWin : public Scene
{
public:
	virtual ~SceneWin();
	SceneWin();

	virtual bool handleKey(SDL_KeyboardEvent key);
	virtual void render(int timeUsed);

private:
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Surface *surfaceBrought;
	SDL_Texture *textureBrought;
};

#endif // SCENES_SCENE_WIN_HXX

