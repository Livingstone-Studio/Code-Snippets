#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "Structs.h"
#include "AssetLoader.h"
#include "Time.h"
#include "Input.h"

class AssetLoader;
class Time;
class Input;

using namespace std;

class Sprite
{
public:

	Sprite();
	Sprite(SDL_Renderer* renderer, string imgFile);
	Sprite(SDL_Renderer* renderer, string imgFile, Vector2 offset);

	~Sprite();

	void SetSprite();
	void SetSprite(SDL_Renderer* renderer, string name);

	SDL_Texture* GetSpriteTexture();

	SDL_Rect GetSrcRect() { return m_srcRect; }
	SDL_Rect GetDesRect() { return m_desRect; }

	Sprite GetSprite() { return *this; }

	void SetDesRect(float x, float y, float xScale = 1, float yScale = 1);
	void SetDesRect(Vector2 position, float xScale = 1, float yScale = 1);
	void SetDesRect(float x, float y, Vector2 scale);
	void SetDesRect(Vector2 position, Vector2 scale);

	void SetSrcRect(Vector2 position);
	void SetSrcRect(Transform t);
	
	void SetOffset(Vector2 offset) { m_offset = offset;  }

	void Update();

	Vector2 GetImageSize() { return Vector2(imageWidth, imageHeight); }
	Vector2 GetBaseImageSize() { return Vector2(BASE_SPRITE_WIDTH, BASE_SPRITE_HEIGHT); }

private:

	string m_name{ "" };

	SDL_Texture* m_sprite{ nullptr };

	SDL_Rect m_srcRect;
	SDL_Rect m_desRect;

	Vector2 m_offset{ 0, 0 };

	const int BASE_SPRITE_WIDTH{ 32 };
	const int BASE_SPRITE_HEIGHT{ 32 };

	int imageWidth{ 0 };
	int imageHeight{ 0 };


	bool m_custom_size{ false };
	Vector2 m_ui;
};