#include "stdafx.h"
#include "SkillSet.h"
#include "../Player.h"
#include "../PlayInterface.h"
#include "Skill_Active.h"
#include "Skill_Passive.h"
#include "A_Attack.h"
#include "Action.h"

SkillSet::SkillSet()
{
}

SkillSet::~SkillSet()
{
	for (auto active : actives)
		SafeDelete(active);
	for (auto passive : passives)
		SafeDelete(passive);
}

void SkillSet::Update(Matrix & V, Matrix & P)
{
	for (auto active : actives) 
	{
		active->Position(playerPosition);
		active->Update(V, P);
	}
}

void SkillSet::Render()
{
	for (auto active : actives) {
		if (active->IsAction() == true
			|| active->IsEnd() == false)
			active->Render();
	}
}


void SkillSet::AddActive(Skill_Active * skill)
{
	actives.push_back(skill);
	//keys.push_back(keyCode);
}

void SkillSet::AddPassive(Skill_Passive * skill)
{
	passives.push_back(skill);
}

void SkillSet::ActionPassive(UINT num)
{
}

void SkillSet::Attack(GameObject* target, UINT atk, Vector3& playerPosition, function<void(GameObject*)> interfaceTarget)
{
	for (auto active : actives) 
	{
		
		if (active->IsAction() == true)
		{

			function<void(float)> attack = [=](float damage) {
				target->Attacked((INT64)(atk * damage), 1.0f);
				interfaceTarget(target);
			};
			active->Attack(target, atk, playerPosition, attack);

		}
	}
}

void SkillSet::SkillAction(PlayerState state, float speed, Vector3 poistion, Vector3 rotation)
{
	// 스킬 시작 지점
	playerPosition = poistion;
	// 스킬 실행
	for (UINT i = 0; i < actives.size(); i++)
	{
		actives[i]->CurrentState(state);
		int key = actives[i]->Key();
		// 설정된 키가 없는 경우
		if (key == 0)
			continue;

		// 설정되 키 입력 && 사용 가능 상태이면 실행
		if (Key->Down(key))
		{
			bool a = actives[i]->IsEnable();
			bool b = actives[i]->IsActive();
			if (a&&b)
			{
				currentActive = i;
				// 스킬 실행 시작
				actives[i]->Rotation(rotation);
				actives[i]->Position(poistion);
				actives[i]->SkillStart(speed);
			}
		}
	}
}

Skill_Active* SkillSet::CurrentActive()
{
	if (actives[currentActive]->IsAction() == true)
		return actives[currentActive];
	return NULL;
}

