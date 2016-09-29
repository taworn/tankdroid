/**
 * @file animation.hxx
 * @desc Animation header.
 */
#ifndef ANIMATION_HXX
#define ANIMATION_HXX

class Sprite;

/**
 * An animation class.
 */
class Animation
{
public:
	static const int ON_END_CONTINUE = 0;
	static const int ON_END_KEEP_LAST_FRAME = 1;
	static const int ON_END_HIDDEN = 2;

	/**
	 * Destructs an animation.
	 */
	~Animation();

	/**
	 * Constructs an animation.
	 */
	Animation();

	/**
	 * Adds a playing animation, only 16 set allow.
	 */
	void add(int number, int start, int end, int onEnd, int time);

	/**
	 * Uses a playing animation.
	 */
	void use(int number);

	/**
	 * Draws animation.
	 */
	void draw(SDL_Renderer *renderer, Sprite *sprite, SDL_Rect *rectTarget);

	bool isEnded() const { return ending; }

private:
	static const int PLAYING_MAX = 16;
	struct PLAYING {
		int start;
		int end;
		int onEnd;
		unsigned int time;
	};
	PLAYING plays[PLAYING_MAX];
	int currentPlaying;
	int currentImage;
	bool ending;

	unsigned int timeStart;

	Animation(const Animation&);
	Animation& operator=(const Animation&);
};

#endif // ANIMATION_HXX

