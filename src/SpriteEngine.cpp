/* SpriteEngine.cpp
Michael Zahniser, 10/12/07

Function definitions for the SpriteEngine class.
*/

#include "SpriteEngine.h"

#include <SDL2/SDL_image.h>
#include <stdexcept>

using std::runtime_error;



SpriteEngine::SpriteEngine()
{
}



SpriteEngine::~SpriteEngine()
{
	for(vector<SDL_Surface *>::const_iterator it = surfaces.begin(); it != surfaces.end(); ++it)
		SDL_FreeSurface(*it);
	surfaces.clear();
}



// Read the source of a sprite from the given data file.
void SpriteEngine::Add(DataFile &in, const string &resourcePath)
{
	string name;
	in>>name;
	// This name will be associated with the next image.
	indices[name] = static_cast<int>(sprites.size());
	names.push_back(name);
	
	// Read the number of rows and columns, if specified.
	int rows = 1;
	int columns = 1;
	
	string tag;
	in>>tag;
	
	while(tag != "end")
	{
		if(tag == "rows")
			rows = in.ReadInt();
		else if(tag == "columns")
			columns = in.ReadInt();
		
		in>>tag;
	}
	
	// Load the image, and save a pointer so it can be freed later.
	SDL_Surface *surface = IMG_Load((resourcePath + name + ".png").c_str());
	surfaces.push_back(surface);
	
	// Find the dimensions of a single sprite. Assume that the image size is divisible
	// by the given number of rows and columns.
	int spriteWidth = surface->w / columns;
	int spriteHeight = surface->h / rows;
	
	// Create the vector of sprites for this image and get a reference to it.
	sprites.push_back(vector<Sprite>());
	vector<Sprite> &frames = sprites.back();
	
	// Add all the sprites in the image, from left to right and then from top to bottom.
	for(int r = 0; r < rows; ++r)
		for(int c = 0; c < columns; ++c)
			frames.push_back(
				Sprite(surface, 
					c * spriteWidth, r * spriteHeight, 
					spriteWidth, spriteHeight));
}



const Sprite &SpriteEngine::GetSprite(const string &name, int frame) const
{
	return GetSprite(SpriteIndex(name), frame);
}



const Sprite &SpriteEngine::GetSprite(int spriteIndex, int frame) const
{
	return sprites[spriteIndex][frame];
}



int SpriteEngine::SpriteIndex(const string &name) const
{
	map<string, int>::const_iterator it = indices.find(name);
	return it->second;
}



const string &SpriteEngine::SpriteName(int spriteIndex) const
{
	return names[spriteIndex];
}

