#include "stdafx.h"
#include "AA00_Attack.h"



AA00_Attack::AA00_Attack(wstring shaderFile, wstring spriteFile)
	:Skill_Active(L"평타", L"")
{
	enableStates = {
		PlayerState::Idle,
		PlayerState::Walk,
		PlayerState::Run,
		PlayerState::Jump,
		PlayerState::Attack
	};
	//motions = {
	//	MotionAttack(PlayerMotion::Attack1, 1.0f),
	//	MotionAttack(PlayerMotion::Attack2, 1.2f),
	//	MotionAttack(PlayerMotion::Attack3, 1.4f),
	//	MotionAttack(PlayerMotion::Attack4, 1.6f),
	//	MotionAttack(PlayerMotion::JumpAttack, 2.0f, Condition::Jump),
	//};

	vector<UINT> idx = { 0, 5, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 1.0f };
	vector<float> delay = { 0.0f };
	vector<Vector3> offset = { Vector3(48, 10, 0) };
	CreateAction(PlayerMotion::Attack1, damage, delay, idx, offset, shaderFile, spriteFile);

	idx = { 5, 9, 0 };
	damage = { 1.2f };
	CreateAction(PlayerMotion::Attack2, damage, delay, idx, offset, shaderFile, spriteFile);

	idx = { 9, 13, 0 };
	damage = { 1.4f };
	CreateAction(PlayerMotion::Attack3, damage, delay, idx, offset, shaderFile, spriteFile);

	idx = { 13, 17, 0 };
	damage = { 1.6f };
	CreateAction(PlayerMotion::Attack4, damage, delay, idx, offset, shaderFile, spriteFile);

	idx = { 0, 5, 0 };
	damage = { 2.0f };
	CreateAction(PlayerMotion::JumpAttack, damage, delay, idx, offset, shaderFile, spriteFile, Condition::Jump);
	




	Scale(Vector3(1.5f, 1.5f, 1.0f));


	key = 'X';
	type = Type::Chain;
	skillState = PlayerState::Attack;
}

AA00_Attack::~AA00_Attack()
{
}

void AA00_Attack::Update(Matrix & V, Matrix & P)
{
	__super::Update(V, P);

	if(currentState == PlayerState::Idle)
		jumpAttack = 0;
}

void AA00_Attack::Render()
{
	__super::Render();
}

void AA00_Attack::SkillStart(float speed)
{
	// 대시 공격
	if (currentState == PlayerState::Run)
	{
		currentAction = 3;
	}
	// 점프 공격
	else if (currentState == PlayerState::Jump)
	{
		if (jumpAttack >= jumpAttackLimit)
			return;
		jumpAttack++;
		currentAction = 4;
	}
	__super::SkillStart(speed);
}

