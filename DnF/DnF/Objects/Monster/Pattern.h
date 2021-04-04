#pragma once


class Pattern
{
public:
	Pattern();
	virtual ~Pattern();

	virtual void Update(Matrix & V, Matrix & P);
	virtual void Render();

	void Initialize(MonsterState* state);

	virtual void SkillStart(float speed = 1.0f);
	virtual void ActionStart();
	virtual void ActionNextCheck();
	virtual void ActionEnd();
	virtual void ActionNext();

	virtual void AttackAction();
	virtual void AttackAction(GameObject* target);

	void Attack(GameObject* target, UINT atk, Vector3& playerPosition, function<void(float)> attack);


	MonsterMotion CurrentMotion();
	MonsterState CurrentState() { return skillState; }
	float Speed() { return speed; }

	float CoolTime() { return coolTime; }
	float ResetTime() { return resetTime; }

	bool IsAction() { return isAction; }
	bool IsEnd() { return !isStart; }
	bool IsActive() { return isActive; }
	bool IsEnable();

	int Key() { return key; }
	void Key(int value) { key = value; }

	void Position(float x, float y);
	void Position(Vector2& vec);
	void Position(Vector3& vec);
	Vector3 Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& vec);
	void Scale(Vector3& vec);
	Vector3 Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& vec);
	Vector3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& vec);
	Vector3 RotationDegree();

	Vector3 Move() { return actions[currentAction]->Move(); }

	Vector3 MoveTo();
	Direction DirectionTo();

	float Delay() { return delay; }

protected:
	void CreateAction(MonsterMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile);


	virtual void SkillEnd();


protected:
	float delay = 1.0f;
	int key = 0;
	bool isActive = true;		// 사용 가능여부

	bool isAction = false;		// 실행 상태
	bool isStart = false;

	float coolTime = 0.0f;		// 재사용 대기시간
	float resetTime = 0.0f;	// cooltime 시간 체크
	float speed = 1.0f;

	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 rotation = Vector3();

	Vector3 moveTo = Vector3(0, 0, 0);

	MonsterState skillState;		// 스킬 상태
	MonsterState* currentState;	// 현재 플레이어가 가진 상태
	vector<MonsterState> enableStates;

	vector<class Action*> actions;
	UINT nextAction = 0;
	UINT currentAction = 0;

	bool isAttack = false;		// 현재 공격이 타격 됐는지
	bool isBackAttack = false;	// 현재 공격이 백어택인지
	Direction direction = Direction::None;
};

