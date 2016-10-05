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
	static const int ITEM_GUN = 0;
	static const int ITEM_HELMET = 1;
	static const int ITEM_STAR = 2;

	/**
	 * Destructs the arena.
	 */
	~Arena();

	/**
	 * Constructs the arena.
	 */
	Arena();

	/**
	 * Gets score.
	 */
	int getScore() const { return score; }

	/**
	 * Adds score.
	 */
	void addScore(int point) { score += point; }

	/**
	 * Gets stage.
	 */
	int getStage() const { return stage; }

	/**
	 * Pickup item.
	 */
	void pickItem(int item);

	/**
	 * Boosting fire power.
	 */
	bool boostFirepower() const { return firepower; }

	/**
	 * Starts battle.
	 */
	void startBattle();

	/**
	 * Ends battle.
	 */
	void endBattle();

	/**
	 * Draws a battle scene.
	 */
	void draw(int timeUsed);

	Map* getMap() const { return map; }

private:
	Mix_Music *musicTrack;
	int score;
	int stage;
	bool firepower;
	int firepowerTime;
	Map *map;

	Arena(const Arena&);
	Arena& operator=(const Arena&);
};

#endif // GAME_ARENA_HXX

