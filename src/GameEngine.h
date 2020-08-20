/* GameEngine.h
Michael Zahniser, 09/22/07

Header files for the GameEngine class.
*/

#ifndef __MZ_GAME_ENGINE_H__
#define __MZ_GAME_ENGINE_H__


#include "Sprite.h"
#include "GameData.h"
#include "CroppedView.h"
#include "Controlled_AI.h"
#include "Sidebar.h"

#include <SDL2/SDL.h>
#include <string>
#include <vector>

using std::string;
using std::vector;



class GameEngine {
public:
	GameEngine(string resourcePath);
	~GameEngine();
	
	void Run();
	
	
private:
	void Render();
	
	void StepAll(int ticks);
	void Fade(int ticks);
	
	void LoadImage(SDL_Surface *&surface, string path);
	
	bool CheckWallCollisions(Entity &object);
	bool DoWallCollision(Object &object, int gridX, int gridY, float wallX, float wallY);

	void TimeStep(float timeStep);
	void Animate();
	
	bool LineOfSight(const Object &from, const Object &to) const;
	
	void Pause();
	void Unpause();
	
	
private:
	Controlled_AI control;

	int lastTimestamp;
	int lastAnimate;
	
	int transitionTime;
	int newLevelIndex;
	
	int lockedWeapon;
	
	bool showVisibility;
	
	CroppedView view;
	Sidebar sidebar;
	
	SDL_Window *window;
	SDL_Surface *screen;
	
	SDL_Surface *tilesSurface;
	vector<Sprite> tiles;
	
	const Sprite *shadow;
	
	string savePath;
	
	bool isPaused;
	bool isShowingMenu;
	bool isShowingKeys;
	bool mouseCheat;
	
	
private:
	static const int TIME_STEP = 10;
	static const int ANIMATION_STEP = 50;

	static const int SCREEN_WIDTH = 880;
	static const int SCREEN_HEIGHT = 720;
	static const int TILE_SIZE = 48;
	static const int VIEW_WIDTH = 720;
	static const int VIEW_HEIGHT = 720;
	static const int VIEW_LEFT = 0;
	static const int VIEW_TOP = 0;
};

#endif
