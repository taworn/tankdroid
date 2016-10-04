/**
 * @file item.hxx
 * @desc Item header.
 */
#ifndef GAME_ITEM_HXX
#define GAME_ITEM_HXX

/**
 * An item class.
 */
class Item
{
public:
	/**
	 * Destructor.
	 */
	~Item();

	/**
	 * Constructor.
	 */
	Item();

	/**
	 * Initializes.
	 */
	void init(int x, int y, int imageIndex);

	/**
	 * Done.
	 */
	void done() { happen = false; }

	/**
	 * Draws item.
	 */
	void draw(SDL_Renderer *renderer, Sprite *spriteMisc, SDL_Rect *viewport);

	bool isHappen() const { return happen; }
	int getX() const { return x; }
	int getY() const { return y; }
	int getImageIndex() const { return imageIndex; }

private:
	bool happen;
	int x, y;
	int imageIndex;

	Item(const Item&);
	Item& operator=(const Item&);
};

#endif // GAME_ITEM_HXX

