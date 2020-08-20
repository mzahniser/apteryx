/* Sprite.cpp
Michael Zahniser, 09/22/07

Function definitions for the Sprite class.
*/

#include "Sprite.h"



Sprite::Sprite(const SDL_Surface *sourceImage, int left, int top, int width, int height)
	: source(sourceImage)
{
	sourceRect.x = left;
	sourceRect.y = top;
	sourceRect.w = width;
	sourceRect.h = height;
}



void Sprite::Draw(SDL_Surface *dest, int x, int y) const
{
	destRect.x = x;
	destRect.y = y;
	
	SDL_BlitSurface(const_cast<SDL_Surface *>(source), const_cast<SDL_Rect *>(&sourceRect), dest, &destRect);
}



void Sprite::Draw(SDL_Rect cropRect, SDL_Surface *dest, int x, int y) const
{
	cropRect.x += sourceRect.x;
	cropRect.y += sourceRect.y;
	destRect.x = x;
	destRect.y = y;
	
	SDL_BlitSurface(const_cast<SDL_Surface *>(source), &cropRect, dest, &destRect);
}

