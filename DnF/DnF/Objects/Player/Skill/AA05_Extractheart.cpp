#include "stdafx.h"
#include "AA05_Extractheart.h"

AA05_Extractheart::AA05_Extractheart(wstring shaderFile, wstring spriteFile)
	:Skill_Active(L"암살", spriteFile + L"/extractheart.png")
{
	enableStates = {
		PlayerState::Idle,
		PlayerState::Walk,
		PlayerState::Run,
		PlayerState::Attack
	};

	vector<UINT> idx = { 0, 5, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 5.0f };
	vector<float> delay = { 0.0f };
	vector<Vector3> offset = { Vector3(20, 150, 0) };
	CreateAction(PlayerMotion::Extract, damage, delay, idx, offset, shaderFile, spriteFile, ArmorState::SuperArmor);
	

	idx = { 5, 9, 0 };
	damage = { 5.0f };
	offset = { Vector3(0, 200, 0) };
	CreateAction(PlayerMotion::Invisible, damage, delay, idx, offset, shaderFile, spriteFile, Condition::NonBackAttack, ArmorState::Invincible);
	actions[1]->IsPlayerFollow(false);

	idx = { 9, 15, 0 };
	damage = { 10.0f };
	offset = { Vector3(-250, 350, 0) };
	CreateAction(PlayerMotion::Attack1, damage, delay, idx, offset, shaderFile, spriteFile, Condition::NonBackAttack, ArmorState::Invincible);
	actions[2]->IsPlayerFollow(false);
	actions[2]->Scale(Vector3(2.0f, 2.0f, 1.5f));
	

	Scale(Vector3(1.5f, 1.5f, 1.5f));
	key = 'F';
	coolTime = 3.0f;
	skillState = PlayerState::Extractheart;
}

AA05_Extractheart::~AA05_Extractheart()
{
}

void AA05_Extractheart::Update(Matrix & V, Matrix & P)
{
	__super::Update(V, P);

	// 시작 액션은 3초 => 이 시간에 적과 닿으면 다음 타격 액션 실행
	if (startTime > 0.0f)
	{
		startTime -= Time::Delta();
		if (startTime <= 0.0f)
			ActionEnd();
	}
}

void AA05_Extractheart::SkillStart(float speed)
{

	__super::SkillStart(2.0f);

	moveTo = ColliderFactory::Get()->InRange(position, 600, ColliderTag::Monster);
	
}

void AA05_Extractheart::ActionEnd()
{
	__super::ActionEnd();
}

void AA05_Extractheart::AttackAction(GameObject* target)
{
	if (currentAction == 0)
	{
		nextAction = 1;
		moveTo = target->Position();
		actions[1]->Position(moveTo);


	}
	else if (currentAction == 1)
	{
		nextAction = 2;
		moveTo = target->Position();
		actions[2]->Position(moveTo);
		moveTo += Vector3(1, 0, 0) * target->ScaledSize().x * 0.75f * ((float)target->Direction_());
	}
}
