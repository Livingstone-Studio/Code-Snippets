#include "RaycastRenderer.h"

#include "Input.h"
#include "Time.h"
#include "RaycasterMaths.h"

#include "Player.h"
#include "GridMap.h"

#include <iostream>
#include <cmath>

RaycastRenderer::RaycastRenderer()
{
	// SDL initialization.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		// Error
		std::cout << "Failed to intialise SDL. Error: " << SDL_GetError() << std::endl;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0)
	{

	}

	m_window = SDL_CreateWindow("Raycast Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE[0], WINDOW_SIZE[1], SDL_WINDOW_RESIZABLE);
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);

	m_player = new Player();
	m_current_level = new GridMap(m_renderer);

	m_wall_texture = SDL_CreateTextureFromSurface(m_renderer, IMG_Load("wall textures.png"));

	m_gun_texture = SDL_CreateTextureFromSurface(m_renderer, IMG_Load("gun holding-export.png"));

	if (m_wall_texture == NULL)
		std::cout << "Failed to load" << std::endl;

	m_sprites[0].type = 1;
	m_sprites[0].state = 1;
	m_sprites[0].texture = m_wall_texture;
	m_sprites[0].position[0] = (7 * 64) / 2;
	m_sprites[0].position[1] = (7 * 64) / 4;
	m_sprites[0].position[2] = 1;
}

RaycastRenderer::~RaycastRenderer()
{
	if (m_current_level)
		delete m_current_level;

	if (m_player)
		delete m_player;

	if (m_wall_texture)
	{
		SDL_DestroyTexture(m_wall_texture);
	}

	if (m_gun_texture)
	{
		SDL_DestroyTexture(m_gun_texture);
	}

	// Window cleanup.
	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	// Renderer cleanup.
	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	// SDL_Image cleanup.
	IMG_Quit();

	// SDL cleanup.
	SDL_Quit();
}

void RaycastRenderer::Inputs()
{
	float moveInterval = 0.5f;

	float move[2];
	if (Input::GetKey(SDL_SCANCODE_W))
	{
		move[0] = moveInterval;
		move[1] = moveInterval;
		m_player->MoveForward(m_current_level, move);
	}
	else if (Input::GetKey(SDL_SCANCODE_S))
	{
		move[0] = -moveInterval;
		move[1] = -moveInterval;
		m_player->MoveForward(m_current_level, move);
	}
	if (Input::GetKey(SDL_SCANCODE_A))
	{
		move[0] = -moveInterval;
		move[1] = -moveInterval;
		m_player->MoveRight(m_current_level, move);
	}
	else if (Input::GetKey(SDL_SCANCODE_D))
	{
		move[0] = moveInterval;
		move[1] = moveInterval;
		m_player->MoveRight(m_current_level, move);
	}

	if (Input::GetKeyDown(SDL_SCANCODE_M))
	{
		m_player->ToggleDrawState();
		m_current_level->ToggleDrawState();
	}

	if (Input::GetKey(SDL_SCANCODE_LEFT))
		m_player->Rotate(-0.002f);
	else if (Input::GetKey(SDL_SCANCODE_RIGHT))
		m_player->Rotate(0.002f);
}

void RaycastRenderer::Update()
{
	SDL_GetWindowSize(m_window, &wind_size[0], &wind_size[1]);
}

void RaycastRenderer::Render()
{
	SDL_RenderClear(m_renderer);


	SDL_Rect des1 = { 0, 0, wind_size[0], wind_size[1]/2 };
	SDL_SetRenderDrawColor(m_renderer, 25 * m_illum, 25 * m_illum, 25 * m_illum, 255);
	SDL_RenderFillRect(m_renderer, &des1);

	DrawRays();

	m_current_level->Draw(m_renderer);

	m_player->Draw(m_renderer);

	float res_scale[2] = { (float)wind_size[0] / (float)WINDOW_SIZE[0], (float)wind_size[1] / (float)WINDOW_SIZE[1] };

	int w = 250 * res_scale[0];
	int h = 250 * res_scale[1];

	SDL_Rect src = { 0,0,100,100 };
	SDL_Rect des = { wind_size[0] - w, wind_size[1] - h, w, h };
	SDL_SetTextureColorMod(m_gun_texture, 255 * m_illum, 255 * m_illum, 255 * m_illum);
	SDL_RenderCopy(m_renderer, m_gun_texture, &src, &des);

	SDL_SetRenderDrawColor(m_renderer, 55 * m_illum, 20 * m_illum, 0 * m_illum, 255);
	SDL_RenderPresent(m_renderer);
}

void RaycastRenderer::ToggleFullscreen()
{
	if (!m_fullscreen)
	{
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		m_fullscreen = true;
	}
	else if (m_fullscreen)
	{
		SDL_SetWindowFullscreen(m_window, 0);
		m_fullscreen = false;
	}
}

void RaycastRenderer::DrawRays()
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo;

	float disT = 0;

	int rayAmount = 120;

	float p_pos[2] = { m_player->GetPosition()[0],m_player->GetPosition()[1] };

	for (r = 0; r < rayAmount; r++)
	{
		ra = m_player->GetForward() + (r * 0.0174533f) / 2 - (rayAmount / 2 * 0.0174533f) / 2;

		if (ra > M_PI * 2)
			ra -= M_PI * 2;
		else if (ra < 0)
			ra += M_PI * 2;

		float disH = 10000000;
		float hx = p_pos[0];
		float hy = p_pos[1];

		float disV = 10000000;
		float vx = p_pos[0];
		float vy = p_pos[1];

		// Horizontal Line Checks
		dof = 0;
		float aTan = -1 / tan(ra);
		if (ra > M_PI)
		{
			ry = (((int)p_pos[1] >> 6) << 6) - 0.0001f;
			rx = (p_pos[1] - ry) * aTan + p_pos[0];
			yo = -64;
			xo = -yo * aTan;
		}
		else if (ra < M_PI)
		{
			ry = (((int)p_pos[1] >> 6) << 6) + 64;
			rx = (p_pos[1] - ry) * aTan + p_pos[0];
			yo = 64;
			xo = -yo * aTan;
		}
		else
		{
			rx = p_pos[0];
			ry = p_pos[1];
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;

			int pos[2] = { mx,my };
			if (my >= 0 && mx >= 0 && my < 8 && mx < 8 && m_current_level->GetPositionValue(pos[0], pos[1]) == 1)
			{
				dof = 8;
				hx = rx;
				hy = ry;
				disH = RaycasterMaths::Distance(p_pos[0], p_pos[1], hx, hy, ra);
			}
			else {
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}

		// Vertical Line Checks
		dof = 0;
		float nTan = -tan(ra);
		if (ra > M_PI * 0.5f && ra < M_PI * 1.5f)
		{
			rx = (((int)p_pos[0] >> 6) << 6) - 0.0001f;
			ry = (p_pos[0] - rx) * nTan + p_pos[1];
			xo = -64;
			yo = -xo * nTan;
		}
		else if (ra < M_PI * 0.5f || ra > M_PI * 1.5f)
		{
			rx = (((int)p_pos[0] >> 6) << 6) + 64;
			ry = (p_pos[0] - rx) * nTan + p_pos[1];
			xo = 64;
			yo = -xo * nTan;
		}
		else
		{
			rx = p_pos[0];
			ry = p_pos[1];
			dof = 8;
		}

		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;

			int pos[2] = { mx,my };
			if (my >= 0 && mx >= 0 && my < 8 && mx < 8 && m_current_level->GetPositionValue(pos[0], pos[1]) == 1)
			{
				dof = 8;
				vx = rx;
				vy = ry;
				disV = RaycasterMaths::Distance(p_pos[0], p_pos[1], vx, vy, ra);
			}
			else {
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}

		if (disH > disV)
		{
			rx = vx;
			ry = vy;
			SDL_SetRenderDrawColor(m_renderer, 125, 0, 0, 255);
			disT = disV;
		}
		else if (disH < disV)
		{
			rx = hx;
			ry = hy;
			SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
			disT = disH;
		}

		// Draw 3D Walls

		float ca = m_player->GetForward() - ra;
		if (ca < 0)
			ca += 2 * M_PI;
		else if (ca > 2 * M_PI)
			ca -= 2 * M_PI;

		m_illum = disT / 150.0f;

		float disTFish = disT;

		disT = disT * cos(ca);

		float lineH;

		if (disT != 0)
			lineH = (32 * wind_size[1]) / disT;
		else
			lineH = 0;

		float ty_step = 100.0f / lineH;
		float ty_off = 0;
		if (lineH > wind_size[1])
		{
			ty_off = (lineH - wind_size[1]) / 2.0f;
			lineH = wind_size[1];
		}

		float lineO = wind_size[1] / 2 - lineH * 0.5f;

		float tx;



		if (disH < disV)
		{
			tx = (int)(rx / 2.0f) % 10;
		
			if (ra < M_PI)
				tx = 9 - tx;
			m_illum = 0.7f;
		}
		else
		{
			tx = (int)(ry / 2.0f) % 10;

			if (ra > M_PI * 0.5f && ra < M_PI * 1.5f)
				tx = 9 - tx;
			m_illum = 0.4f;
		}

		//if (m_illum > 1.0f)
		//	m_illum = 1.0f;
		//else if (m_illum < 0.0f)
		//	m_illum = 0.0f;

		//m_illum = 1.0f - m_illum;

		//if (m_illum > 1.0f)
		//	m_illum = 1.0f;
		//else if (m_illum < 0.0f)
		//	m_illum = 0.0f;


		SDL_SetTextureColorMod(m_wall_texture, 255 * m_illum, 255 * m_illum, 255 * m_illum);

		//

		int texture_offset[2] = { (int)(rx / 64), (int)(ry / 64) };

		if (texture_offset[0] < 0 || texture_offset[0] >= 8)
			texture_offset[0] = 0;
		if (texture_offset[1] < 0 || texture_offset[1] >= 8)
			texture_offset[1] = 0;

		//

		SDL_Rect srcRect = { tx * 10 * 5, 0, 1, 100 };
		SDL_Rect desRect = { r * 12.8f + 0, lineO, 13, (lineH) };

		SDL_RenderCopy(m_renderer, m_wall_texture, &srcRect, &desRect);
	}

	m_illum = 0.4f;

	DrawSprites();
}

void RaycastRenderer::DrawSprites()
{
	float direction[3] = { m_player->GetPosition()[0] - m_sprites[0].position[0], m_player->GetPosition()[1] - m_sprites[0].position[1], m_sprites[0].position[2]};
	
	float cs = cos(m_player->GetForward()), sn = sin(m_player->GetForward());

	float a = direction[1] * cs + direction[0] * sn;
	float b = direction[0] * cs + direction[1] * sn;
	direction[0] = a;
	direction[1] = b;


	//tx = p_pos[0] * 1 - cos(ra) * (220) * 64 / dy / raFix;
	//float ty = p_pos[1] * 1 - sin(ra) * (220) * 64 / dy / raFix;

	direction[0] = (direction[0] * 108.0f / direction[1]) + (120 / 2);
	direction[1] = (direction[2] * 108.0f / direction[1]) + (80 / 2);

	SDL_Rect srcRect = { 0, 0, 100, 100 };
	SDL_Rect desRect = {direction[0]*8, direction[1] * 8, 20, 20};

	SDL_RenderCopy(m_renderer, m_sprites[0].texture, &srcRect, &desRect);
}
