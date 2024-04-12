#pragma once

#include <vector>
#include <functional>

class RectangleCollider;
class CircleCollider;

/// <summary>
/// The 'Collider' class is the abstract base class the other collider classes inherit from, it 
/// is not to be used directly however holds some of the primary functionality vital in the actual colliders.
/// </summary>
class Collider
{
public:
	/// <summary>
	/// The shape of the collider. (RECT = Rectangle, CIRCLE = Circle)
	/// </summary>
	enum ColliderShape { RECT, CIRCLE };

	/// <summary>
	/// The constructor for the base collider class.
	/// </summary>
	/// <param name="x">The position on the X axis.</param>
	/// <param name="y">The position on the Y axis.</param>
	/// <param name="shape">The shape of the collider.</param>
	/// <param name="collisionEvent">The function called when a collision is detected with this collider.</param>
	Collider(float x, float y, ColliderShape shape, std::function<void()> collisionEvent);
	/// <summary>
	/// The destructor for the base collider class.
	/// </summary>
	~Collider();

	/// <summary>
	/// Sets the function to be executed on a collision detection from this collider.
	/// </summary>
	/// <param name="collisionEvent">The function called when a collision is detected with this collider.</param>
	void SetCollisionEvent(std::function<void()> collisionEvent);
	/// <summary>
	/// Sets the new position on the X axis.
	/// </summary>
	/// <param name="x">The new position on the X axis.</param>
	void SetX(float x);
	/// <summary>
	/// Sets the new position on the Y axis.
	/// </summary>
	/// <param name="x">The new position on the Y axis.</param>
	void SetY(float y);

	/// <summary>
	/// Gets the current X position.
	/// </summary>
	/// <returns>The colliders X position.</returns>
	float GetX();
	/// <summary>
	/// Gets the current Y position.
	/// </summary>
	/// <returns>The colliders Y position.</returns>
	float GetY();
	/// <summary>
	/// Gets the collider shape.
	/// </summary>
	/// <returns>The colliders shape.</returns>
	ColliderShape GetShape();

	/// <summary>
	/// The function that checks all other colliders against this collider. USED INTERNALLY IN THE COLLISION SYSTEM.
	/// </summary>
	/// <param name="colliders">All the other registered colliders.</param>
	void CheckCollision(std::vector<Collider*> colliders);

protected:
	/// <summary>
	/// Checks this collider against another collider dependant on shape. USED INTERNALLY IN THE COLLISION SYSTEM.
	/// </summary>
	/// <param name="collider">The other collider being checked against.</param>
	/// <returns>Whether a collision has been identified or not.</returns>
	bool IsCollided(Collider* collider);
	/// <summary>
	/// The function that checks if there's a collision event function and executes it if it exists. USED INTERNALLY IN THE COLLISION SYSTEM.
	/// </summary>
	void OnCollision();

	/// <summary>
	/// The method of checking this collider against an AABB collider rectangle.
	/// </summary>
	/// <param name="collider">The other collider being checked against.</param>
	/// <returns>Whether a collision has been identifed or not.</returns>
	virtual bool RectangleCheck(RectangleCollider* collider) = 0;
	/// <summary>
	/// The method of checking this collider against a circle collider.
	/// </summary>
	/// <param name="collider">The other collider being checked against.</param>
	/// <returns>Whether a collision has been identifed or not.</returns>
	virtual bool CircleCheck(CircleCollider* collider) = 0;

	/// <summary>
	/// The position of the collider { x, y }.
	/// </summary>
	float m_position[2] = { 0,0 };
	/// <summary>
	/// The shape of the collider.
	/// </summary>
	ColliderShape m_shape;
	/// <summary>
	/// The function ran when a collision is detected.
	/// </summary>
	std::function<void()> m_collision_event;

};