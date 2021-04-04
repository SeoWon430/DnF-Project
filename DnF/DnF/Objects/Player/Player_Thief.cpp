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
	// �ִϸ��̼� ����
	animation->Position(position);
	animation->Scale(Vector3(1.5f, 1.5f, 0.2f));
	//renderTarget = new RenderTarget();
	//canvas = new Canvas(Vector2(600, 400), animation->ScaledSize());

	// ���� �ƹ�Ÿ Ŭ�� ����
	CreateAvataClip();

	// ���� ��ų ����
	CreateSkill();

	// ������ �⺻ �÷��̾� �ӵ����� ���� ���� ��
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
		0, 12, 1		// ����		0
		, 12, 18, 1		// �������		1
		, 18, 27, 1		// �ȱ�			2
		, 46, 51, 0		// ���� 	����		3
		, 51, 53, 0		// ����	��		4
		, 53, 60, 0		// ����	����		5
		, 61, 67, 1		// �޸���		6
		, 91, 93, 0		// �ǰ�			7
		, 93, 96, 0		// �ٿ�			8
		, 73, 78, 0		// ��Ÿ1			9	5
		, 76, 72, 0		// ��Ÿ2			10	4
		, 97, 101, 0	// ��Ÿ3			11	4
		, 102, 106, 0	// ��Ÿ4			12	4
		, 67, 71, 0		// ���		13	4
		, 73, 77, 0		// ���⻩����	14	4
		, 82, 85, 0		// ������		15
		, 85, 89, 0		// ����			16
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
	


	// �� idx�� ������ ���� �ɸ��Ϳ� ���� �̹����� �޾� Ŭ������ ����
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

		// Ŭ�� ����
		animation->AddClip(clip);
	}
}



void Player_Thief::CreateSkill()
{
	wstring shaderFile = L"Effect.fx";

	// ���� ��ų
	{
		// X : ��Ÿ
		skillset->AddActive(new AA00_Attack(shaderFile, L"thief/skill/attack/"));
		
		// V : ������ ��
		skillset->AddActive(new AA01_ShadowCut(shaderFile, L"thief/skill/shadowcut/"));

		// S : ���� ���
		skillset->AddActive(new AA02_Cardioclasis(shaderFile, L"thief/skill/cardioclasis/"));

		// F : Į����ǳ
		//skillset->AddActive(new AA03_BladeGust(shaderFile, L"thief/skill/bladegust/"));

		// D : �ϻ�
		skillset->AddActive(new AA04_Assassination(shaderFile, L"thief/skill/assassination/"));

		// F : ����
		skillset->AddActive(new AA05_Extractheart(shaderFile, L"thief/skill/extractheart/"));

	}

	// ���� ��ų
	{
		// A : �ϻ����� ��������
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
