/* CroppedView.h
Michael Zahniser, 09/23/07

Header file for the CroppedView class, which represents a window into the game world
that is cropped to be centered on a given location.
*/

#ifndef __MZ_CROPPED_VIEW_H__
#define __MZ_CROPPED_VIEW_H__


#include "Sprite.h"
#include "Image.h"
#include "Interpolator.h"

#include <SDL2/SDL.h>



class CroppedView {
public:
	CroppedView(SDL_Surface *&screenSurface, int left, int top, int width, int height, int tileSize);
	
	// Set the center of the view, in game coordinates (i.e. one tile = 1 unit).
	void SetCenter(float x, float y, int mapWidth, int mapHeight);

	// Get the first and last row and column indices of tiles that are visible.
	int FirstTileX() const { return xOffset / TILE_SIZE; }
	int LastTileX() const { return (xOffset + VIEW_WIDTH - 1) / TILE_SIZE; }
	int FirstTileY() const { return yOffset / TILE_SIZE; }
	int LastTileY() const { return (yOffset + VIEW_HEIGHT - 1) / TILE_SIZE; }
	
	// Draw the tile with the given tile indices.
	void DrawTile(const Sprite &sprite, int x, int y);

	// Draw the given sprite at the given game coordinates.
	void DrawEntity(const Sprite &sprite, float x, float y);
	
	// Fade to the given alpha level.
	void Fade(unsigned char alpha);
	// Draw shadows.
	void DrawShadows();
	

private:
	// Apply the given alpha values to the given portion of the screen.
	void ApplyAlpha(unsigned char *alphaIt, unsigned char *alphaEnd, Uint32 *it);
	void TakeIfGreater(unsigned char &value, unsigned char compare) { if(compare > value) value = compare; }

	void Limit(int &value, int minValue, int maxValue);

	// Draw a Sprite, cropped to the screen and with the current offset applied.
	void DrawCropped(const Sprite &sprite, int left, int top);
	
	
	
private:
	// Offset, in pixels, of the view.
	int xOffset;
	int yOffset;
	
	float playerX;
	float playerY;
	
	SDL_Surface *&screen;
	
	GreyImage shadowGrid;
	GreyImage shadowExtended;
	Interpolator interpolate;
	GreyImage shadows;
	
	
private:
	const int VIEW_LEFT;
	const int VIEW_TOP;
	const int VIEW_WIDTH;
	const int VIEW_HEIGHT;
	const int TILE_SIZE;
	
	// Wierd things may happen if this isn't a divisor of the tile width and height.
	static const int SHADOW_RESOLUTION = 4;
	const int SHADOW_BLOCK_SIZE;
};

#endif
