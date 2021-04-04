#include "stdafx.h"
#include "AA04_Assassination.h"

AA04_Assassination::AA04_Assassination(wstring shaderFile, wstring spriteFile)
	:Skill_Active(L"암살", spriteFile + L"/assassination.png")
{
	enableStates = {
		PlayerState::Idle,
		PlayerState::Walk,
		PlayerState::Run,
		PlayerState::Attack
	};

	vector<UINT> idx = { 0, 7, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 2.5f };
	vector<float> delay = { 0.0f };
	vector<Vector3> offset = { Vector3(0, 0, 0) };
	CreateAction(PlayerMotion::Run, damage, delay, idx, offset, shaderFile, spriteFile, ArmorState::SuperArmor);
	actions[0]->Move(Vector3(200, 0, 0));
	actions[0]->Scale(Vector3(0.5f, 0.5f, 1.0f));
	actions[0]->Speed(0.75f);

	idx = { 8, 13, 0 };
	damage = { 7.5f };
	offset = { Vector3(0, 50, 0) };
	CreateAction(PlayerMotion::Slash, damage, delay, idx, offset, shaderFile, spriteFile, Condition::NonBackAttack, ArmorState::Invincible);

	idx = { 14, 21, 0 };
	damage = { 12.5f };
	delay = { 0.0f };
	CreateAction(PlayerMotion::Slash, damage, delay, idx, offset, shaderFile, spriteFile, Condition::BackAttack);
	actions[2]->Speed(2.5f);
	actions[2]->IsPlayerFollow(false);
	actions[2]->Scale(Vector3(1.25f, 1.25f, 1.0f));

	key = 'D';
	coolTime = 9.0f;
	//icon = new Sprite(spriteFile + L"/assassination.png", L"Skill_Icon.fx");
	//icon = new Sprite(spriteFile + L"/cardioclasis.png", L"Skill_Icon.fx");
	skillState = PlayerState::Assassination;
}

AA04_Assassination::~AA04_Assassination()
{
}

void AA04_Assassination::Update(Matrix & V, Matrix & P)
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

void AA04_Assassination::SkillStart(float speed)
{
	currentAction = 0;
	startTime = 2.0f;
	__super::SkillStart(1.0f);
}

void AA04_Assassination::ActionEnd()
{
	__super::ActionEnd();
}

void AA04_Assassination::AttackAction(GameObject* target)
{
	if (currentAction == 0)
	{
		if (isBackAttack == true)
		{
			nextAction = 2;
			moveTo = target->Position();
			moveTo += Vector3(1, 0, 0) * target->ScaledSize().x * 0.75f * ((float)target->Direction_());


			Vector3 pos = moveTo;
			pos.y = PlayInterface::Get()->ScreenCenter().y;
			actions[2]->Position(pos);
		}
		else {
			nextAction = 1;
			speed = 2.0f;
		}
		ActionNext();

	}
	


}
