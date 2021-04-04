#include "stdafx.h"
#include "P01_Claw.h"

P01_Claw::P01_Claw(wstring shaderFile, wstring spriteFile)
	:Pattern()
{
	enableStates = {
		MonsterState::Idle,
	};

	vector<UINT> idx = { 0, 10, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 6.0f };
	vector<float> delays = { 0.0f };
	vector<Vector3> offset = { Vector3(80, 120, 0) };
	CreateAction(MonsterMotion::Claw, damage, delays, idx, offset, shaderFile, spriteFile);


	Scale(Vector3(1.5f, 1.5f, 0.3f));
	coolTime = 6.0f;
	skillState = MonsterState::Claw;

	isActive = false;
	resetTime = coolTime;
	delay = 3.0f;
}

P01_Claw::~P01_Claw()
{
}
