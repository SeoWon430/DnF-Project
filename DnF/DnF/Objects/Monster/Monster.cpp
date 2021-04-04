#include "stdafx.h"
#include "Monster.h"
#include "PatternSet.h"
#include "Pattern.h"
//#include "SkillSet.h"

Monster::Monster(wstring name)
	:GameObject()
	, name(name)
	, moveSpeed(1.0f)
	, attackSpeed(1.0f)
	, atk(1)
	, isGround(true)
	, isAttack(false)
	, groundY(0.0f)
{
	//skillset = new SkillSet();
	collider->Tag(ColliderTag::Monster);

	patternSet = new PatternSet();
}

Monster::~Monster()
{
	SafeDelete(patternSet);
}

void Monster::Update(Matrix & V, Matrix & P)
{
	__super::Update(V, P);

	if (hp <= 0)
	{
		motion = MonsterMotion::Die;
	}
	else if (isHold == true
		&& (state == MonsterState::Idle || state == MonsterState::Walk ))
	{
		motion = MonsterMotion::Hit;
	}
	else
	{
		position = animation->Position();
		position.z = position.y;

		if (patternDelay > 0.0f)
			patternDelay -= Time::Delta();

		if (turnDelay > 0.0f)
		{
			turnDelay -= Time::Delta();
			if (turnDelay <= 0.0f)
			{
				turnDelay = 1.0f;
				if (target->Position().x < position.x)
					animation->RotationDegree(0, 180, 0);
				else
					animation->RotationDegree(0, 0, 0);
			}
		}

		if (state == MonsterState::Start
			&& animation->GetCurrentClip()->EndFrame())
		{
			state = MonsterState::Idle;
			motion = MonsterMotion::Idle;
		}
		else if (state != MonsterState::Start)
		{
			state = MonsterState::Idle;
			motion = MonsterMotion::Idle;

			//PatternUpdate();

			//Move();


		}
	}

	patternSet->Update(V, P);

	collider->Update(V, P);
	collider->World(animation->GetSprite()->World());

	animation->Position(position);
	animation->Update(V, P);
	animation->Play((UINT)motion, 1.5f);
}

void Monster::Render()
{
	// Text
	{

		ImGui::Text("Monster Motion : %d", (UINT)motion);
		ImGui::Text("Monster Hp : %I64d", hp);
		ImGui::Text("Monster Position : %.2f, %.2f(%.2f)", Position().x, Position().y, Position().z);
		
	}

	animation->Render();
	//collider->Render();
	patternSet->Render();
}

void Monster::Focus(Vector2 * position, Vector2 * size)
{
	Vector2 pos;
	pos.x = animation->Position().x;
	pos.y = animation->Position().y;
	*position = pos;
	*size = focusOffset;
}

void Monster::PatternUpdate()
{
	if (patternSet->Patterns().size() == 0)
		return;

	// 이펙트의 위치
	// xy:이펙트 중심 좌표 / z:플레이어 바닥부터의 y차이 값
	Vector3 effectPosition = position;
	effectPosition.y += animation->ScaledSize().y * 0.5f;

	if (patternDelay <= 0.0f)
		patternDelay = patternSet->PatternAction(attackSpeed, effectPosition, animation->Rotation());
	else
		int d = 0;
	// 현재 진행중인 스킬
	//	여러 스킬이 동시에 진행 될 수 있지만 플레이어에 적용 될모션은 1개뿐임
	Pattern* pattern = patternSet->CurrentActive();
	if (pattern != NULL)
	{
		// 현재 스킬에 해당하는 모션이 실행 중일 때
		if (animation->GetCurrentClip()->EndFrame() == false)
		{
			// 스킬의 모션과 상태를 가져옴
			motion = pattern->CurrentMotion();
			state = pattern->CurrentState();
			position += pattern->Move() * (float)direction;
		}
		// 스킬에 해당하는 모션이 끝났을 때
		else
		{
			pattern->ActionEnd();

		}

		// 스킬에 의한 케릭터 이동
		Vector3 skillMoveTo = pattern->MoveTo();
		if (skillMoveTo != Vector3(0, 0, 0))
			position = skillMoveTo;
		Direction directionTo = pattern->DirectionTo();
		if (directionTo != Direction::None)
			animation->RotationDegree(0, 90 - ((float)directionTo * 90), 0);

	}
}

void Monster::Move()
{
	if (!(motion == MonsterMotion::Idle
		|| motion == MonsterMotion::Walk)
		|| target == NULL)
		return;

	Vector3 current = this->Position();
	Vector3 to = target->Position();

	if (Math::Length(current, to) > 400 * 400)
	{
		to -= current;
		to.x = Math::Clamp(to.x, -250.0f, 250.0f);
		to.y = Math::Clamp(to.y, -500.0f, 500.0f);

		Position(current + to * Time::Delta());
		if (to.x < 0)
			animation->RotationDegree(0, 180, 0);
		else
			animation->RotationDegree(0, 0, 0);


		motion = MonsterMotion::Walk;
	}
	else
		motion = MonsterMotion::Idle;



}


void Monster::PatternAttack(GameObject* target)
{

	patternSet->Attack(target, atk, Position());
}