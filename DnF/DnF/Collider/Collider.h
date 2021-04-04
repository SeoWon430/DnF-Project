#pragma once


enum class ColliderType {
	Flat = 0,	// �ٴڰ� ������ �浹 (ex �ٴ� �����)
	Stand,		// �ٴڰ� ������ �浹 (ex ���η� ��� �˱�)
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

		Vector2 Right;	// ���� ���� Right����
		Vector2 Up;		// ���� ���� Up����

		Vector2 Length_Right;	// ũ�Ⱑ ����� Right����
		Vector2 Length_Up;		// ũ�Ⱑ ����� Up����
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