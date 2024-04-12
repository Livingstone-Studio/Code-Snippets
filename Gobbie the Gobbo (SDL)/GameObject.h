#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Animation.h"
#include "Camera.h"
#include "Structs.h"
#include "Collider.h"
#include "AudioManager.h"

class Sprite;
class AnimClip;
class Animation;
class Camera;
class Character;

class GameObject
{

public:

	GameObject();
	GameObject(Vector2 position);
	GameObject(Transform transform);

	~GameObject();

	void SetX(float x) { m_transform.position.x = x; }
	void SetY(float y) { m_transform.position.y = y; }
	void SetScaleX(float x) { m_transform.scale.x = x; }
	void SetScaleY(float y) { m_transform.scale.y = y; }

	Transform GetTransform() { return m_transform; }

	float GetX() { return m_transform.position.x; }
	float GetY() { return m_transform.position.y; }
	float GetScaleX() { return m_transform.scale.x; }
	float GetScaleY() { return m_transform.scale.y; }

	virtual void Start();

	virtual void Update(float deltaTime);

	virtual void RenderStart(SDL_Renderer* renderer, Camera camera);

	virtual void RenderUpdate(SDL_Renderer* renderer, Camera camera);

	virtual void RenderDebug(SDL_Renderer* renderer, Camera camera);


	bool IsInitialized() { return m_initialized; }
	bool IsRenderInitialized() { return m_render_initialized; }

	void Delete() { m_deletion = true; }
	bool MarkedForDeletion() { return m_deletion; }

	float m_sort_order{ 0 };

	virtual void CollisionCheck(vector<GameObject*> gameObjects);
	virtual void CollisionResponse();

	virtual Vector2 GetColliderScale() { return m_collider_scale; }
	virtual Collider GetCollider() { return m_collider; }

	virtual string GetAnimName() { return m_gfx.GetAnimName(); }

protected:

	Transform AnimOffsetPosition();

	Collider m_collider;
	Vector2 m_collider_scale{ 1,1 };

	AnimClip m_collider_debug;

	vector<Collided> m_frame_collided;

	bool m_dynamic_sort_order{ true };

	float m_default_sort{ 0 };

	bool m_initialized{ false };
	bool m_render_initialized{ false };

	bool m_deletion{ false };

	float m_debug_box_scale{ 3.75f };

	Transform m_transform;
	Animation m_gfx;

	string m_image_name{ "Orc" };
};

