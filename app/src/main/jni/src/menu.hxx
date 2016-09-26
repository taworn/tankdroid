/**
 * @file menu.hxx
 * @desc Simple menu header.
 */
#ifndef MENU_HXX
#define MENU_HXX

/**
 * A simple menu class.
 */
class Menu
{
public:
	/**
	 * Destructs menu.
	 */
	~Menu();

	/**
	 * Constructs menu.
	 */
	Menu(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *entries[], int count, int vspace, int hspace, Sprite *sprite, Animation *animation, int scale);

	/**
	 * Called when user press keyboard.
	 */
	bool handleKey(SDL_KeyboardEvent key);

	/**
	 * Draws menu.
	 */
	void draw(SDL_Renderer *renderer, SDL_Rect *rectTarget);

	int getWidth() const { return surface->w; }
	int getHeight() const { return surface->h; }
	int getSelected() const { return selected; }

private:
	SDL_Surface *surface;
	SDL_Texture *texture;
	Sprite *sprite;
	Animation *animation;
	int count;
	int hspace;
	int scale;
	int selected;
	int selectedY[16];

	Menu(const Menu&);
	Menu& operator=(const Menu&);
};

#endif // MENU_HXX

