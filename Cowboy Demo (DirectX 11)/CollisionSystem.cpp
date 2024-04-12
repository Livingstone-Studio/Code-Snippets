#include "CollisionSystem.h"

#include "../../GameObjects/Collider.h"

CollisionSystem* CollisionSystem::Instance = nullptr;

CollisionSystem::CollisionSystem()
{
	if (!Instance)
		Instance = this;
}

CollisionSystem::~CollisionSystem()
{
	m_colliders.clear();
}

void CollisionSystem::CheckCollisions()
{
	for (Collider* collider : m_colliders)
		if (collider)
			collider->CheckCollisions(m_colliders);
}

void CollisionSystem::RemoveCollider(Collider* collider)
{
	m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
}