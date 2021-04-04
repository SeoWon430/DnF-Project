#pragma once

enum class Condition
{
	None,
	Jump,
	BackAttack,
	NonBackAttack
};

enum class ActionPosition
{
	None,
	Player,
	Camera
};

class Action
{
public:
	Action(UINT motion, Condition condition = Condition::None, ArmorState armorState = ArmorState::Normal);
	virtual ~Action();

	virtual void Start(Vector3 position, Vector3 scale, Vector3 rotation, float speed);
	void Update(Matrix V, Matrix P);
	void Render();

	UINT Motion() { return motion; }
	ArmorState GetArmorState() { return armorState; }

	void AddEffect(Clip * effect, float delay, Vector3 offset, float damage = -1.0f);

	bool IsStart() { return isStart; }
	Condition Condition_() { return condition; }

	vector<Effect*> AttackEffects() { return effects; }

	bool IsAttack();

	void Position(Vector3& vec) { position = vec; }
	Vector3 Move() { return move * Time::Delta(); }
	void Move(Vector3 move) { this->move = move; }
	void IsPlayerFollow(bool is) { isPlayerFollow = is; }
	bool IsPlayerFollow() { return isPlayerFollow; }
	void Scale(Vector3& vec);
	void Speed(float sp);
	void ColliderTypeChange(UINT index, ColliderType type);

private:
	bool isStart = false;
	bool isPlayerFollow = true;
	bool isScaled = false;
	bool isSetSpeed = false;

	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 rotation = Vector3(0, 0, 0);
	Vector3 move = Vector3();
	float speed;

	Condition condition;

	UINT motion;
	vector<Effect*> effects;
	//vector<EffectAttack*> attackEffects;
	float timer = 0;

	ArmorState armorState;
};