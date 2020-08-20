/* CroppedView.cpp
Michael Zahniser, 09/23/07

Function definitions for the CroppedView class.
*/

#include "CroppedView.h"

#include "GameData.h"

#include <cstring>

using std::memset;



CroppedView::CroppedView(SDL_Surface *&screenSurface, int left, int top, int width, int height, int tileSize)
	: screen(screenSurface), 
	VIEW_LEFT(left), VIEW_TOP(top), VIEW_WIDTH(width), VIEW_HEIGHT(height),
	TILE_SIZE(tileSize), SHADOW_BLOCK_SIZE(tileSize / SHADOW_RESOLUTION)
{
	shadowGrid.Size((width + SHADOW_BLOCK_SIZE - 1) / SHADOW_BLOCK_SIZE + 3,
		(height + SHADOW_BLOCK_SIZE - 1) / SHADOW_BLOCK_SIZE + 3);
	shadowExtended.Size(shadowGrid);
	shadows.Size((shadowGrid.Width() - 1) * SHADOW_BLOCK_SIZE, 
		(shadowGrid.Height() - 1) * SHADOW_BLOCK_SIZE);
}



// Set the center of the view, in game coordinates (i.e. one tile = 1 unit).
void CroppedView::SetCenter(float x, float y, int mapWidth, int mapHeight)
{
	playerX = x;
	playerY = y;
	
	xOffset = static_cast<int>(.5f + x * static_cast<float>(TILE_SIZE) - static_cast<float>(VIEW_WIDTH) * .5f);
	yOffset = static_cast<int>(.5f + y * static_cast<float>(TILE_SIZE) - static_cast<float>(VIEW_HEIGHT) * .5f);
	
	Limit(xOffset, 0, TILE_SIZE * mapWidth - VIEW_WIDTH);
	Limit(yOffset, 0, TILE_SIZE * mapHeight - VIEW_HEIGHT);

}



// Draw the tile with the given tile indices.
void CroppedView::DrawTile(const Sprite &sprite, int x, int y)
{
	DrawCropped(sprite, x * TILE_SIZE, y * TILE_SIZE);
}



// Draw the given sprite at the given game coordinates.
void CroppedView::DrawEntity(const Sprite &sprite, float x, float y)
{
	int left = static_cast<int>(.5f + x * static_cast<float>(TILE_SIZE) - static_cast<float>(sprite.Width()) * .5f);
	int top = static_cast<int>(.5f + y * static_cast<float>(TILE_SIZE) - static_cast<float>(sprite.Height()) * .5f);

	DrawCropped(sprite, left, top);
}


	
// Fade to the given alpha level.
void CroppedView::Fade(unsigned char alpha)
{
	if(alpha == 255)
		return;
		
	Uint32 multiplier = alpha + 1;
	
	if(!SDL_MUSTLOCK(screen) || SDL_LockSurface(screen) >= 0)
	{		
		Uint32 *pixels = reinterpret_cast<Uint32 *>(screen->pixels);
		int pitch = (screen->pitch / 4);
		
		for(int row = 0; row < VIEW_HEIGHT; ++row)
		{
			Uint64 *it = reinterpret_cast<Uint64 *>(pixels + pitch * row);
			Uint64 *end = it + (VIEW_WIDTH / 2);
			
			for(; it != end; ++it)
			{
				Uint64 odd = *it & 0x00FF00FF00FF00FFuLL;
				odd *= multiplier;
				odd &= 0xFF00FF00FF00FF00uLL;
				
				Uint64 even = *it & 0x0000FF000000FF00uLL;
				even *= multiplier;
				even &= 0x00FF000000FF0000uLL;
				
				*it = (odd | even) >> 8;
			}
		}

		if(SDL_MUSTLOCK(screen))
			SDL_UnlockSurface(screen);
	}
}



void CroppedView::DrawShadows()
{
	if(SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
		return;

	Level &level = *GameData::GetInstance().CurrentLevel();
	
	int gridStartX = (xOffset - SHADOW_BLOCK_SIZE / 2) / SHADOW_BLOCK_SIZE - 1;
	int gridStartY = (yOffset - SHADOW_BLOCK_SIZE / 2) / SHADOW_BLOCK_SIZE - 1;
	int pixelStartX = gridStartX * SHADOW_BLOCK_SIZE + SHADOW_BLOCK_SIZE / 2;
	int pixelStartY = gridStartY * SHADOW_BLOCK_SIZE + SHADOW_BLOCK_SIZE / 2;
	int gridOffX = xOffset - pixelStartX;
	int gridOffY = yOffset - pixelStartY;
	
	float stepSize = static_cast<float>(SHADOW_BLOCK_SIZE) / static_cast<float>(TILE_SIZE);
	
	float startX = static_cast<float>(pixelStartX) / static_cast<float>(TILE_SIZE);
	float startY = static_cast<float>(pixelStartY) / static_cast<float>(TILE_SIZE);
	
	float y = startY;
	for(int row = 0; row < shadowGrid.Rows(); ++row)
	{
		float x = startX;
		for(unsigned char *it = shadowGrid.Begin(row); it != shadowGrid.End(row); ++it)
		{
			*it = static_cast<unsigned char>(255.99f * level.LineOfSight(playerX, playerY, x, y));
			x += stepSize;
		}
		y += stepSize;
	}
	
	shadowExtended = shadowGrid;
	
	y = startY;
	for(int row = 0; row < shadowGrid.Rows(); ++row)
	{
		int gridY = static_cast<int>(y);
		
		bool yOutOfBounds = (gridY < 0) | (gridY >= level.Height());
		
		float x = startX;
		for(int column = 0; column < shadowGrid.Columns(); ++column)
		{
			int gridX = static_cast<int>(x);
			
			bool xOutOfBounds = (gridX < 0) | (gridX >= level.Width());
			
			if((xOutOfBounds | yOutOfBounds) || level(gridX, gridY).IsOpaque())
			{
				unsigned char &pixel = shadowExtended(column, row);

				int cornerX = ((gridStartX + column) / SHADOW_RESOLUTION) * SHADOW_RESOLUTION - gridStartX;
				int cornerY = ((gridStartY + row) / SHADOW_RESOLUTION) * SHADOW_RESOLUTION - gridStartY;
				
				int left = cornerX - 1;
				int top = cornerY - 1;
				int right = cornerX + SHADOW_RESOLUTION;
				int bottom = cornerY + SHADOW_RESOLUTION;
				
				bool leftIn = (left >= 0);
				bool topIn = (top >= 0);
				bool rightIn = (right < shadowGrid.Width());
				bool bottomIn = (bottom < shadowGrid.Height());
				
				bool blockedLeft = leftIn & 
					(((gridX - 1 < 0) | yOutOfBounds) || level(gridX - 1, gridY).IsOpaque());
				bool blockedTop = topIn & 
					(((gridY - 1 < 0) | xOutOfBounds) || level(gridX, gridY - 1).IsOpaque());
				bool blockedRight = rightIn & 
					(((gridX + 1 >= level.Width()) | yOutOfBounds) || level(gridX + 1, gridY).IsOpaque());
				bool blockedBottom = bottomIn & 
					(((gridY + 1 >= level.Height()) | xOutOfBounds) || level(gridX, gridY + 1).IsOpaque());
				
				if(blockedTop & blockedLeft)
					TakeIfGreater(pixel, shadowGrid(left, top));
				if(leftIn)
					TakeIfGreater(pixel, shadowGrid(left, row));
				if(blockedLeft & blockedBottom)
					TakeIfGreater(pixel, shadowGrid(left, bottom));
				if(bottomIn)
					TakeIfGreater(pixel, shadowGrid(column, bottom));
				if(blockedBottom & blockedRight)
					TakeIfGreater(pixel, shadowGrid(right, bottom));
				if(rightIn)
					TakeIfGreater(pixel, shadowGrid(right, row));
				if(blockedRight & blockedTop)
					TakeIfGreater(pixel, shadowGrid(right, top));
				if(topIn)
					TakeIfGreater(pixel, shadowGrid(column, top));
			}
			
			x += stepSize;
		}
		y += stepSize;
	}
			
	interpolate(shadowExtended, shadows);

	Uint32 *pixels = reinterpret_cast<Uint32 *>(screen->pixels);
	int pitch = (screen->pitch / 4);

	for(int row = 0; row < VIEW_HEIGHT; ++row)
	{
		Uint32 *it = pixels + row * pitch;
		unsigned char *alphaBegin = shadows.Begin(row + gridOffY) + gridOffX;
		Uint64 *alpha8 = reinterpret_cast<Uint64 *>(alphaBegin);
		Uint64 *end8 = alpha8 + VIEW_WIDTH / 8;
		unsigned char *alphaEnd = alphaBegin + VIEW_WIDTH;
		
		for( ; alpha8 != end8; ++alpha8, it += 8)
		{
			if(!*alpha8)
				memset(it, 0, 32);
			else if(~*alpha8)
			{
				unsigned char *alpha = reinterpret_cast<unsigned char *>(alpha8);
				ApplyAlpha(alpha, alpha + 8, it);
			}
		}
		ApplyAlpha(reinterpret_cast<unsigned char *>(end8), alphaEnd, it);
	}
	
	if(SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);
}



// Apply the given alpha values to the given portion of the screen.
void CroppedView::ApplyAlpha(unsigned char *alphaIt, unsigned char *alphaEnd, Uint32 *it)
{
	for( ; alphaIt != alphaEnd; ++it, ++alphaIt)
	{
		Uint64 temp = static_cast<Uint64>(*it);
		temp |= (temp << 32);
		temp &= 0x0000FF0000FF00FFull;
		temp *= (*alphaIt + 1);
		temp >>= 8;
		temp &= 0x0000FF0000FF00FFull;
		temp |= (temp >> 32);
		*it = static_cast<Uint32>(temp);
	}
}



void CroppedView::Limit(int &value, int minValue, int maxValue)
{
	if(value < minValue)
		value = minValue;
	else if(value > maxValue)
		value = maxValue;
}



// Draw a Sprite, cropped to the screen and with the current offset applied.
void CroppedView::DrawCropped(const Sprite &sprite, int left, int top)
{
	// Apply the current view offset.
	left -= xOffset;
	top -= yOffset;
	// Calculate the full bounding box of the sprite.
	int right = left + sprite.Width();
	int bottom = top + sprite.Height();
	
	// If no part of the sprite appears in the view, we don't need to draw anything.
	if((right <= 0) | (bottom <= 0) | (left >= VIEW_WIDTH) | (top >= VIEW_HEIGHT))
		return;
	
	// Default source rect - the entire sprite.
	SDL_Rect source;
	source.x = 0;
	source.y = 0;
	source.w = sprite.Width();
	source.h = sprite.Height();
	
	// If the sprite is overlapping the left edge of the screen, only the right hand side
	// of the sprite must be drawn.
	// If it is overlapping the right, the opposite side should be drawn.
	if(left < 0)
	{
		source.x += -left;
		source.w -= -left;
		left = 0;
	}
	else if(right > VIEW_WIDTH)
		source.w -= (right - VIEW_WIDTH);
	
	// Calculate what vertical portion of the sprite is in the view.
	if(top < 0)
	{
		source.y += -top;
		source.h -= -top;
		top = 0;
	}
	else if(bottom > VIEW_WIDTH)
		source.h -= (bottom - VIEW_WIDTH);
	
	// Draw the sprite to the view region of the screen.
	sprite.Draw(source, screen, left + VIEW_LEFT, top + VIEW_TOP);
}
