#include "stdafx.h"
#include "AA02_Cardioclasis.h"

AA02_Cardioclasis::AA02_Cardioclasis(wstring shaderFile, wstring spriteFile)
	:Skill_Active(L"심장 찌르기", spriteFile + L"/cardioclasis.png")
{
	enableStates = {
		PlayerState::Idle,
		PlayerState::Walk,
		PlayerState::Run,
		PlayerState::Attack
	};
	//motions = {
	//	MotionAttack(PlayerMotion::Sting, 10.0f),
	//	MotionAttack(PlayerMotion::DrawOff, 5.0f, Condition::BackAttack)
	//};

	vector<UINT> idx = { 0, 4, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 7.0f };
	vector<float> delay = { 0.0f };
	vector<Vector3> offset = { Vector3(40, 25, 0) };
	CreateAction(PlayerMotion::Sting, damage, delay, idx, offset, shaderFile, spriteFile, ArmorState::SuperArmor);

	idx = { 4, 8, 0 };
	damage = { 3.5f };
	offset = { Vector3(40, 40, 0) };
	CreateAction(PlayerMotion::Attack2, damage, delay, idx, offset, shaderFile, spriteFile, Condition::BackAttack, ArmorState::SuperArmor);

	Scale(Vector3(1.2f, 1.2f, 1.2f));
	key = 'S';
	coolTime = 5.0f;
	//icon = new Sprite(spriteFile + L"/cardioclasis.png", L"Skill_Icon.fx");
	skillState = PlayerState::Cardioclasis;
}

AA02_Cardioclasis::~AA02_Cardioclasis()
{
}

void AA02_Cardioclasis::AttackAction()
{
	if (currentAction == 0 && isBackAttack == true)
	{
		nextAction = 1;
	}
}
