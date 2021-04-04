#pragma once
#include "Viewer/IFollowing.h"


enum class MonsterMotion
{
	Start = 0,
	Idle,		//1
	Walk,		//2
	Claw,		//3
	Thorn1,		//4
	Thorn2,		//5
	Hit,		//6
	Jump1,		//7
	Jump2,		//8
	Die,		//9
	None
};
enum class MonsterState 
{
	Start = 0,
	Idle,
	Walk,
	Claw,
	Thorn
};



class Monster : public GameObject, IFollowing
{
public:
	Monster(wstring name);
	virtual ~Monster();


	virtual void Update(Matrix& V, Matrix &P);
	virtual void Render();


	void MoveSpeed(float val) { moveSpeed = val; }

	void Focus(Vector2 * position, Vector2 * size) override;

	void SetState(MonsterState state) { this->state = state; }
	
	void SetTarget(GameObject* t) { target = t; };
	Matrix World() { return animation->GetSprite()->World(); }

	void PatternAttack(GameObject* target);

private:
	virtual void CreateAvata() = 0;
	virtual void CreateSkill() = 0;


	void Move();
	void PatternUpdate();

protected:
	MonsterMotion motion = MonsterMotion::Start;
	MonsterState state = MonsterState::Start;

	//class SkillSet* skillset;
	GameObject* target = NULL;
	class PatternSet* patternSet = NULL;

	wstring name;

	float moveSpeed;		// 이동속도
	float attackSpeed;		// 공격속도
	
	UINT atk;				// 공격력

	bool isGround;
	bool isAttack;
	float groundY;

	float patternDelay = 1.0f;
	float turnDelay = 1.0f;
};