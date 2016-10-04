/**
 * @file map.hxx
 * @desc Map header.
 */
#ifndef GAME_MAP_HXX
#define GAME_MAP_HXX

/**
 * A map class.
 */
class Map
{
public:
	/**
	 * Destructs the map.
	 */
	~Map();

	/**
	 * Constructs the map.
	 */
	Map();

	/**
	 * Loads map data.
	 */
	bool load(const char *fileName);

	/**
	 * Checks if direction is can be pass.
	 */
	bool canMove(Movable *movable, int direction, SDL_Point *pt);

	/**
	 * Checks if bullet direction is can be pass.
	 */
	bool canShot(int x, int y, int direction, SDL_Point *pt);

	/**
	 * Checks if position, in pixels, is in bounds or not.
	 */
	bool inBounds(int x, int y) { return x >= 0 && x < getPixelWidth() && y >= 0 && y < getPixelHeight(); }

	/**
	 * Adds bullet.
	 */
	bool addBullet(int x, int y, int action);

	/**
	 * Adds item.
	 */
	bool addItem(int x, int y, int imageIndex);

	/**
	 * Checks items when moved.
	 */
	bool checkItems(SDL_Rect *rect);

	/**
	 * Draws map.
	 */
	void draw(SDL_Renderer *renderer, int timeUsed);

	int getUnitWidth() const { return width * 2; }
	int getUnitHeight() const { return height * 2; }
	int getPixelWidth() const { return width * 64; }
	int getPixelHeight() const { return height * 64; }

	Movable* getHero() { return &movHero; }

private:
	Sprite *spriteMap;
	Sprite *spriteTank;
	Sprite *spriteMisc;

	// map parsed
	int width, height;
	char *blockMap;
	int *imageMap;

	// movables and bullets
	int countTank;
	int countBullets;
	int countItems;
	Tank movTanks[64];
	TankHero movHero;
	Bullet *bullets[64];
	Item items[4];

	Arena *arena;

	static const int BLOCK_PASS = 0;
	static const int BLOCK_TREE = 1;
	static const int BLOCK_BRICK = 2;
	static const int BLOCK_STEEL = 3;
	static const int BLOCK_WATER = 4;
	static const int BLOCK_EAGLE = 5;
	static const int BLOCK_HERO = 6;

	void rawToMap(int raw, char *block, int *image);
	bool blockIsPass(int unitX, int unitY);
	bool blockHasEnemy(SDL_Rect *rect);
	bool blockIsShootPass(int unitX, int unitY);
	bool blockShootEnemy(int x, int y);

	Map(const Map&);
	Map& operator=(const Map&);
};

#endif // GAME_MAP_HXX

