#pragma once

#include "Collider.h"

/// <summary>
/// The 'RectangleCollider' class is the class used for rectangle colliders
/// using an Axis-Aligned Bounding Box (AABB) approach.
/// </summary>
class RectangleCollider : public Collider
{
public:
	/// <summary>
	/// The constructor for the rectangle collider.
	/// </summary>
	/// <param name="x">The position on the X axis.</param>
	/// <param name="y">The position on the Y axis.</param>
	/// <param name="w">The width of the rectangle.</param>
	/// <param name="h">The height of the rectangle.</param>
	/// <param name="collisionEvent">The function called when a collision is detected with this collider.</param>
	RectangleCollider(float x, float y, float w, float h, std::function<void()> collisionEvent);
	/// <summary>
	/// The destructor for the rectangle collider.
	/// </summary>
	~RectangleCollider();

	/// <summary>
	/// Sets a new width size for the rectangle collider.
	/// </summary>
	/// <param name="w">The new width for the rectangle.</param>
	void SetW(float w);
	/// <summary>
	/// Sets a new height size for the rectangle collider.
	/// </summary>
	/// <param name="h">The new height for the rectangle.</param>
	void SetH(float h);

	/// <summary>
	/// Gets the current width.
	/// </summary>
	/// <returns>The colliders width.</returns>
	float GetW();
	/// <summary>
	/// Gets the current height.
	/// </summary>
	/// <returns>The colliders height.</returns>
	float GetH();

protected:
	/// <summary>
	/// The method of checking this collider against another rectangle collider.
	/// </summary>
	/// <param name="collider">The other rectangle collider being checked against.</param>
	/// <returns>Whether a collision has been identifed or not.</returns>
	virtual bool RectangleCheck(RectangleCollider* collider) override;
	/// <summary>
	/// The method of checking this collider against a circle collider.
	/// </summary>
	/// <param name="collider">The circle collider being checked against.</param>
	/// <returns>Whether a collision has been identifed or not.</returns>
	virtual bool CircleCheck(CircleCollider* collider) override;

	/// <summary>
	/// The size of the collider { w, h }.
	/// </summary>
	float m_size[2];

};

