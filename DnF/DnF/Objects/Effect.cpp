#include "stdafx.h"
#include "Effect.h"

Effect::Effect(Clip* effect, float delay, Vector3 offset, float damage)
	:effect(effect)
	, delay(delay)
	, offset(offset)
	, damage(damage)
{
	if(damage >= 0.0f)
		collider = new BoxCollider();
}

Effect::~Effect()
{
	SafeDelete(effect);

	if (damage >= 0.0f)
		SafeDelete(collider);
}

void Effect::Update(Matrix & V, Matrix & P)
{
	// 이펙트의 오프셋 계산
	// 좌우 방향에 따라 x축상 +, - 인지를 고려
	Vector3 tempOffset = offset;
	if (this->rotation.y != 0)
		tempOffset.x = -offset.x;

	effect->Position(this->position + tempOffset);
	effect->Rotation(this->rotation);
	effect->Scale(this->scale);
	effect->Update(V, P);
}

void Effect::Render()
{
	effect->Render();
}

void Effect::Start(float speed)
{
	// 이펙트 시작
	isStart = true;

	// 이펙트(클립) 종료시 실행
	function<void()> effectEnd = [=] { isStart = false; targets.clear(); };
	effect->Play(effectEnd, 0, speed);
}

bool Effect::IsCollision(GameObject * target, Vector3 & playerPosition, bool& isAttack, bool& isBackAttack)
{
	if (collider == NULL || isStart == false)
		return false;

	for (auto t : targets)
		if (target == t)
			return false;

	if (ColliderFactory::AabbEX(target->Position(), effect->Position(), target->ScaledSize3D(), effect->GetSprite()->ScaledSize3D(), target->GetCollider(), collider))
	{
		targets.push_back(target);
		isAttack = true;
		if (effect->Rotation().y == target->Rotation().y)
			isBackAttack = true;

		return true;
	}
	return false;
}

void Effect::ColliderTypeChange(ColliderType type)
{
	BoxCollider* coll = dynamic_cast<BoxCollider*>(collider);
	if (coll != NULL)
		coll->Type(type);
}






void Effect::Position(Vector3 & position)
{
	this->position = position;
}
Vector3 Effect::Position()
{
	return position;
}

void Effect::Scale(Vector3 & scale)
{
	this->scale = scale;
}
Vector3 Effect::Scale()
{
	return scale;
}

void Effect::Rotation(Vector3 & rotation)
{
	this->rotation = rotation;
}
Vector3 Effect::Rotation()
{
	return rotation;
}

