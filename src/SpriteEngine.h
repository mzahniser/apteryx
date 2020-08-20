/* SpriteEngine.h
Michael Zahniser, 10/12/07

Header file for the SpriteEngine class, which stores a list of sprites indexed by file name
and frame number.
*/

#ifndef __MZ_SPRITE_ENGINE_H__
#define __MZ_SPRITE_ENGINE_H__


#include "Sprite.h"
#include "DataFile.h"

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;



class SpriteEngine {
public:
	SpriteEngine();
	~SpriteEngine();
	
	// Read the source of a sprite from the given data file.
	void Add(DataFile &in, const string &resourcePath);
	
	const Sprite &GetSprite(const string &name, int frame) const;
	const Sprite &GetSprite(int spriteIndex, int frame) const;
	int SpriteIndex(const string &name) const;
	const string &SpriteName(int spriteIndex) const;
	
	
private:
	map<string, int> indices;
	vector<string> names;
	vector< vector<Sprite> > sprites;
	vector<SDL_Surface *> surfaces;
};

#endif
