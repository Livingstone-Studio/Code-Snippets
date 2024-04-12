#pragma once

#include "../Core/AssetManager.h"
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <SDL.h>

// RenderInfo data, texture, src, sorting layer are extrinsic while position, size are intrinsic
class Tile
{
public:
	Tile(std::string texture_path, SDL_Rect src, int layer) 
		: Texture{ AssetManager::instance().LoadImage(texture_path) }, Src{ src }, SortingLayer{ layer }
	{
	}

	SDL_Texture* Texture = nullptr;
	SDL_Rect Src = {};
	int SortingLayer = 0;
};

class TileMap
{
public:
	TileMap(std::string mapPath, int tileSize);
	~TileMap();

	static std::vector<TileMap*> TileMaps;

protected:
	std::vector<std::vector<char>> _tileMap;
	std::map<char, Tile*> _tiles;
	float _tileSize{ 0.0f };
	Uint8 _width{ 0 };
	Uint8 _height{ 0 };

public:
	void LoadMapFromFile(std::string mapPath);
	Tile* FindTile(char placeholder);
	void Clear();
	
protected:
	void AddInfoToRenderer();
};
