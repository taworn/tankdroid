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
	 * SAves data.
	 */
	void save();

	/**
	 * Restarts game.
	 */
	void restart();

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
	 * Nexts stage.  Returns true if ok, otherwise, it is false and you win game.
	 */
	bool nextStage()
	{
		if (stage + 1 < STAGE_COUNT) {
			stage++;
			return true;
		}
		else
			return false;
	}

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
	 * Ends battle and report result.  Returns 1 is win, returns -1 is loss.
	 */
	int endBattle();

	/**
	 * Draws a battle scene.
	 */
	void draw(int timeUsed);

	Map* getMap() const { return map; }

private:
	SDL_Surface *surfaceScore;
	SDL_Texture *textureScore;
	SDL_Surface *surfaceHP;
	SDL_Texture *textureHP;
	DrawNumber *drawNumber;

	int score;
	int stage;
	bool firepower;
	int firepowerTime;
	Map *map;

	bool load();

	Arena(const Arena&);
	Arena& operator=(const Arena&);
};

#endif // GAME_ARENA_HXX

