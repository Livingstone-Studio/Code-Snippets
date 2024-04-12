#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Input.h"
#include "Time.h"
#include "Maths.h"
#include "AssetLoader.h"
#include "LevelHandler.h"

#include "Camera.h"

#include "GameObject.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Orc.h"
#include "Slime.h"
#include "Collectable.h"
#include "AudioManager.h"
#include "ScoreManager.h"

#include <vector>

using namespace std;

class Camera;

class GameObject;
class Character;
class Player;
class Enemy;

enum GameState {
	SMainMenu, Gameplay
};

class Game
{
public:

	Game();

	void Execute();

	void GameExecute();

	void UnclampedVelocity();

	static SDL_Renderer* GetRenderer() { return m_renderer; }

	static void ToggleDebug() {	m_debug = !m_debug;	}

	static void Play();

	static void ReturnToMenu();

	static void Quit() { m_main_menu = false; }

	static bool DebugMode() { return m_debug; }

	static void ChangeCanvasState(CurrentUI state) { m_camera.SetCanvasState(state); }
	static CurrentUI GetCanvasState() { return m_camera.GetCanvasState(); }

private:

	static GameState m_game_state;

	static bool m_main_menu;

	static bool m_menu_first_frame;

	static bool m_run;
	static bool m_debug;

	static SDL_Window* m_window;
	static SDL_Renderer* m_renderer;

	bool m_fullscreen{ false };

	static Camera m_camera;


	Player* m_player_cast;
	vector<GameObject*> m_gameobjects;

	const char* WINDOW_NAME = "Gobbie the Gobbo";
	 int DEFAULT_WINDOW_WIDTH = 960;
	const int DEFAULT_WINDOW_HEIGHT = 590;

	void Setup();

	void AppLoop();

	void MainMenuExecution();

	void GameSetup();

	void FrameInit();

	void GameLoop();

	void FrameCleanup();

	void InputHandler();

	void EventHandler();

	void RendererHandler();

	void GameClose();

	void Close();


	Vector2 velocity{ 100,100 };
	Vector2 old_velocity{ 0,0 };
	Vector2 frame_movement{ 10,10 };
	Vector2 decrease_rate{ 2,2 };
};