#include "CollisionHandler.h"

#include <iostream>

std::vector<Collider*> CollisionHandler::m_colliders;

void CollisionHandler::Update()
{
	for (Collider* collider : m_colliders) 
	{
		if (collider)
		{
			collider->CheckCollision(m_colliders);
		}
	}
}

void CollisionHandler::Cleanup()
{
	while (m_colliders.size() > 0)
	{
		delete m_colliders[0];
	}

	m_colliders.clear();
}

void CollisionHandler::Add(Collider* collider)
{
	m_colliders.push_back(collider);
}

void CollisionHandler::Remove(Collider* collider)
{
	m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
}