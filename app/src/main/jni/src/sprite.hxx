/**
 * @file sprite.hxx
 * @desc Sprite header.
 */
#ifndef SPRITE_HXX
#define SPRITE_HXX

/**
 * A sprite class.
 */
class Sprite
{
public:
	/**
	 * Destructs a sprite.
	 */
	~Sprite();

	/**
	 * Constructs a sprite.
	 */
	Sprite(SDL_Renderer *renderer, const char *imageFileName, int sliceHorz, int sliceVert);

	/**
	 * Draws sprite.
	 */
	void draw(SDL_Renderer *renderer, int imageIndex, SDL_Rect *rectTarget);

	/**
	 * Gets number of images.
	 */
	int getImageCount() const { return sliceHorz * sliceVert; }

	/**
	 * Gets width of one image.
	 */
	int getWidth() const { return width; }

	/**
	 * Gets height of one image.
	 */
	int getHeight() const { return height; }

private:
	SDL_Surface *surface;
	SDL_Texture *texture;
	int sliceHorz;
	int sliceVert;
	int width;
	int height;

	Sprite(const Sprite&);
	Sprite& operator=(const Sprite&);
};

#endif // SPRITE_HXX

