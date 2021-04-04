#pragma once



class BoxCollider : public Collider
{
public:
	BoxCollider(CenterPoint mode = CenterPoint::Center, ColliderType type = ColliderType::Stand, ColliderTag tag= ColliderTag::None);
	~BoxCollider();


	void World(Matrix & world) override;

	CenterPoint CenterMode() { return centerMode; }
	ColliderType Type() { return type; }
	void Type(ColliderType type) { this->type = type; }

	void CreateBuffer() override;


private:
	ColliderType type = ColliderType::Stand;
	CenterPoint centerMode;
};