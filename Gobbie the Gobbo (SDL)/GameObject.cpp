#include "GameObject.h"

GameObject::GameObject()
	: m_transform(), m_collider(true, false)
{
}

GameObject::GameObject(Vector2 position)
	: m_transform( position ), m_collider(true, false)
{
}

GameObject::GameObject(Transform transform)
	: m_transform( transform ), m_collider(true, false)
{
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
	m_sort_order = m_default_sort - m_transform.position.y;

	m_initialized = true; 
}

void GameObject::Update(float deltaTime)
{
	if (m_initialized && !m_deletion) m_gfx.Update();

	if (m_dynamic_sort_order) m_sort_order = m_default_sort - m_transform.position.y;

}

void GameObject::RenderStart(SDL_Renderer* renderer, Camera camera)
{
	m_collider_debug.Initialize(renderer, "Debug", { 0, 1, 0.0f, 0, false });
	m_render_initialized = true;
}

void GameObject::RenderUpdate(SDL_Renderer* renderer, Camera camera)
{
	if (m_render_initialized && !m_deletion)
	{
		m_gfx.RenderUpdate(renderer, camera.ConvertedToScreenSpace(AnimOffsetPosition()));
	}
}

void GameObject::RenderDebug(SDL_Renderer* renderer, Camera camera)
{
	if (m_render_initialized && !m_deletion && m_collider.GetActive())
	{
		m_collider_debug.SetAnimation({0, 1, 0.0f, 0, false});

		Vector2 offset = m_collider.GetCollisionOffset();

		if (m_collider.GetActive()) m_collider_debug.RenderUpdate(renderer, camera.ConvertedToScreenSpace({ m_transform.position + offset, m_transform.scale * m_collider_scale * m_debug_box_scale }));
	}
}

void GameObject::CollisionCheck(vector<GameObject*> gameObjects)
{
	m_frame_collided.clear();

	if (!m_collider.GetActive())
	{
		return;
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i] != this && gameObjects[i]->GetCollider().GetActive())
		{
			Collided c = { gameObjects[i], m_collider.CollisionCheck(
				m_collider.IsTrigger(),
			  {
				m_transform.position.x + m_collider.GetCollisionOffset().x,
				m_transform.position.y + m_collider.GetCollisionOffset().y,
				m_transform.scale.x * m_collider_scale.x,
				m_transform.scale.y * m_collider_scale.y
			  },
			  { gameObjects[i]->GetTransform().position.x + gameObjects[i]->GetCollider().GetCollisionOffset().x,
				gameObjects[i]->GetTransform().position.y + gameObjects[i]->GetCollider().GetCollisionOffset().y,
				gameObjects[i]->GetTransform().scale.x * gameObjects[i]->GetColliderScale().x,
				gameObjects[i]->GetTransform().scale.y * gameObjects[i]->GetColliderScale().y 
			  })
			};

			if (c.info.collided)
			{
				m_frame_collided.push_back(c);
			}
		}
	}
}

void GameObject::CollisionResponse()
{
}

Transform GameObject::AnimOffsetPosition()
{
	Transform t = m_transform;
	Vector2 o = m_gfx.GetOffset();

	t.position += o;

	return t;
}
