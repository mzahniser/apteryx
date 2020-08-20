/* Sidebar.cpp
Michael Zahniser, 10/01/07

Function definitions for the Sidebar class.
*/

#include "Sidebar.h"

#include "GameData.h"
#include "Player.h"
#include "SoundEngine.h"



void Sidebar::Draw(SDL_Surface *screen)
{
	if(!screen)
		return;
		
	int ticks = SDL_GetTicks();
	int left = screen->w - 160;

	GameData &data = GameData::GetInstance();
	data.GetSprite("sidebar").Draw(screen, left, 0);
	
	// Draw the shields bar. 1.0 shield units = 16 pixels.
	Player &player = Player::GetInstance();
	
	if(player.Shields() < 0.0f)
		return;

	SDL_Rect barRect;
	barRect.x = 0;
	barRect.w = 20;
	
	int blinkFrame = (ticks / 50) % 20;
	if((player.Shields() < 3.0f) & ((blinkFrame == 0) | (blinkFrame == 4)))
	{
		SDL_Rect blinkRect;
		blinkRect.x = left + 16;
		blinkRect.y = 160;
		blinkRect.w = 20;
		blinkRect.h = 200;
		SDL_FillRect(screen, &blinkRect, 0xFFFFFF);
		
		if(!blinkFrame)
			SoundEngine::GetInstance().Play("warning");
	}
	else
	{
		barRect.h = static_cast<int>(player.Shields() * 20.0f);
		barRect.y = 200 - barRect.h;
		data.GetSprite("powerBars", 0).Draw(barRect, screen, left + 16, 160 + barRect.y);
	}
	
	barRect.h = static_cast<int>(player.Energy() * 20.0f);
	barRect.y = 200 - barRect.h;
	data.GetSprite("powerBars", 1).Draw(barRect, screen, left + 48, 160 + barRect.y);

	barRect.h = player.UpgradeCount(player.SelectedWeapon()) * 10;
	barRect.y = 200 - barRect.h;
	data.GetSprite("powerBars", 2).Draw(barRect, screen, left + 80, 160 + barRect.y);
	
	int lasers = player.UpgradeCount(Player::LASER);
	if(lasers > 8)
		lasers = 8;
	for(int i = 0; i < lasers; ++i)
		data.GetSprite("upgrades", 4 * 3 + (ticks / 200) % 3).Draw(screen, left + 112, 328 - 24 * i);
	
	if(data.ControlledEntity()->IsPlayer())
	{
		data.GetSprite("weaponSelect", player.SelectedWeaponSprite()).Draw(screen, left + 16, 432);
		if(player.SelectedWeaponSprite())
		{
			const Sprite &sprite = data.GetSprite("upgrades", 
				4 * 3 + 3 * player.SelectedWeaponSprite() + (ticks / 200) % 3);
			sprite.Draw(screen, left + 16, 432);
		}
	}
	else
		data.GetSprite("weaponSelect", 11).Draw(screen, left + 16, 432);

	if(player.UpgradeCount(Player::RADAR))
		DrawMap(screen, ticks / 50);
}



// Draw the map view.
void Sidebar::DrawMap(SDL_Surface *screen, int frame)
{
	const SharedPtr<Level> &level = GameData::GetInstance().CurrentLevel();

	if(!SDL_MUSTLOCK(screen) || SDL_LockSurface(screen) >= 0)
	{
		Uint32 *pixels = reinterpret_cast<Uint32 *>(screen->pixels) + screen->w - 160;
		int pitch = (screen->pitch / 4);
		
		for(int x = 0; x < 64; ++x)
			for(int y = 0; y < 64; ++y)
			{
				const TileType &tile = (*level)(x, y);
				
				Uint32 color = 0x666666;
				
				if(tile.FieldTag() == 1)
					color = 0xFF0000;
				else if(tile.FieldTag() == 2)
					color = 0xFF9900;
				else if(tile.FieldTag() == 3)
					color = 0x00FF00;
				else if(tile.FieldTag() == 4)
					color = 0x0066FF;
				else if(tile.IsBlocked())
					color = 0;
				else if(tile.IsTunnelUp())
					color = 0x999999;
				else if(tile.IsTunnelDown())
					color = 0x333333;
				
				DrawMapPixel(pixels, pitch, x, y, color);
			}
		if(frame & 3)
		{
			const EntityList &entities = level->Entities();
			for(EntityList::const_iterator it = entities.begin(); it != entities.end(); ++it)
			{
				if((*it)->IsEffect())
					continue;
				
				Uint32 color = 0xFFFF00;
				if((*it)->IsEnemy())
					color = 0xFF0000;
				else if((*it)->IsDead())
					color = 0x0000FF;
				else if((*it)->IsPlayer())
					color = 0xFFFFFF;
				
				DrawMapPixel(pixels, pitch, 
					static_cast<int>((*it)->X()) + screen->w, static_cast<int>((*it)->Y()) - 1, color);
			}
		}
	
		if(SDL_MUSTLOCK(screen))
			SDL_UnlockSurface(screen);
	}
}



void Sidebar::DrawMapPixel(Uint32 *pixels, int pitch, int x, int y, Uint32 color)
{
	pixels += 16 * (pitch + 1);
	
	pixels[(2 * x) + (2 * y) * pitch] = color;
	pixels[(2 * x + 1) + (2 * y) * pitch] = color;
	pixels[(2 * x) + (2 * y + 1) * pitch] = color;
	pixels[(2 * x + 1) + (2 * y + 1) * pitch] = color;
}

