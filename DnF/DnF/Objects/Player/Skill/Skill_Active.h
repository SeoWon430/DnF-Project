#pragma once
#include "Skill.h"

enum class Type
{
	None,
	Chain,
	Loop
};

struct SkillData
{
	PlayerMotion Motion;	// ��ų ��� : �÷��̾� ��� Ŭ��
	PlayerState State;		// ��ų ���� : �÷��̾� ����
	float Speed;			// ��� �ӵ� : �÷��̾� ��� ��� �ӵ�
	Vector3 Move;			// ��ų ������ : ��ų�� ���� ��� �̵�
	ArmorState ArmorState_;
};

struct SkillMoveTo
{
	Vector3 MoveTo;			// ��ų�� ���� �̵�(�����̵�)
	Direction Direction_;	// �̵� �� �� ����
};



class Skill_Active : public Skill
{
public:
  	Skill_Active(wstring name, wstring iconFile);
  	virtual ~Skill_Active();
  
  	virtual void Update(Matrix & V, Matrix & P);
  	virtual void Render();
  

	virtual void SkillStart(float speed = 1.0f);
	virtual void ActionStart();
	virtual void ActionNextCheck();
	virtual void ActionEnd();
	virtual void ActionNext();

	virtual void AttackAction();
	virtual void AttackAction(GameObject* target);

	void Attack(GameObject* target, UINT atk, Vector3& playerPosition, function<void(float)> attack);


	SkillData CurrentSkillData();
	SkillMoveTo CurrentSkillMoveTo();
	void CurrentState(PlayerState state) {currentState = state; }

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




protected:
	void CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile);
	void CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile, ArmorState armorState);
	void CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile, Condition condition);
	void CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile, Condition condition, ArmorState armorState);
	

	virtual void SkillEnd();
  
  
protected:
	Type type = Type::None;
	int key = 0;
	bool isActive = true;		// ��� ���ɿ���

	bool isAction = false;		// ���� ����
	bool isStart = false;

	float coolTime = 0.0f;		// ���� ���ð�
	float resetTime = 0.0f;	// cooltime �ð� üũ
	float speed = 1.0f;

	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 rotation = Vector3();

	Vector3 moveTo = Vector3(0, 0, 0);

	PlayerState skillState;		// ��ų ����
	PlayerState currentState;	// ���� �÷��̾ ���� ����
	vector<PlayerState> enableStates;
	
	vector<class Action*> actions;
	UINT currentAction = 0;
	UINT nextAction = 0;

	bool isAttack = false;		// ���� ������ Ÿ�� �ƴ���
	bool isBackAttack = false;	// ���� ������ ���������
	Direction direction = Direction::None;
};
