#pragma once

#include "GameObject.h"
#include "Input.h"

#include "Canvas.h"

#include <vector>
#include <algorithm>

class GameObject;

class Camera
{
public:

	Camera();

	~Camera();

	void SetCameraSize(Vector2 size) { m_transform.scale = size; }
	void SetCameraPosition(Vector2 pos) { m_transform.position = pos; }

	Transform ConvertedToScreenSpace(Transform t);
	Transform ConvertToUISpace(AnchorPosition aP, Transform t);

	void Follow(GameObject* target, float deltaTime);

	void Start();

	void Update( Vector2 defaultCamSize, Vector2 currentCamSize);

	void RenderStart(SDL_Renderer* renderer, vector<GameObject*> gameObjects);
	void RenderUpdate(SDL_Renderer* renderer, vector<GameObject*> gameObjects);
	void RenderUpdateUI(SDL_Renderer* renderer, Camera camera);

	void RenderDebug(SDL_Renderer* renderer, vector<GameObject*> gameObjects);

	Transform GetTransform() { return m_transform; }

	Vector2 GetCurrentWindow() { return m_window_size; }

	Vector2 GetWindowNormalized() { return m_window_size / m_default_window_size; }

	void InitializePlayerHUD(Player* player) { canvas.InitializePlayerHUD(player); }

	void SetCanvasState(CurrentUI state) { canvas.ChangeState(state); }
	CurrentUI GetCanvasState() { return canvas.GetState(); }

	void SetScale(float s) { m_scale = s; }

private:

	Canvas canvas;

	vector<GameObject*> SortRenderOrder(vector<GameObject*> gameObjects);

	
	void SetClampedSpeed(float s);

	// Rendering

	Transform m_transform{ 0, 0, 5, 5 };

	Vector2 m_units_on_screen{ 10, 5 };
	float m_scale{ 1.0f };
	
	Vector2 m_default_window_size{ 0, 0 };
	Vector2 m_window_size{ 0, 0 };

	// Follow

	float m_follow_speed{ 0.0f };
	float m_min_clamped_speed{ 1.0f };
	float m_max_clamped_speed{ 5.0f };
	float m_distance_before_follow{ 0.1f };

};

