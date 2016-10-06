/**
 * @file bullet.hxx
 * @desc Bullet header.
 */
#ifndef GAME_BULLET_HXX
#define GAME_BULLET_HXX

/**
 * A bullet class.
 */
class Bullet
{
public:
	/**
	 * Destructor.
	 */
	~Bullet();

	/**
	 * Constructor.
	 */
	Bullet(int x, int y, int dir, bool hero);

	/**
	 * Checks it's should be delete.
	 */
	bool shouldBeDelete();

	/**
	 * Checks if obstacle is exists or not.
	 */
	bool check();

	/**
	 * Plays animation.
	 */
	void play(int timeUsed);

	/**
	 * Draws bullet.
	 */
	void draw(SDL_Renderer *renderer, Sprite *spriteMisc, SDL_Rect *viewport, int timeUsed);

	int getAction() const { return direction; }
	int getX() const { return point.x; }
	int getY() const { return point.y; }
	bool isFromHero() const { return fromHero; }

private:
	int direction;
	int imageIndex;
	int timePerMove;
	int timeUsed;
	bool deleted;
	bool fromHero;
	SDL_Point point;
	SDL_Point target;
	SDL_Point distance;
	Arena *arena;

	Bullet(const Bullet&);
	Bullet& operator=(const Bullet&);
};

#endif // GAME_BULLET_HXX

