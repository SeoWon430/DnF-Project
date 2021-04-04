#include "stdafx.h"
#include "CylinderCollider.h"

CylinderCollider::CylinderCollider(float radius, float height)
	: Collider()
	,radius(radius)
	,height(height)
{
}

CylinderCollider::~CylinderCollider()
{
}

void CylinderCollider::World(Matrix & world)
{
	this->world = world;
	this->position.x = world._41;
	this->position.y = world._42;
	this->position.z = world._43;
}

bool CylinderCollider::IsInRange(Vector3 position, Vector3 scale)
{
	bool result = false;


	float x = -this->position.x;
	float y = -this->position.y;
	float b = (float)radius * Baxis;

	if (this->position.x < position.x) 
		x += (position.x - scale.x);
	else
		x += (position.x + scale.x);

	if (this->position.y < position.y)
		y += position.z;
	else
		y += position.z + scale.z;

	// 타원 방정식
	if ((x*x) / (radius*radius) + (y*y) / (b*b) <= 1)
		result = true;




	return result;
}
