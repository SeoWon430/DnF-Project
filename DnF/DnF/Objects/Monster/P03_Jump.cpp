#include "stdafx.h"
#include "P03_Jump.h"

P03_Jump::P03_Jump(wstring shaderFile, wstring spriteFile)
	:Pattern()
{
	enableStates = {
		MonsterState::Idle,
	};

	vector<UINT> idx = { 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 0.0f };
	vector<float> delays = { 0.0f };
	vector<Vector3> offset = { Vector3(0, 0, 0) };
	CreateAction(MonsterMotion::Jump1, damage, delays, idx, offset, shaderFile, spriteFile);

	idx = { 5, 16, 0 };
	damage = { 9.0f };
	offset = { Vector3(0, -200, 0) };
	CreateAction(MonsterMotion::Jump2, damage, delays, idx, offset, shaderFile, spriteFile);
	actions[1]->Speed(2.5f);
	actions[1]->ColliderTypeChange(0, ColliderType::Flat);

	Scale(Vector3(5.0f, 5.0f, 1.0f));
	coolTime = 12.0f;
	skillState = MonsterState::Thorn;

	isActive = false;
	resetTime = coolTime;
	delay = 4.0f;
}

P03_Jump::~P03_Jump()
{
}
