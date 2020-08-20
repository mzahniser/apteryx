/* Sidebar.h
Michael Zahniser, 10/01/07

Header file for the Sidebar class, which represents the player status display
on the side of the screen.
*/

#ifndef __MZ_SIDEBAR_H__
#define __MZ_SIDEBAR_H__


#include "Sprite.h"

#include <SDL2/SDL.h>
#include <vector>
#include <string>

using std::vector;
using std::string;



class Sidebar {
public:
	void Draw(SDL_Surface *screen);
	
	
private:
	// Draw the map view.
	void DrawMap(SDL_Surface *screen, int frame);
	void DrawMapPixel(Uint32 *pixels, int pitch, int x, int y, Uint32 color);
};

#endif
