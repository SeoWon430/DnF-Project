#pragma once

class Effect
{
public:
	Effect(Clip* effect, float delay, Vector3 offset, float damage);
	~Effect();

	void Update(Matrix & V, Matrix & P);
	void Render();

	bool IsStart() { return isStart; }
	float Delay() { return delay; }
	float Damage() { return damage; }
	void Start(float speed);


	void Position(Vector3& position);
	Vector3 Position();
	void Scale(Vector3& scale);
	Vector3 Scale();
	void Rotation(Vector3& rotation);
	Vector3 Rotation();



	bool IsCollision(GameObject * target, Vector3 & playerPosition, bool& isAttack, bool& isBackAttack);
	void ColliderTypeChange(ColliderType type);

private:
	bool isStart = false;

	Clip* effect;
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0, 0, 0);
	Vector3 rotation = Vector3(0, 0, 0);

	float delay;
	float damage = 0.0f;
	Vector3 offset;

	vector<GameObject*> targets;
	Collider* collider = NULL;


};