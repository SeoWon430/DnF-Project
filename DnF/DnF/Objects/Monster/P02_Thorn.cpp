#include "stdafx.h"
#include "P02_Thorn.h"

P02_Thorn::P02_Thorn(wstring shaderFile, wstring spriteFile)
	:Pattern()
{
	enableStates = {
		MonsterState::Idle,
	};

	vector<UINT> idx = { 0, 8, 0
						,0, 8, 0
						,0, 8, 0
						,0, 8, 0
						,0, 8, 0
						,0, 8, 0 };		// 클립 프레임 이미지 { 시작, 끝, 타입 }
	vector<float> damage = { 0.0f ,0.0f,0.0f ,0.0f ,0.0f ,0.0f };
	vector<float> delays = { 0.0f, 0.1f, 0.1f, 0.2f, 0.2f, 0.2f };
	vector<Vector3> offset = { Vector3(200, -150, 0)
								,Vector3(400, -225, 0)
								,Vector3(400, -75, 0)
								,Vector3(600, -300, 0)
								,Vector3(600, -150, 0)
								,Vector3(600, 0, 0) };
	CreateAction(MonsterMotion::Thorn1, damage, delays, idx, offset, shaderFile, spriteFile);

	idx = { 8, 17, 0
			,8, 17, 0
			,8, 17, 0
			,8, 17, 0
			,8, 17, 0
			,8, 17, 0 };
	damage = { 2.0f ,2.0f,2.0f ,2.0f ,2.0f ,2.0f };
	offset = { Vector3(200, -50, -50)
								,Vector3(400, -125, -125)
								,Vector3(400, 25, 25)
								,Vector3(600, -200, -200)
								,Vector3(600, -50,  -50)
								,Vector3(600, 100, 100) };
	CreateAction(MonsterMotion::Thorn2, damage, delays, idx, offset, shaderFile, spriteFile);


	Scale(Vector3(1.25f, 1.25f, 0.3f));
	coolTime = 9.0f;
	skillState = MonsterState::Thorn;

	isActive = false;
	resetTime = coolTime;
	delay = 4.0f;
}

P02_Thorn::~P02_Thorn()
{
}
