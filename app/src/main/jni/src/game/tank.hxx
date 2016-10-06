/**
 * @file tank.hxx
 * @desc Tank header.
 */
#ifndef GAME_TANK_HXX
#define GAME_TANK_HXX

/**
 * A tank class.
 */
class Tank : public Movable
{
public:
	/**
	 * Constructor.
	 */
	Tank();

	/**
	 * Called by map to initialization.
	 */
	void init(int x, int y, int tank, int color);

	/**
	 * Makes decision.
	 */
	void ai();

	virtual void dead();

private:
	Tank(const Tank&);
	Tank& operator=(const Tank&);
};

#endif // GAME_TANK_HXX

