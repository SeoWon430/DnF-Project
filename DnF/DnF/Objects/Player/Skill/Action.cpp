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
		// ����Ʈ �����ð� üũ
		if (timer >= effect->Delay() && effect->IsStart() == false)
		{
			// ����Ʈ ����
			effect->Start(speed);
		}
		// ����Ʈ ������Ʈ
		if (effect->IsStart() == true)
		{
			effect->Rotation(this->rotation);
			effect->Scale(this->scale);
			effect->Position(this->position);
			effect->Update(V, P);
		}

		// ��� ����Ʈ �������� üũ
		isEnd |= effect->IsStart();
	}


	// ��� ����Ʈ�� ������ ������ �׼� ����
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


// ��ų ����Ʈ �߰� (Ÿ��o)
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
