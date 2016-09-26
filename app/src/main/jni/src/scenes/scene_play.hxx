/**
 * @file scene_play.hxx
 * @desc Play scene header.
 */
#ifndef SCENES_SCENE_PLAY_HXX
#define SCENES_SCENE_PLAY_HXX

/**
 * Play scene.
 */
class ScenePlay : public Scene
{
public:
	virtual ~ScenePlay();
	ScenePlay();

	virtual bool handleKey(SDL_KeyboardEvent key);
	virtual void render(unsigned int timeUsed);

private:
};

#endif // SCENES_SCENE_PLAY_HXX

