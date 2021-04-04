#include "stdafx.h"
#include "AB00_NeoBuff.h"


AB00_NeoBuff::AB00_NeoBuff(wstring shaderFile, wstring spriteFile)
	:Skill_Active(L"암살자의 마음가짐", spriteFile + L"/neo_buff.png")
{
	enableStates = {
		PlayerState::Idle,
		PlayerState::Walk,
		PlayerState::Run
	};
	//motions = {
	//	MotionBuff(PlayerMotion::Attack2)
	//};


	vector<UINT> idx = { 0, 19, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 0.0f };
	vector<float> delay = { 0.0f };
	vector<Vector3> offset = { Vector3(40, 25, 0) };
	CreateAction(PlayerMotion::Attack1, damage, delay, idx, offset, shaderFile, spriteFile);


	Scale(Vector3(1.0f, 1.0f, 1.0f));
	coolTime = 10.0f;
	//icon = new Sprite(spriteFile+L"/neo_buff.png", L"Skill_Icon.fx");
	key = 'H';
	skillState = PlayerState::NeoBuff;
}

AB00_NeoBuff::~AB00_NeoBuff()
{
}

