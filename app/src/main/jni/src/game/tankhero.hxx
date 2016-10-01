/**
 * @file tankhero.hxx
 * @desc Tank hero header.
 */
#ifndef GAME_TANKHERO_HXX
#define GAME_TANKHERO_HXX

/**
 * A tank hero class.
 */
class TankHero : public Movable
{
public:
	/**
	 * Constructor.
	 */
	TankHero();

	/**
	 * Called by map to initialization.
	 */
	void init(int x, int y);

private:
	TankHero(const TankHero&);
	TankHero& operator=(const TankHero&);
};

#endif // GAME_TANKHERO_HXX

