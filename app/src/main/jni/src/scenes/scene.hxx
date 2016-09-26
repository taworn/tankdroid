/**
 * @file scene.hxx
 * @desc Base game scene header.
 */
#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

/**
 * A base game scene.
 */
class Scene
{
public:
	/**
	 * Destructs a game scene.
	 */
	virtual ~Scene();

	/**
	 * Constructs a game scene.
	 */
	Scene();

	/**
	 * Called when user press keyboard.
	 */
	virtual bool handleKey(SDL_KeyboardEvent key);

	/**
	 * Called every render frame.
	 */
	virtual void render(unsigned int timeUsed);

private:
	Scene(const Scene&);
	Scene& operator=(const Scene&);
};

#endif // SCENES_SCENE_HXX

