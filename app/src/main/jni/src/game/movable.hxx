/**
 * @file movable.hxx
 * @desc Movable header.
 */
#ifndef GAME_MOVABLE_HXX
#define GAME_MOVABLE_HXX

class Map;

/**
 * A movable class.
 */
class Movable
{
public:
	static const int ACTION_DEAD = 0;
	static const int ACTION_IDLE = 1;
	static const int ACTION_MOVE_LEFT = 2;
	static const int ACTION_MOVE_RIGHT = 3;
	static const int ACTION_MOVE_UP = 4;
	static const int ACTION_MOVE_DOWN = 5;

	static const int MOVE_LEFT = 1;
	static const int MOVE_RIGHT = 2;
	static const int MOVE_UP = 4;
	static const int MOVE_DOWN = 8;

	/**
	 * Destructs the movable.
	 */
	~Movable();

	/**
	 * Constructs the movable.
	 */
	Movable();

	/**
	 * Moves by direction, use constant MOVE_*.
	 */
	virtual void move(int dir);

	/**
	 * Plays animation after call move().
	 */
	virtual void play(int timeUsed);

	/**
	 * Checks whether movable is alive or dead.
	 */
	bool isAlive() const { return action >= ACTION_IDLE; }

	/**
	 * Checks whether movable is idling or busing.
	 */
	bool isIdle() const { return action == ACTION_IDLE; }

	/**
	 * Draws movable.
	 */
	void draw(SDL_Renderer *renderer, Sprite *spriteTank, Sprite *spriteMisc, SDL_Rect *viewport);

	int getX() const { return rect.x; }
	int getY() const { return rect.y; }
	int getUnitX() const { return rect.x / 32; }
	int getUnitY() const { return rect.y / 32; }

	bool isMovingAction() const { return action >= ACTION_MOVE_LEFT && action <= ACTION_MOVE_DOWN; }

private:
	int action;         // see ACTION_* constants
	int direction;      // move in direction, see MOVE_* constants
	int nextDirection;  // next move in queue
	int timePerDead;    // time for one shot when dead
	int timePerMove;    // time for one shot when move
	int timeUsed;       // time in animation
	bool lock;          // lock when perform animation

	SDL_Rect rect;       // rectangle (x, y, w, h) for this unit, relative with (0, 0) of map
	SDL_Point target;    // move to target
	SDL_Point distance;  // distance between target and point
	Animation ani;

	Movable(const Movable&);
	Movable& operator=(const Movable&);
};

#endif // GAME_MOVABLE_HXX

