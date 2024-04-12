#include "RectangleCollider.h"
#include "CircleCollider.h"

#include <iostream>

RectangleCollider::RectangleCollider(float x, float y, float w, float h, std::function<void()> collisionEvent)
    : Collider(x,y,RECT,collisionEvent),m_size{ w,h }
{
}

RectangleCollider::~RectangleCollider()
{
}

void RectangleCollider::SetW(float w)
{
    m_size[0] = w;
}

void RectangleCollider::SetH(float h)
{
    m_size[1] = h;
}

float RectangleCollider::GetW()
{
    return m_size[0];
}

float RectangleCollider::GetH()
{
    return m_size[1];
}

bool RectangleCollider::RectangleCheck(RectangleCollider* collider)
{
    if (!collider) return false;

	float o_pos[2] = { collider->GetX(), collider->GetY() };
	float o_size[2] = { collider->GetW()/2, collider->GetH()/2 };

	float m_c1_min[2] = { m_position[0] - m_size[0]/2, m_position[1] - m_size[1]/2 };
	float m_c1_max[2] = { m_position[0] + m_size[0]/2, m_position[1] + m_size[1]/2 };

	float m_c2_min[2] = { o_pos[0] - o_size[0], o_pos[1] - o_size[1] };
	float m_c2_max[2] = { o_pos[0] + o_size[0], o_pos[1] + o_size[1] };


	if (m_c1_min[0] > m_c2_max[0] || m_c1_max[0] < m_c2_min[0]) return false;
	if (m_c1_min[1] > m_c2_max[1] || m_c1_max[1] < m_c2_min[1]) return false;

	return true;
}

bool RectangleCollider::CircleCheck(CircleCollider* collider)
{
    if (!collider) return false;

    float o_pos[2] = { collider->GetX(), collider->GetY() };

    float axis_distance[2] = { abs(o_pos[0] - m_position[0]),abs(o_pos[1] - m_position[1]) };

    float corner_distance_sq = pow(axis_distance[0] - m_size[0] / 2, 2) + pow(axis_distance[1] - m_size[1] / 2, 2);
    if (corner_distance_sq <= pow(collider->GetRadius(), 2)) { return true; }

    if (axis_distance[0] > (m_size[0] / 2 + collider->GetRadius())) { return false; }
    if (axis_distance[1] > (m_size[1] / 2 + collider->GetRadius())) { return false; }

    if (axis_distance[0] <= (m_size[0] / 2)) { return true; }
    if (axis_distance[1] <= (m_size[1] / 2)) { return true; }

    return false;
}
