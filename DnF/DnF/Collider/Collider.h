#pragma once


enum class ColliderType {
	Flat = 0,	// 바닥과 평행한 충돌 (ex 바닥 충격파)
	Stand,		// 바닥과 수직한 충돌 (ex 세로로 쏘는 검기)
};

enum class ColliderTag
{
	None,
	Player,
	PlayerEffect,
	Monster,
	MonsyerEffect

};


class Collider
{
public:
	Collider(ColliderTag tag = ColliderTag::None);
	virtual ~Collider();

	void Update(Matrix& V, Matrix& P);
	void Render();

	virtual void World(Matrix& world);
	Matrix GetWorld() { return world; }
	bool IsActive() { return isActive; }

	Vector3 Position();
	void Tag(ColliderTag t) { tag = t; }
	ColliderTag Tag() { return tag; }

	void DrawCollisionCollider(bool val) { isCollisionColor = val; }


protected:

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

	virtual void CreateBuffer();


protected:
	ColliderTag tag = ColliderTag::None;
	Matrix world;
	bool isActive = false;
	bool isCollisionColor;

	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	
	ID3DX11EffectMatrixVariable* sWorld;
	ID3DX11EffectMatrixVariable* sView;
	ID3DX11EffectMatrixVariable* sProjection;

	ID3DX11EffectVectorVariable* sColor;

};