#include "stdafx.h"
#include "Leveche.h"
#include "PatternSet.h"
#include "Pattern.h"
#include "P01_Claw.h"
#include "P02_Thorn.h"
#include "P03_Jump.h"

Leveche::Leveche()
	:Monster(L"레베체")
{
	CreateAvata();
	CreateSkill();

	animation->Scale(Vector3(1.5f, 1.5f, 0.25f));
	hpMax = 40000000000;	//300억
	hp = hpMax;
	atk = 1500;

	animation->Play(0, 2.0f);
	Initalize();
}

Leveche::~Leveche()
{
}

void Leveche::Update(Matrix & V, Matrix & P)
{
	__super::Update(V, P);
}

void Leveche::Render()
{
	__super::Render();
}

void Leveche::CreateAvata()
{

	vector<UINT> idx = {
		0, 22, 0		// 등장			0
		, 22, 32, 1		// 기본			1
		, 32, 42, 1		// 걷기			2
		, 43, 54, 0		// 공격1			3
		, 54, 62, 0		// 공격2	-2		4
		, 62, 70, 0		// 공격2-2		5
		, 42, 43, 0		// 6
		, 89, 95, 0		// 7
		, 95, 97, 0		// 8
		, 70, 89, 0		// 9
	};
	//CreateMotionClip(L"Character.fx", L"monster/leveche/leveche_", idx);
	test();

}

void Leveche::test()
{
	Clip* clip;
	UINT start[] = { 632, 1058, 1476, 1852, 2240, 2698, 3092, 3520, 3932, 4370 };
	UINT mode[] = { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 };

	vector<UINT> offset[] = { {547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632, 547,632}
								, {323,318, 323,318, 323,318 ,323,318 ,323,318 ,323,318 ,323,318 ,323,318 ,323,318 ,323,318}
								, {262,314 ,262,314 ,262,314 ,262,314 ,262,314 ,262,314 ,262,314 ,262,314 ,262,314 ,262,314}
								, {290,322 , 290,322 ,290,322 ,290,322 ,290,322 ,290,322 ,290,322 ,290,322 ,290,322 ,290,322 ,290,322}
								, {430,363 , 430,363 ,430,363 ,430,363 ,430,363 ,430,363 ,430,363 ,430,363}
								, {430,363 , 430,363 ,430,363 ,430,363 ,430,363 ,430,363 ,430,363 ,430,363}
								, {289,314}
								, {455,394 , 455,394 ,455,394 ,455,394 ,455,394 ,455,394}
								, {455,394 , 455,394}
								, {275 ,373, 380 ,373, 342 ,373, 329 ,373, 320 ,373, 289 ,373, 280 ,373, 264 ,373, 264 ,373, 264 ,373, 266 ,373, 308 ,373, 402 ,373, 440 ,373, 444 ,373, 440 ,373, 137 ,373, 24 ,373, 2 ,373}
								
	};
	//{ 64,132 , 64,133 , 63,133 , 64,132 , 65,133 , 69,132 , 72,133 , 76,133 , 75,133 , 64,133 , 68,132 , 65,133 };



	// 위 idx를 가지고 도적 케릭터에 대한 이미지를 받아 클립으로 저장
	for (UINT i = 0; i < ARRAYSIZE(start); i++) {
		clip = new Clip((PlayMode)mode[i], 1.0f);
		float startX = 0;
		float startY = 0;
		float endX = 0;
		float endY = 0;
		for (UINT j = 0; j < offset[i].size(); j += 2) {
			startX = endX;
			endY = start[i];
			endX = startX + offset[i][j];
			startY = endY - (offset[i][j + 1]);
			clip->AddFrame(new Sprite(L"monster/leveche/00.png", L"Character.fx", startX, startY, endX, endY, CenterPoint::CenterDown), 0.2f);
		}

		// 클립 저장
		animation->AddClip(clip);
	}
}

void Leveche::CreateSkill()
{
	wstring shaderFile = L"Effect.fx";
	patternSet->AddPattern(new P01_Claw(shaderFile, L"monster/leveche/skill/claw/"));
	patternSet->AddPattern(new P02_Thorn(shaderFile, L"monster/leveche/skill/thorn/"));
	patternSet->AddPattern(new P03_Jump(shaderFile, L"monster/leveche/skill/jump/"));



	patternSet->Initialize(&state);
}
