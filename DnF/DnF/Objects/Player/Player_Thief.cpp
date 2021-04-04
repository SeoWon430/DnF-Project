#include "stdafx.h"
#include "Player_Thief.h"
#include "Skill/Skill_Active.h"
#include "Skill/AA00_Attack.h"
#include "Skill/AA01_ShadowCut.h"
#include "Skill/AA02_Cardioclasis.h"
#include "Skill/AA03_BladeGust.h"
#include "Skill/AA04_Assassination.h"
#include "Skill/AA05_Extractheart.h"
#include "Skill/AB00_NeoBuff.h"
#include "Skill/SkillSet.h"

Player_Thief::Player_Thief(wstring name)
	:Player(name)
{
	// 애니메이션 설정
	animation->Position(position);
	animation->Scale(Vector3(1.5f, 1.5f, 0.2f));
	//renderTarget = new RenderTarget();
	//canvas = new Canvas(Vector2(600, 400), animation->ScaledSize());

	// 도적 아바타 클립 생성
	CreateAvataClip();

	// 도적 스킬 생성
	CreateSkill();

	// 도적은 기본 플레이어 속도보다 높게 설정 됨
	moveSpeed = 1.8f;
	attackSpeed = 3.0f;
	atk = 300000000;
	hpMax = 70000;
	hp = hpMax;

	animation->Play(0);
	Initalize();
}

Player_Thief::~Player_Thief()
{
	//SafeDelete(canvas);
	//SafeDelete(renderTarget);
}



void Player_Thief::CreateAvataClip()
{
	
	vector<UINT> idx = {
		0, 12, 1		// 평상시		0
		, 12, 18, 1		// 전투대기		1
		, 18, 27, 1		// 걷기			2
		, 46, 51, 0		// 점프 	시작		3
		, 51, 53, 0		// 점프	끝		4
		, 53, 60, 0		// 점프	공격		5
		, 61, 67, 1		// 달리기		6
		, 91, 93, 0		// 피격			7
		, 93, 96, 0		// 다운			8
		, 73, 78, 0		// 평타1			9	5
		, 76, 72, 0		// 평타2			10	4
		, 97, 101, 0	// 평타3			11	4
		, 102, 106, 0	// 평타4			12	4
		, 67, 71, 0		// 찌르기		13	4
		, 73, 77, 0		// 무기빼내기	14	4
		, 82, 85, 0		// 슬래시		15
		, 85, 89, 0		// 추출			16
		, 176, 181, 0	// Invisible
	};
	//CreateMotionClip(L"Character.fx", L"thief/thief_", idx);
	test();
}

void Player_Thief::test()
{
	Clip* clip;
	UINT start[] = { 136, 272, 423, 572, 694, 831, 962, 1091, 1181, 1313, 1445, 1560, 1682, 1818, 1942, 2064, 2197, 2390 };
	UINT mode[] = { 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 };

	vector<UINT> offset[] = { {64,132 , 64,133 , 63,133 , 64,132 , 65,133 , 69,132 , 72,133 , 76,133 , 75,133 , 64,133 , 68,132 , 65,133}
								, {80,118 , 78,119 , 87,120 , 87,118 , 72,119 , 78,120}
								, {62,132 , 68,133 , 88,129 , 70,127 , 68,128 , 73,129 , 91,129 , 67,130 , 62,131}
								, {70,104 , 74,119 , 92,93 , 104,128 , 105,131}
								, {87,104 , 70,102}
								, {69,119 , 98,103 , 95,88 , 109,99 , 86,108 , 77,107 , 67,107 }
								, {105,93 , 94,111 , 92,115 , 103,88 , 91,114 , 92,117}
								, {71,100 , 66,112}
								, {100,73 , 112,66 , 126,64}
								, {103,107 , 75,115 , 72,115 , 73,115 , 77,115}
								, {73,115 , 72,115 , 75,115 , 103,107}
								, {127,99 , 101,95 , 101,82 , 101,80}
								, {94,104 , 127,99 , 129,92 , 127,88}
								, {89,109 , 87,103 , 124,115 , 98,103}
								, {103,107 , 75,115 , 72,115 , 73,115}
								, {103,105 , 103,83 , 103,83}
								, {93,116 , 88,115 , 119,121 , 103,100}
								, {0,1 , 0,1 , 0,1 , 0,1}
							};
	//{ 64,132 , 64,133 , 63,133 , 64,132 , 65,133 , 69,132 , 72,133 , 76,133 , 75,133 , 64,133 , 68,132 , 65,133 };
	


	// 위 idx를 가지고 도적 케릭터에 대한 이미지를 받아 클립으로 저장
	for (UINT i = 0; i < ARRAYSIZE(start); i++) {
		clip = new Clip((PlayMode)mode[i], 1.0f);
		float startX = 0;
		float startY = 0;
		float endX = 0;
		float endY = 0;
		for (UINT j = 0; j < offset[i].size() ; j+=2) {
			startX = endX;
			endY = start[i];
			endX = startX + offset[i][j] + 4;
			startY = endY - (offset[i][j+1] + 4);
			clip->AddFrame(new Sprite(L"thief/thief.png", L"Character.fx", startX, startY, endX, endY, CenterPoint::CenterDown), 0.2f);
		}

		// 클립 저장
		animation->AddClip(clip);
	}
}



void Player_Thief::CreateSkill()
{
	wstring shaderFile = L"Effect.fx";

	// 공격 스킬
	{
		// X : 평타
		skillset->AddActive(new AA00_Attack(shaderFile, L"thief/skill/attack/"));
		
		// V : 섀도우 컷
		skillset->AddActive(new AA01_ShadowCut(shaderFile, L"thief/skill/shadowcut/"));

		// S : 심장 찌르기
		skillset->AddActive(new AA02_Cardioclasis(shaderFile, L"thief/skill/cardioclasis/"));

		// F : 칼날돌풍
		//skillset->AddActive(new AA03_BladeGust(shaderFile, L"thief/skill/bladegust/"));

		// D : 암살
		skillset->AddActive(new AA04_Assassination(shaderFile, L"thief/skill/assassination/"));

		// F : 적출
		skillset->AddActive(new AA05_Extractheart(shaderFile, L"thief/skill/extractheart/"));

	}

	// 버프 스킬
	{
		// A : 암살자의 마음가짐
		skillset->AddActive(new AB00_NeoBuff(shaderFile, L"thief/skill/neo_buff/"));
	}


	PlayInterface::Get()->SetSkillset(skillset);


}


void Player_Thief::Update(Matrix & V, Matrix & P)
{
	__super::Update(V, P);
}

void Player_Thief::Render()
{
	//Key->Render();
	__super::Render();
}


Sprite * Player_Thief::GetSprite()
{
	return animation->GetSprite();
}
