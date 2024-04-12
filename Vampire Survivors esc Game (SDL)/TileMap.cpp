#include "TileMap.h"

#include "../Core/Vector2.h"
#include "Renderer.h"
#include "RenderInstanceManager.h"

std::vector<TileMap*> TileMap::TileMaps;

TileMap::TileMap(std::string mapPath, int tileSize)
{
	TileMaps.push_back(this);

	_tileSize = tileSize;

	LoadMapFromFile(mapPath);

	AddInfoToRenderer();
}

TileMap::~TileMap()
{
	Clear();
}

void TileMap::LoadMapFromFile(std::string mapPath)
{
	Clear();

	int totalCount = 0;
	std::ifstream mapFile(mapPath);
	if (mapFile.is_open())
	{
		std::string lineText;

		int j = 0;
		while (std::getline(mapFile, lineText))
		{
			std::vector<char> tileRow;
			for (int i = 0; i < lineText.length(); ++i)
			{
				if (FindTile(lineText[i]))
				{
					tileRow.push_back(lineText[i]);
				}
				else
				{
					tileRow.push_back(' ');
				}
				_width = i;

				totalCount++;
			}
			_tileMap.push_back(tileRow);
			_width = tileRow.size();
			j++;
		}

		_height = _tileMap.size();


		mapFile.close();

	}
	else
	{
		std::cout << "Unable to open file";
	}
}

Tile* TileMap::FindTile(char placeholder)
{
	std::map<char, Tile*>::iterator it;
	it = _tiles.find(placeholder);
	if (it != _tiles.end())
		return _tiles[placeholder];

	Tile* tile = nullptr;
	switch (placeholder)
	{
	case 'F':
		tile = new Tile("Assets/Textures/TextureLoadingTest.png", { 16,64,16,16 }, -10);
		break;
	case 'B':
		tile = new Tile("Assets/Textures/TextureLoadingTest.png", { 32,64,16,16 }, -10);
		break;
	case 'Q':
		tile = new Tile("Assets/Textures/TextureLoadingTest.png", { 16,12,16,16 }, 0);
		break;
	case 'W':
		tile = new Tile("Assets/Textures/TextureLoadingTest.png", { 32,12,16,16 }, 0);
		break;
	case 'E':
		tile = new Tile("Assets/Textures/TextureLoadingTest.png", { 48,12,16,16 }, 0);
		break;
	}

	if (!tile)
		return nullptr;

	_tiles.insert(std::make_pair(placeholder, tile));

	return tile;
}

void TileMap::Clear()
{
	_tileMap.clear();
	for (int i = 0; i < _tiles.size(); ++i)
	{
		delete _tiles[i];
	}
	_tiles.clear();
}

void TileMap::AddInfoToRenderer()
{
	for (int i = 0; i < _tileMap.size(); ++i)
	{
		for (int j = 0; j < _tileMap[i].size(); ++j)
		{
			if (_tileMap[i][j] == ' ')
				continue;
			Vector2 pos = { (j * _tileSize) - (_width * _tileSize) / 2, ((_tileMap.size()-1 - i) * _tileSize) - (_height * _tileSize) / 2 };
			Tile* tile = FindTile(_tileMap[i][j]);
			if (!tile)
				continue;
			RenderInstanceManager::instance().GetRenderer("main")->AddToRenderList(
				RenderInfo(pos, { _tileSize, _tileSize }, tile->Texture, tile->Src, false, tile->SortingLayer, {255,255,255,255})
			);
		}
	}
}