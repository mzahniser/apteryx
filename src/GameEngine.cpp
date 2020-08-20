/* GameEngine.cpp
Michael Zahniser, 09/22/07

Function definitions for the GameEngine class.
*/

#include "GameEngine.h"

#include "EntityList.h"
#include "LevelChanges.h"
#include "Player.h"
#include "SoundEngine.h"

#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <cmath>
#include <sys/stat.h>

using std::runtime_error;
using std::floor;
using std::ceil;
using std::fabs;
using std::sqrt;



// This class is to make sure that SDL_Quit() is called if an error is throw during
// initialization of the program.
class SDLStart {
public:
	SDLStart() : isFullyInitialized(false)
	{ if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		throw runtime_error("Unable to initialize SDL."); }
	
	~SDLStart() { if(!isFullyInitialized) SDL_Quit(); }
	
	void SetSucceeded() { isFullyInitialized = true; }
	
private:
	bool isFullyInitialized;
};



GameEngine::GameEngine(string resourcePath)
	: view(screen, VIEW_LEFT, VIEW_TOP, VIEW_WIDTH, VIEW_HEIGHT, TILE_SIZE),
	newLevelIndex(-1), transitionTime(-1), showVisibility(true), 
	isPaused(false), isShowingMenu(false), isShowingKeys(true), mouseCheat(false)
{
	// Start SDL. This object makes sure that SDL_Quit() is called if we bail out
	// of this function before calling start.SetSucceeded()
	SDLStart start;

	// Create a window.
	window = SDL_CreateWindow("Apteryx", SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_SWSURFACE);
	screen = SDL_GetWindowSurface(window);
	if(!screen)
		throw runtime_error("Unable to create window.");
	
	// Start the audio engine. This must be done before GameData loads the sounds.
	SoundEngine::GetInstance().Initialize();
	
	// Load the resources.
	GameData &data = GameData::GetInstance();
	data.Initialize(resourcePath);
	savePath = getenv("HOME") + string("/.apteryx");
	{
		struct stat st;
		if(!stat(savePath.c_str(), &st) && S_ISREG(st.st_mode))
		{
			isShowingKeys = false;
			data.LoadGame(savePath);
		}
		else
			data.SaveGame(savePath);
	}
	
	shadow = &data.GetSprite("shadow");
	
	// Load the tile sprites.
	LoadImage(tilesSurface, resourcePath + "tiles.png");
	
	// Set the tile locations.
	for(int i = 0; i < 12 * 18; ++i)
	{
		int set = (i / 18);
		int row = (i / 3) % 6;
		int col = set * 3 + (i % 3);
		
		tiles.push_back(Sprite(tilesSurface, col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}
	
	lastTimestamp = SDL_GetTicks();
	lastAnimate = lastTimestamp;

	// We have initialized without a hitch.
	start.SetSucceeded();
}



GameEngine::~GameEngine()
{
	SDL_Quit();
}



void GameEngine::Run()
{
	GameData &data = GameData::GetInstance();
	
	while(true)
	{
		Render();

		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				/*case SDL_MOUSEMOTION:
					if(mouseCheat && event.motion.x < VIEW_WIDTH)
					{
						double x = static_cast<double>(event.motion.x - VIEW_WIDTH / 2);
						double y = static_cast<double>(event.motion.y - VIEW_HEIGHT / 2);
					
						control.CheatSetVelocity(x * .05, y * .05);
					}
					break;
					
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT)
						control.KeyDown(Controlled_AI::PRIMARY);
					break;
					
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT)
						control.KeyUp(Controlled_AI::PRIMARY);
					break;*/
					
				case SDL_KEYDOWN:
					if(event.key.keysym.mod & (KMOD_GUI | KMOD_CTRL))
					{
						// Fade in.
						transitionTime = SDL_GetTicks();
						
						if(event.key.keysym.sym == 'n')
							data.NewGame();
						else if(event.key.keysym.sym == 'o')
							data.LoadGame(savePath);
						else if(event.key.keysym.sym == 's')
							data.SaveGame(savePath);
						else if(event.key.keysym.sym == 'q')
							return;
						else
							transitionTime = -1;

						if(event.key.keysym.sym == 'p')
							isShowingMenu = !isShowingMenu;
						else if(event.key.keysym.sym == 'k')
							isShowingKeys = !isShowingKeys;
						else if(event.key.keysym.sym == 'c')
							mouseCheat = !mouseCheat;
					}

					else if(event.key.keysym.sym == SDLK_UP)
						control.KeyDown(Controlled_AI::UP);

					else if(event.key.keysym.sym == SDLK_DOWN)
						control.KeyDown(Controlled_AI::DOWN);

					else if(event.key.keysym.sym == SDLK_LEFT)
						control.KeyDown(Controlled_AI::LEFT);

					else if(event.key.keysym.sym == SDLK_RIGHT)
						control.KeyDown(Controlled_AI::RIGHT);

					else if(event.key.keysym.sym == SDLK_LSHIFT)
						control.KeyDown(Controlled_AI::PRIMARY);

					else if(event.key.keysym.sym == 'a')
						control.KeyDown(Controlled_AI::SECONDARY);

					else if(event.key.keysym.sym == 's')
						Player::GetInstance().SelectNextWeapon();
					
					else if(event.key.keysym.sym == SDLK_TAB)
						GameData::GetInstance().SwitchControl();
						
					else if(event.key.keysym.sym == SDLK_CAPSLOCK)
						lockedWeapon = Player::GetInstance().SelectedWeapon();

					else if(event.key.keysym.sym == 'z' && data.CanTunnelUp())
						newLevelIndex = data.CurrentLevelIndex() - 1;

					else if(event.key.keysym.sym == 'x' && data.CanTunnelDown())
						newLevelIndex = data.CurrentLevelIndex() + 1;
						
					else if(event.key.keysym.sym == 'v')
						showVisibility = !showVisibility;

					break;
										
				case SDL_KEYUP:
					if(event.key.keysym.sym == SDLK_UP)
						control.KeyUp(Controlled_AI::UP);

					else if(event.key.keysym.sym == SDLK_DOWN)
						control.KeyUp(Controlled_AI::DOWN);

					else if(event.key.keysym.sym == SDLK_LEFT)
						control.KeyUp(Controlled_AI::LEFT);

					else if(event.key.keysym.sym == SDLK_RIGHT)
						control.KeyUp(Controlled_AI::RIGHT);

					else if(event.key.keysym.sym == SDLK_LSHIFT)
						control.KeyUp(Controlled_AI::PRIMARY);
					
					else if(event.key.keysym.sym == 'a')
						control.KeyUp(Controlled_AI::SECONDARY);

					else if(event.key.keysym.sym == SDLK_CAPSLOCK)
						Player::GetInstance().SelectWeapon(lockedWeapon);

					break;

				case SDL_QUIT:
					return;
					
				default:
					break;
			}
		}
	}
}


	
void GameEngine::Render()
{
	if(isPaused)
		return;
		
	const SharedPtr<Level> &level = GameData::GetInstance().CurrentLevel();

	// Ask SDL for the time in milliseconds
	int ticks = SDL_GetTicks();	
	
	if(!isShowingMenu)
		StepAll(ticks);
	
	Entity::Ptr controlledEntity = GameData::GetInstance().ControlledEntity();
	// Center the view on the player.
	view.SetCenter(
		controlledEntity->X(), 
		controlledEntity->Y(), 
		level->Width(), level->Height());
	
	int tileFrame = ticks / 200;
	for(int y = view.FirstTileY(); y <= view.LastTileY(); ++y)
		for(int x = view.FirstTileX(); x <= view.LastTileX(); ++x)
			view.DrawTile(tiles[(*level)(x, y).SpriteIndex(tileFrame)], x, y);
			
	const EntityList &entities = level->Entities();
	const GameData &data = GameData::GetInstance();
	for(EntityList::const_iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if(!(*it)->IsEffect())
			view.DrawEntity(*shadow, (*it)->X() + .15, (*it)->Y() + .15);
		const Sprite &sprite = data.GetSprite((*it)->SpriteName(), (*it)->SpriteFrame());
		view.DrawEntity(sprite, (*it)->X(), (*it)->Y());
	}
	
	if(showVisibility)
		view.DrawShadows();
	
	Fade(ticks);

	sidebar.Draw(screen);
	
	if(isShowingMenu)
	{
		const Sprite &info = data.GetSprite("menu", 0);
		int x = (screen->w - info.Width()) / 2;
		int y = (screen->h - info.Height()) / 2;
		info.Draw(screen, x, y);
	}
	else if(isShowingKeys)
		data.GetSprite("keys", 0).Draw(screen, 0, 0);

	// Tell SDL to update the whole screen
	SDL_UpdateWindowSurface(window);
}



void GameEngine::StepAll(int ticks)
{
	// Entities don't move if fading out or fading in.
	if((newLevelIndex < 0) & (transitionTime < 0))
	{
		// Handle long pauses (e.g. debugger breaks).
		if(ticks - lastTimestamp > 500)
		{
			lastTimestamp = ticks;
			lastAnimate = ticks;
		}
		
		// Step all objects in small increments until we reach the current time.
		const float TIME_SLICE = static_cast<float>(TIME_STEP) / 1000.0;
		while(lastTimestamp < ticks)
		{
			lastTimestamp += TIME_STEP;
			
			// Animate less frequently than we check collisions.
			// Animate() also calls the AIs, which can be processor intensive
			// because line-of-sight and other calculations are involved.
			for( ; lastAnimate + ANIMATION_STEP <= lastTimestamp; lastAnimate += ANIMATION_STEP)
				Animate();

			// Move the objects one time slice and check for collisions.
			TimeStep(TIME_SLICE);
		}
	}
}



void GameEngine::Fade(int ticks)
{
	if(newLevelIndex >= 0)
	{
		if(transitionTime == -1)
			transitionTime = ticks;
		
		int alpha = 255 - (ticks - transitionTime);
		if(alpha < 0)
		{
			alpha = 0;
			transitionTime = ticks;
			GameData::GetInstance().SetCurrentLevel(newLevelIndex);
			newLevelIndex = -1;
		}
		
		view.Fade(alpha);
	}
	else if(transitionTime >= 0)
	{
		int alpha = (ticks - transitionTime);
		if(alpha > 255)
			transitionTime = -1;
		else
			view.Fade(alpha);
	}
}



void GameEngine::LoadImage(SDL_Surface *&surface, string path)
{
	surface = IMG_Load(path.c_str());
	if(!surface)
		throw runtime_error("Unable to load image: " + path);
}



bool GameEngine::CheckWallCollisions(Entity &object)
{
	if(!object.Radius())
		return false;
	
	const SharedPtr<Level> &level = GameData::GetInstance().CurrentLevel();

	int objectX = static_cast<int>(object.X());
	int objectY = static_cast<int>(object.Y());
	
	if((objectX < 0) | (objectY < 0) | (objectX >= level->Width()) | (objectY >= level->Height()))
	{
		LevelChanges::GetInstance().Remove(&object);
		return false;
	}
	
	// If the current grid square is blocked...
	if((*level)(objectX, objectY).IsBlocked())
	{
		// Find out when we entered this square.
		float blockedX = (object.VelocityX() > 0.0f) ? floor(object.X()) : ceil(object.X());
		float blockedY = (object.VelocityY() > 0.0f) ? floor(object.Y()) : ceil(object.Y());
		
		float timeX = (object.X() - blockedX) / object.VelocityX();
		float timeY = (object.Y() - blockedY) / object.VelocityY();
		
		if(timeX < timeY)
			DoWallCollision(object, objectX, objectY, blockedX, object.Y());
		else
			DoWallCollision(object, objectX, objectY, object.X(), blockedY);
			
		return true;
	}
	
	// Get the grid corner closest to the object.
	float gridX = floor(object.X() + .5f);
	float gridY = floor(object.Y() + .5f);
	
	int sideX = objectX + 2 * (gridX > object.X()) - 1;
	int sideY = objectY + 2 * (gridY > object.Y()) - 1;
	
	float dx = object.X() - gridX;
	float dy = object.Y() - gridY;
	
	if(fabs(dx) < object.Radius() && 
			DoWallCollision(object, sideX, objectY, gridX, object.Y()))
		{}
	else if(fabs(dy) < object.Radius() &&
			DoWallCollision(object, objectX, sideY, object.X(), gridY))
		{}
	else if(sqrt(dx * dx + dy * dy) < object.Radius() &&
			DoWallCollision(object, sideX, sideY, gridX, gridY))
		{}		
	else
		return false;
		
	return true;
}



bool GameEngine::DoWallCollision(Object &object, int gridX, int gridY, float wallX, float wallY)
{
	const GameData &data = GameData::GetInstance();
	
	const TileType &tileType = (*data.CurrentLevel())(gridX, gridY);

	if(!tileType.IsBlocked())
		return false;
		
	object.DoBounce(wallX, wallY, tileType, data.GetMaterial(tileType).Bounce());
	
	return true;
}



void GameEngine::TimeStep(float timeStep)
{
	SharedPtr<Level> &level = GameData::GetInstance().CurrentLevel();
	EntityList &entities = level->Entities();

	// Move all objects.
	for(EntityList::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->Move(timeStep);
		
	// Handle collisions between objects.
	for(EntityList::iterator it = entities.begin(); it != entities.end(); ++it)
		for(EntityList::iterator otherIt = entities.begin(); otherIt != it; ++otherIt)
			(*it)->DoCollision(**otherIt);
			
	// Handle collisions with walls. Handle double bounces in one timestep.
	for(EntityList::iterator it = entities.begin(); it != entities.end(); ++it)
		if(CheckWallCollisions(**it))
			CheckWallCollisions(**it);
			
	// Apply any changes resulting from the above actions.
	level->ApplyChanges();
}



void GameEngine::Animate()
{
	SharedPtr<Level> &level = GameData::GetInstance().CurrentLevel();
	EntityList &entities = level->Entities();
	Entity::Ptr &player = GameData::GetInstance().PlayerEntity();
	
	// Apply all internal AIs.	
	for(EntityList::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if(LineOfSight(**it, *player))
		{
			if(Player::GetInstance().WeaponEffect())
				Player::GetInstance().WeaponEffect()->ApplyTo(**it);
			else
				(*it)->ApplyAI(*player);
		}
		else
			(*it)->ApplyAI();
	}

	// Apply global AIs.
	control.ApplyTo(*GameData::GetInstance().ControlledEntity());
	
	// Move all objects forward one animation step.
	for(EntityList::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->Animate();
		
	// Animate the player.
	Player::GetInstance().Animate();
			
	// Apply any changes resulting from the above actions.
	level->ApplyChanges();	
}



bool GameEngine::LineOfSight(const Object &from, const Object &to) const
{
	return GameData::GetInstance().CurrentLevel()->LineOfSight(from.X(), from.Y(), to.X(), to.Y()) > 0.0f;
}



void GameEngine::Pause()
{
	isPaused = true;
	SoundEngine::GetInstance().Pause();
}



void GameEngine::Unpause()
{
	isPaused = false;
	SoundEngine::GetInstance().Unpause();
}
