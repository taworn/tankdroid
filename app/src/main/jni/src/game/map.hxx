/**
 * @file map.hxx
 * @desc Map header.
 */
#ifndef GAME_MAP_HXX
#define GAME_MAP_HXX

class Movable;

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
	 * Draws map.
	 */
	void draw(SDL_Renderer *renderer, unsigned int timeUsed);

	int getUnitWidth() const { return width * 2; }
	int getUnitHeight() const { return height * 2; }

	Movable* getHero() { return &movHero; }

private:
	Sprite *spriteMap;
	Sprite *spriteTank;
	Sprite *spriteMisc;

	// map loaded
	int width, height;
	int32_t *blockRaw;

	// map parsed
	char *blockMap;
	int *imageMap;

	// movables and bullets
	Movable movHero;
	Movable movTanks[64];
	//Bullet bullets[128];
	//Item item[16];

	static const int BLOCK_PASS = 0;
	static const int BLOCK_TREE = 1;
	static const int BLOCK_BRICK = 2;
	static const int BLOCK_STEEL = 3;
	static const int BLOCK_WATER = 4;
	static const int BLOCK_EAGLE = 5;
	static const int BLOCK_HERO = 6;

	void rawToMap(int raw, char *block, int *image);

	Map(const Map&);
	Map& operator=(const Map&);
};

#endif // GAME_MAP_HXX

