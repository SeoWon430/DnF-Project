#pragma once

#define Baxis 0.3f

class CylinderCollider :public Collider
{
public:
	CylinderCollider(float radius, float height);
	~CylinderCollider();

	void World(Matrix & world) override;
	bool IsInRange(Vector3 position, Vector3 scale);

	Vector3 Position() { return position; }
	float Height() { return height; }
	float Radius() { return radius; }

private:
	Vector3 position = Vector3();
	float radius = 0;
	float height = 0;

};