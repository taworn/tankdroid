/**
 * @file drawnum.hxx
 * @desc Simple integer draw header.
 */
#ifndef DRAWNUM_HXX
#define DRAWNUM_HXX

/**
 * Simple integer draw class.
 */
class DrawNumber
{
public:
	/**
	 * Destructs integer drawer.
	 */
	~DrawNumber();

	/**
	 * Constructs integer drawer.
	 */
	DrawNumber(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);

	/**
	 * Draws prepared integer.
	 */
	void draw(SDL_Renderer *renderer, int value, int limit, SDL_Point point);

	int getWidth() const { return maxDigitWidth; }
	int getHeight() const { return maxDigitHeight; }

private:
	SDL_Surface *surface[10];
	SDL_Texture *texture[10];
	int maxDigitWidth;
	int maxDigitHeight;
	int limitDigits;

	DrawNumber(const DrawNumber&);
	DrawNumber& operator=(const DrawNumber&);
};

#endif // DRAWNUM_HXX

