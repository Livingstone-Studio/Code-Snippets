#pragma once

#include "Collider.h"

/// <summary>
/// The 'CircleCollider' class is the class used circle colliders.
/// </summary>
class CircleCollider : public Collider
{
public:
	/// <summary>
	/// The constructor for the circle collider.
	/// </summary>
	/// <param name="x">The position on the X axis.</param>
	/// <param name="y">The position on the Y axis.</param>
	/// <param name="r">The radius of the circle.</param>
	/// <param name="collisionEvent">The function called when a collision is detected with this collider.</param>
	CircleCollider(float x, float y, float r, std::function<void()> collisionEvent);
	/// <summary>
	/// The destructor for the circle collider.
	/// </summary>
	~CircleCollider();

	/// <summary>
	/// Sets a new radius size for the circle collider.
	/// </summary>
	/// <param name="r">The new radius for the circle.</param>
	void SetRadius(float r);
	/// <summary>
	/// Gets the current radius.
	/// </summary>
	/// <returns>The colliders radius.</returns>
	float GetRadius();

protected:
	/// <summary>
	/// The method of checking this collider against a rectangle collider.
	/// </summary>
	/// <param name="collider">The rectangle collider being checked against.</param>
	/// <returns>Whether a collision has been identifed or not.</returns>
	virtual bool RectangleCheck(RectangleCollider* collider) override;
	/// <summary>
	/// The method of checking this collider against another circle collider.
	/// </summary>
	/// <param name="collider">The other circle collider being checked against.</param>
	/// <returns>Whether a collision has been identifed or not.</returns>
	virtual bool CircleCheck(CircleCollider* collider) override;

	/// <summary>
	/// Finds the magnitude of x and y.
	/// </summary>
	/// <param name="x">The x value of the equation.</param>
	/// <param name="y">The y value of the equation.</param>
	/// <returns>The magnitude of x and y.</returns>
	float GetMagnitude(float x, float y);

	/// <summary>
	/// The current radius of the circle.
	/// </summary>
	float m_radius;

};

