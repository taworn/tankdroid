/**
 * @file animation.cxx
 * @desc Animation module.
 */
#include <SDL.h>
#include "sprite.hxx"
#include "animation.hxx"

Animation::~Animation()
{
}

Animation::Animation()
	: plays(), currentPlaying(-1), currentImage(0), ending(false)
{
	timeStart = SDL_GetTicks();
}

void Animation::add(int number, int start, int end, int onEnd, int time)
{
	SDL_assert(number >= 0 && number < PLAYING_MAX);
	plays[number].start = start;
	plays[number].end = end;
	plays[number].onEnd = onEnd;
	plays[number].time = time;
}

void Animation::use(int number)
{
	SDL_assert(number >= 0 && number < PLAYING_MAX);
	if (number != currentPlaying) {
		currentPlaying = number;
		currentImage = plays[number].start;
		ending = false;
		timeStart = SDL_GetTicks();
	}
}

void Animation::draw(SDL_Renderer *renderer, Sprite *sprite, SDL_Rect *rectTarget)
{
	if (!ending) {
		sprite->draw(renderer, currentImage, rectTarget);

		int usage = (int)(SDL_GetTicks() - timeStart);
		if (usage > plays[currentPlaying].time) {
			currentImage++;
			if (currentImage >= plays[currentPlaying].end) {
				switch (plays[currentPlaying].onEnd) {
				default:
				case ON_END_CONTINUE:
					currentImage = plays[currentPlaying].start;
					break;
				case ON_END_KEEP_LAST_FRAME:
					currentImage--;
					ending = true;
					break;
				case ON_END_HIDDEN:
					ending = true;
					break;
				}
			}
			timeStart = SDL_GetTicks();
		}
	}
	else {
		if (plays[currentPlaying].onEnd == ON_END_KEEP_LAST_FRAME)
			sprite->draw(renderer, currentImage, rectTarget);
	}
}

