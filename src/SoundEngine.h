/* SpriteEngine.h
Michael Zahniser, 10/29/07

Header file for the SoundEngine class, which stores a list of sounds.
*/

#ifndef __MZ_SOUND_ENGINE_H__
#define __MZ_SOUND_ENGINE_H__


#include "DataFile.h"

#include <SDL2/SDL.h>
//#include <SDL/SDL_mixer.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

using std::map;
using std::string;
using std::pair;
using std::vector;



class SoundEngine {
public:
	static SoundEngine &GetInstance();
	
	void Initialize();
	void Stop();
	
	void Pause();
	void Unpause();
	
	// Read the source of a sprite from the given data file.
	void Add(DataFile &in, const string &resourcePath);

	// Play the named sound. If the sound has multiple variants, a random one
	// is chosen.
	void Play(const string &name);
	// Play the named sound at the given (x, y) position.
	void Play(const string &name, float x, float y);
	// Start and stop looped playing of a sound.
	// Only one sound can be looped at a time.
	void PlayLoop(const string &name);
	void EndLoop();
	
	
private:	
	SoundEngine();

	// Play the named sound with the given panning and volume.
	// The panning should be between -1 (full left) and 1 (full right).
	// The distance should be in world units.
	void PlayWithEffect(const string &name, float pan, float distance);


private:
	map<string, pair<int, int> > variants;
	//vector<Mix_Chunk *> sounds;
	
	int loopChannel;


private:
	static SoundEngine instance;
	
	
private:
	// Do not allow copying.
	SoundEngine(const SoundEngine &);
	SoundEngine &operator=(const SoundEngine &);
};

#endif
