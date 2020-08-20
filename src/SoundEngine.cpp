/* SoundEngine.cpp
Michael Zahniser, 10/29/07

Function definitions for the SoundEngine singleton class.
*/

#include "SoundEngine.h"

#include "GameData.h"

#include <stdexcept>
#include <cstdlib>
#include <cmath>

using std::runtime_error;
using std::rand;
using std::sqrt;
using std::fabs;



SoundEngine &SoundEngine::GetInstance()
{
	return instance;
}



void SoundEngine::Initialize()
{
	return;
	/*if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 4, 1024))
		throw runtime_error("Unable to initialize SDL audio.");*/
}



void SoundEngine::Stop()
{
	return;
	/*for(vector<Mix_Chunk *>::iterator it = sounds.begin(); it != sounds.end(); ++it)
		Mix_FreeChunk(*it);
	
	Mix_CloseAudio();*/
}



void SoundEngine::Pause()
{
	return;
	/*Mix_Pause(-1);*/
}



void SoundEngine::Unpause()
{
	return;
	/*Mix_Resume(-1);*/
}



// Read the source of a sprite from the given data file.
void SoundEngine::Add(DataFile &in, const string &resourcePath)
{
	return;
	/*string name;
	in>>name;
	
	// Read the number of variants, if specified.
	int variantCount = 1;

	string tag;
	in>>tag;
	
	while(tag != "end")
	{
		if(tag == "variants")
			variantCount = in.ReadInt();
		
		in>>tag;
	}
	
	variants[name] = pair<int, int>(static_cast<int>(sounds.size()), variantCount);

	// Load the image, and save a pointer so it can be freed later.
	string basePath = resourcePath + name;
	
	sounds.push_back(Mix_LoadWAV((basePath + ".wav").c_str()));
	if(variantCount > 1)
	{
		string::size_type number = basePath.size();
		string path = basePath + "#.wav";
		for(int i = 2; i <= variantCount; ++i)
		{
			path[number] = '0' + i;
			sounds.push_back(Mix_LoadWAV(path.c_str()));
		}
	}*/
}



// Play the named sound. If the sound has multiple variants, a random one
// is chosen.
void SoundEngine::Play(const string &name)
{
	return;
	/*PlayWithEffect(name, 0.0f, 1.0f);*/
}



// Play the named sound at the given (x, y) position.
void SoundEngine::Play(const string &name, float x, float y)
{
	return;
	/*const Entity &observer = *GameData::GetInstance().ControlledEntity();
	float dx = x - observer.X();
	float dy = y - observer.Y();
	
	float pan = dx / fabs(dy);
	PlayWithEffect(name, pan, sqrt(dx * dx + dy * dy));*/
}



// Start and stop looped playing of a sound.
// Only one sound can be looped at a time.
void SoundEngine::PlayLoop(const string &name)
{
	return;
	/*if(loopChannel != -1)
		return;
	
	map<string, pair<int, int> >::const_iterator it = variants.find(name);
	if(it == variants.end())
		return;
	
	int index = it->second.first + rand() % it->second.second;
	loopChannel = Mix_PlayChannel(-1, sounds[index], -1);*/
}



void SoundEngine::EndLoop()
{
	return;
	/*if(loopChannel == -1)
		return;
	
	Mix_HaltChannel(loopChannel);
	loopChannel = -1;*/
}



SoundEngine::SoundEngine()
	: loopChannel(-1)
{
}



// Play the named sound with the given panning and volume.
// The panning should be between -1 and 1. The volume should be between 0 and 1.
// This is used instead of Mix_SetPosition because Mix_SetPosition apparently
// sets the volume to 0 when a sound is directly behind the observer.
void SoundEngine::PlayWithEffect(const string &name, float pan, float distance)
{
	return;
	/*if(pan < -1.0f)
		pan = -1.0f;
	if(pan > 1.0f)
		pan = 1.0f;
	
	unsigned short d = static_cast<unsigned short>(15.0f * distance);
	if(d > 255)
		return;
	
	unsigned char left = static_cast<unsigned char>(128.0f - 127.99f * pan);
	unsigned char right = static_cast<unsigned char>(128.0f + 127.99f * pan);
	
	map<string, pair<int, int> >::const_iterator it = variants.find(name);
	if(it == variants.end())
		return;
	
	int index = it->second.first + rand() % it->second.second;
	int channel = Mix_PlayChannel(-1, sounds[index], 0);
	Mix_SetPanning(channel, left, right);
	Mix_SetDistance(channel, d);*/
}



SoundEngine SoundEngine::instance;
