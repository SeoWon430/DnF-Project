#include "stdafx.h"
#include "PlayInterface.h"
#include "Skill/Skillset.h"
#include "Skill/Skill_Active.h"


PlayInterface* PlayInterface::instance = NULL;

PlayInterface::PlayInterface()
{
	// 플레이어의 인터페이스
	{
		interfaceFrame = new Sprite(L"interface/interface.png", L"Sprite.fx", CenterPoint::CenterDown);
		interfaceFrame->Scale(1.2f, 1.2f);

		// 각 스킬칸의 키 설정
		{
			keys[0] = 'A';
			keys[1] = 'S';
			keys[2] = 'D';
			keys[3] = 'F';
			keys[4] = 'G';
			keys[5] = 'H';
			keys[6] = 'B';

			keys[7] = 'V';
			keys[8] = VK_LSHIFT;
			keys[9] = VK_CONTROL;
			keys[10] = VK_MENU;
			keys[11] = VK_NUMPAD0;
			keys[12] = VK_RSHIFT;
			keys[13] = VK_NEXT;
		}
	
		// 각 스킬칸의 위치 (가로7칸, 세로2칸)
		// 스킬 아이콘이 해당 위치에 랜더 됨
		{
			int x = -97;
			int y = 26;
			int z = 37;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					positionSkill[(i*7)+j] = Vector2(x + j*z, y + i*z);
				}
			}
		}


		hpBar = new Sprite(L"interface/hpbar.png", L"HpPlayer.fx");
		mpBar = new Sprite(L"interface/mpbar.png", L"HpPlayer.fx");
		hpBar->Scale(1.1f, 1.1f);
		mpBar->Scale(1.1f, 1.1f);
		positionHp = Vector2(-167, 42);
		positionMp = Vector2(184, 42);
		fillHpBar = hpBar->GetShader()->AsScalar("fillHp");
		fillMpBar = hpBar->GetShader()->AsScalar("fillHp");
	}


	// 몬스터(target)의 인터페이스
	{
		// 몬스터의 체력 프레임
		targetInterface = new Sprite(L"interface/targetInterface.png", L"Sprite.fx");
		targetInterface->Scale(1.2f, 1.2f);
		targetInterfaceOffset = Vector2(WIDTH*-0.5f + targetInterface->ScaledSize().x*0.5f + 20, HEIGHT * 0.4f);

		// 몬스터의 작은 체력바
		targetHpAll = new Sprite(L"interface/targetHpAll.png", L"HpMonster.fx");
		targetHpAll->Scale(1.2f, 1.2f);
		fillHpAll = targetHpAll->GetShader()->AsScalar("fillHpAll");

		// 몬스터의 큰 체력바
		wstring textureFile;
		for (UINT i = 0; i < 5; i++)
		{
			textureFile = L"interface/hp" + to_wstring(i) + L".png";
			targetHp[i] = new Sprite(textureFile, L"HpMonster.fx");
			targetHp[i]->Scale(1.2f, 1.2f);
			fillHp[i] = targetHp[i]->GetShader()->AsScalar("fillHpAll");
		}
	}


}

PlayInterface::~PlayInterface()
{
	for (UINT i = 0; i < 5; i++)
		SafeDelete(targetHp[i]);

	SafeDelete(targetHpAll);
	SafeDelete(targetInterface);
	SafeDelete(interfaceFrame);
}

void PlayInterface::Create()
{
	assert(instance == NULL);

	instance = new PlayInterface();
}

void PlayInterface::Delete()
{
	SafeDelete(instance);
}

PlayInterface * PlayInterface::Get()
{
	return instance;
}

void PlayInterface::Update(Matrix & V, Matrix & P)
{

	// 플레이어 인터페이스 업데이트
	PlayerUpdate(V, P);
	interfaceFrame->Update(V, P);

	// 타격한 몬스터 Hp 인터페이스 업데이트
	if (target != nullptr)
	{
		TargetUpdate();

		targetInterface->Update(V, P);
		targetHpAll->Update(V, P);
		targetHp[nextLine]->Update(V, P);
		targetHp[currentLine]->Update(V, P);
	}
}

void PlayInterface::Render()
{
	interfaceFrame->Render();

	if (target != nullptr)
	{
		targetInterface->Render();
		targetHpAll->Render();
		targetHp[nextLine]->Render();
		targetHp[currentLine]->Render();

		DirectWrite::GetDC()->BeginDraw();
		{
			wstring str;
			RECT rect = { 660, 70, 760, 170 };
			str = to_wstring(ratioHpTarget) + L"%";
			DirectWrite::RenderText(str, rect);
		}
		DirectWrite::GetDC()->EndDraw();
	}
	PlayerRender();
}

void PlayInterface::SetCamera(Camera * camera)
{
	this->camera = camera;
}

void PlayInterface::SetSkillset(SkillSet * skillset)
{
	
	for (UINT i = 0; i < KeyCount; i++)
	{
		for (auto skill : skillset->Actives())
		{
			if (keys[i] != skill->Key()) continue;
			
			skills[i] = skill;
			Sprite* icon = skill->Icon();
			if (icon != NULL)
			{
				icon->Scale(1.15f, 1.15f);
				skillCoolTime[i] = icon->GetShader()->AsScalar("coolTime");
			}
			
		}
	}

}


void PlayInterface::SetTarget(GameObject * target)
{
	this->target = target;
	targetTime = 0;
}

void PlayInterface::UpdatePlayerHp(INT64 hp, INT64 hpMax)
{
	float fill = (float)hp / (float)hpMax;
	ratioHp = (int)(fill*100);
	fillHpBar->SetFloat(fill);
}

void PlayInterface::UpdatePlayerMp(INT64 mp, INT64 mpMax)
{
	float fill = (float)mp / (float)mpMax;
	fillMpBar->SetFloat(fill);
}

void PlayInterface::PlayerUpdate(Matrix& V, Matrix &P)
{
	Vector2 position = camera->Position();
	position.y -= HEIGHT * 0.5f;
	interfaceFrame->Position(position);

	for (UINT i = 0; i < KeyCount; i++)
	{
		if (skills[i] == NULL) continue;

		Sprite* icon = skills[i]->Icon();
		if (icon == NULL) continue;

		icon->Position(position + positionSkill[i]);
		icon->Update(V, P);
	}

	hpBar->Position(position + positionHp);
	hpBar->Update(V, P);
	mpBar->Position(position + positionMp);
	mpBar->Update(V, P);
}

void PlayInterface::PlayerRender()
{
	Vector2 position = camera->Position();
	for (UINT i = 0; i < KeyCount; i++)
	{
		if (skills[i] == NULL) continue;


		if (skillCoolTime[i] != NULL)
		{
			float coolTime;
			if (skills[i]->IsEnable() == true)
				coolTime = skills[i]->ResetTime() / skills[i]->CoolTime();
			else
				coolTime = 1.0f;
			skillCoolTime[i]->SetFloat(coolTime);
		}

		Sprite* icon = skills[i]->Icon();
		if (icon == NULL) continue;
		icon->Render();
	}
	hpBar->Render();
	mpBar->Render();

	DirectWrite::GetDC()->BeginDraw();
	{
		wstring str;
		RECT rect = { 525, 740, 625, 840 };
		str = to_wstring(ratioHp) + L"%";
		DirectWrite::RenderText(str, rect);
	}
	DirectWrite::GetDC()->EndDraw();
}

void PlayInterface::TargetUpdate()
{
	// 몬스터 인터페이스 위치를 카메라 위치에 맞게 조정
	Vector2 position = camera->Position();
	targetInterface->Position(position + targetInterfaceOffset);

	// 타격한 몬스터 Hp 인터페이스 업데이트
	Vector2 offset = targetInterface->Position();
	UINT64 hp = target->Hp();
	UINT64 hpMax = target->HpMax();
	ratioHpTarget = (int)(hp / (hpMax/100));
	{
		// 큰 체력바 (1줄당 10억단위)
		offset.x += 18;
		offset.y += 3;
		targetHp[nextLine]->Position(offset);
		targetHp[currentLine]->Position(offset);
		UINT64 lineHp = hp % PerLine;
		float hpRate = (float)lineHp / PerLine;
		fillHp[currentLine]->SetFloat(hpRate);

		// 체력바 줄 색상 변화 (빨->주->초->파->보->...)
		UINT currentHpCount = (UINT) (hp / PerLine);
		UINT gap = hpCount - currentHpCount;
		if (gap > 0)
		{
			fillHp[nextLine]->SetFloat(1);
			fillHp[currentLine]->SetFloat(1);

			currentLine += gap;
			nextLine += gap;
			currentLine %= 5;
			nextLine %= 5;

			hpCount = currentHpCount;
		}


		// 작은 체력바 (전체 체력의 비율(0~100%))
		offset.y -= 21;
		targetHpAll->Position(offset);
		hpRate = (float)hp / hpMax;
		fillHpAll->SetFloat(hpRate);

	}


	// 타격 대상 인터페이스 표시는 10초간 유지
	targetTime += Time::Delta();
	if (targetTime >= 10)
	{
		target = nullptr;
		targetTime = 0;
	}
}
