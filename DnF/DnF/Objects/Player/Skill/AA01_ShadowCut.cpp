#include "stdafx.h"
#include "AA01_ShadowCut.h"

AA01_ShadowCut::AA01_ShadowCut(wstring shaderFile, wstring spriteFile)
	:Skill_Active(L"섀도우 컷", spriteFile + L"/shadowcut.png")
{
	enableStates = {
		PlayerState::Idle,
		PlayerState::Walk,
		PlayerState::Run,
		PlayerState::Attack
	};

	vector<UINT> idx = { 0, 5, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 2.0f };
	vector<float> delay = { 0.0f };
	vector<Vector3> offset = { Vector3(48, 10, 0) };
	CreateAction(PlayerMotion::Attack1, damage, delay, idx, offset, shaderFile, spriteFile, ArmorState::SuperArmor);


	Scale(Vector3(1.5f, 1.5f, 1.0f));
	key = 'A';
	coolTime = 4.0f;
	skillState = PlayerState::ShadowCut;
}

AA01_ShadowCut::~AA01_ShadowCut()
{
}

void AA01_ShadowCut::ActionEnd()
{
	__super::ActionEnd();

	if (target != NULL)
	{
		int targetDirection = ((int)target->Direction_());
		moveTo = target->Position();
		moveTo += Vector3(1, 0, 0) * target->ScaledSize().x * (-0.5f) * (float)targetDirection;
		target = NULL;
		direction = (Direction) targetDirection;
	}
}

void AA01_ShadowCut::AttackAction(GameObject * target)
{
	if (currentAction == 0)
	{
		this->target = target;
		
	}
}


