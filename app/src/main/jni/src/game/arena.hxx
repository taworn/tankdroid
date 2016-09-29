/**
 * @file arena.hxx
 * @desc Battle area header.
 */
#ifndef GAME_ARENA_HXX
#define GAME_ARENA_HXX

/**
 * A battle area.
 */
class Arena
{
public:
	/**
	 * Destructs an arena.
	 */
	virtual ~Arena();

	/**
	 * Constructs an arena.
	 */
	Arena();

	/**
	 * Gets score.
	 */
	int getScore() const { return score; }

	/**
	 * Gets stage.
	 */
	int getStage() const { return stage; }

	/**
	 * Gets map.
	 */
	Map* getMap() { return map; }

	/**
	 * Starts batlle.
	 */
	void startBattle();

	/**
	 * Draws a battle scene.
	 */
	void draw(unsigned int timeUsed);

private:
	int score;
	int stage;
	Sprite *spriteTank;
	Map *map;
	Movable *hero;

	Arena(const Arena&);
	Arena& operator=(const Arena&);
};

#endif // GAME_ARENA_HXX

