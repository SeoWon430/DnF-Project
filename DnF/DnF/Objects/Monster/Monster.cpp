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

	// ����Ʈ�� ��ġ
	// xy:����Ʈ �߽� ��ǥ / z:�÷��̾� �ٴں����� y���� ��
	Vector3 effectPosition = position;
	effectPosition.y += animation->ScaledSize().y * 0.5f;

	if (patternDelay <= 0.0f)
		patternDelay = patternSet->PatternAction(attackSpeed, effectPosition, animation->Rotation());
	else
		int d = 0;
	// ���� �������� ��ų
	//	���� ��ų�� ���ÿ� ���� �� �� ������ �÷��̾ ���� �ɸ���� 1������
	Pattern* pattern = patternSet->CurrentActive();
	if (pattern != NULL)
	{
		// ���� ��ų�� �ش��ϴ� ����� ���� ���� ��
		if (animation->GetCurrentClip()->EndFrame() == false)
		{
			// ��ų�� ��ǰ� ���¸� ������
			motion = pattern->CurrentMotion();
			state = pattern->CurrentState();
			position += pattern->Move() * (float)direction;
		}
		// ��ų�� �ش��ϴ� ����� ������ ��
		else
		{
			pattern->ActionEnd();

		}

		// ��ų�� ���� �ɸ��� �̵�
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