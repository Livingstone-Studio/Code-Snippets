#pragma once

#include "SDL.h"
#include "SDL_image.h"

class GridMap;
class Player;

struct Sprite 
{
	int type;
	bool state;
	SDL_Texture* texture;
	int position[3] = {0,0,0};
};

class RaycastRenderer
{
public:
	RaycastRenderer();
	~RaycastRenderer();

	void Inputs();
	void Update();
	void Render();

	void ToggleFullscreen();

private:
	void DrawRays();
	void DrawSprites();

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	SDL_Texture* m_wall_texture;
	
	SDL_Texture* m_gun_texture;

	const int WINDOW_SIZE[2] = { 960, 580 };
	int wind_size[2] = { 960, 580 };
	bool m_fullscreen = false;

	Player* m_player;
	GridMap* m_current_level;

	Sprite m_sprites[4];

	float m_illum;
};

