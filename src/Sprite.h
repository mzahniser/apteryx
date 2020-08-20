/* Sprite.h
Michael Zahniser, 09/22/07

Header file for the Sprite class, which represents a sprite that can be drawn by SDL.
*/

#ifndef __MZ_SPRITE_H__
#define __MZ_SPRITE_H__


#include <SDL2/SDL.h>



class Sprite {
public:
	Sprite() : source(NULL) {}
	Sprite(const SDL_Surface *sourceImage, int left, int top, int width, int height);
	
	void Draw(SDL_Surface *dest, int x, int y) const;
	void Draw(SDL_Rect cropRect, SDL_Surface *dest, int x, int y) const;
	
	int Width() const { return sourceRect.w; }
	int Height() const { return sourceRect.h; }
	
	
private:
	const SDL_Surface *source;
	SDL_Rect sourceRect;
	
	mutable SDL_Rect destRect;
};

#endif
