#pragma once

#include "RectangleCollider.h"
#include "CircleCollider.h"

#include <vector>

/// <summary>
/// The 'CollisionHandler' class is a static class that handles the main functionality 
/// of the collision system, acting as the main foundation of the implementation.
/// </summary>
class CollisionHandler
{
public:
	/// <summary>
	/// Runs the frame check required to detect and respond to collisions.	
	/// </summary>
	static void Update();
	/// <summary>
	/// Deletes all the colliders in the collision system.
	/// </summary>
	static void Cleanup();

	/// <summary>
	/// Adds the collider into the collision system. Automatically called on creation of a collider.
	/// </summary>
	/// <param name="collider">The collider to be added.</param>
	static void Add(Collider* collider);
	/// <summary>
	/// Removes the collider from the collision system. Automatically called on deletion of a collider.
	/// </summary>
	/// <param name="collider">The collider to be removed.</param>
	static void Remove(Collider* collider);

private:
	/// <summary>
	/// Holds every registered collider in the collision system.
	/// </summary>
	static std::vector<Collider*> m_colliders;

};