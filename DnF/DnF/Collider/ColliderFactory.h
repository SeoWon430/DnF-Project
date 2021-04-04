#pragma once

class ColliderFactory
{
public:
	ColliderFactory();
	~ColliderFactory();

	static void Create();
	static void Delete();

	static ColliderFactory* Get();

	string CreateCollider(string name, Collider* collider);

	Vector3 InRange(Vector3 center, float range, ColliderTag tag);

	bool Collision(string a, string b);

	bool Collision(BoxCollider* collider, Vector3 target);
	bool Collision(CylinderCollider* collider, Vector3 target);
	bool Collision(BoxCollider* collider1, CylinderCollider* collider2);
	bool Collision(BoxCollider* collider1, BoxCollider* collider2);

	static bool Aabb(const Matrix& world, const Vector2& position);
	static bool Aabb(const Matrix& world1, const Matrix& world2, CenterPoint m1 = CenterPoint::Center, CenterPoint m2 = CenterPoint::Center);
	//static bool AabbEX(float gapY, Matrix& world1, Matrix& world2, CenterPoint m1 = CenterPoint::Center, CenterPoint m2 = CenterPoint::Center);

	static bool AabbEX(float groundGap, float jumpGap, BoxCollider* collider1, BoxCollider* collider2);
	static bool AabbEX(Vector3 pos1, Vector3 pos2, Vector3 scale1, Vector3 scale2, Collider* collider1, Collider* collider2);


	static bool Obb(Sprite* a, Sprite* b);


private:

	struct Vertex
	{
		Vector3 Position;
	};

	struct ObbDesc {
		Vector2 HalfSize;

		Vector2 Right;	// 원본 방향 Right벡터
		Vector2 Up;		// 원본 방향 Up벡터

		Vector2 Length_Right;	// 크기가 고려된 Right벡터
		Vector2 Length_Up;		// 크기가 고려된 Up벡터
	};

	static void CreateObb(ObbDesc* out, Vector2& half, Matrix& transform);
	static float SeprateAxis(Vector2& init, Vector2& e1, Vector2& e2);

private:
	static ColliderFactory* instance;

	vector<Collider*> colliders2;
	unordered_map<string, Collider*> colliders;


};