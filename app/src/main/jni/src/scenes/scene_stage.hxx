/**
 * @file scene_stage.hxx
 * @desc Stage scene header.
 */
#ifndef SCENES_SCENE_STAGE_HXX
#define SCENES_SCENE_STAGE_HXX

/**
 * Stage scene.
 */
class SceneStage : public Scene
{
public:
	virtual ~SceneStage();
	SceneStage();

	virtual bool handleKey(SDL_KeyboardEvent key);
	virtual void render(unsigned int timeUsed);

private:
	SDL_Surface *surface;
	SDL_Texture *texture;
	unsigned int timeTotal;
};

#endif // SCENES_SCENE_STAGE_HXX

