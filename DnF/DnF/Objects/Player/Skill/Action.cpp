#include "stdafx.h"
#include "Action.h"
#include "Skill_Active.h"

Action::Action(UINT motion, Condition condition, ArmorState armorState)
	:motion(motion)
	,condition(condition)
	, armorState(armorState)
{
}

Action::~Action()
{
	for (auto effect : effects)
	{
		SafeDelete(effect);
	}
}

void Action::Start(Vector3 position, Vector3 scale, Vector3 rotation, float speed)
{
	if(isPlayerFollow == true)
		this->position = position;
	this->rotation = rotation;
	if (isScaled == false)
		this->scale = scale;
	if (isSetSpeed == false)
		this->speed = speed;

	timer = 0;
	isStart = true;
}

void Action::Update(Matrix V, Matrix P)
{
	timer += Time::Delta();
	bool isEnd = false;
	

	for (auto effect : effects)
	{
		// 이펙트 지연시간 체크
		if (timer >= effect->Delay() && effect->IsStart() == false)
		{
			// 이펙트 시작
			effect->Start(speed);
		}
		// 이펙트 업데이트
		if (effect->IsStart() == true)
		{
			effect->Rotation(this->rotation);
			effect->Scale(this->scale);
			effect->Position(this->position);
			effect->Update(V, P);
		}

		// 모든 이펙트 끝났는지 체크
		isEnd |= effect->IsStart();
	}


	// 모든 이펙트가 끝나면 현재의 액션 종료
	if (isEnd == false)
		isStart = false;

}

void Action::Render()
{
	if (isStart == false)
		isStart = false;
	for (auto effect : effects)
	{
		if (effect->IsStart() == true)
		{
			effect->Render();
		}
	}
}


// 스킬 이펙트 추가 (타격o)
void Action::AddEffect(Clip * effect, float delay, Vector3 offset, float damage)
{
	effects.push_back(new Effect(effect, delay, offset, damage));
}

bool Action::IsAttack()
{
	return false;
}


void Action::Scale(Vector3 & vec)
{
	isScaled = true;
	scale = vec;
}

void Action::Speed(float sp)
{
	isSetSpeed = true;
	speed = sp;
}

void Action::ColliderTypeChange(UINT index, ColliderType type)
{
	effects[index]->ColliderTypeChange(type);
}
