#include "stdafx.h"
#include "Pattern.h"


Pattern::Pattern()
{
}

Pattern::~Pattern()
{
	//for (auto e : effect)
	//	SafeDelete(e);
}

void Pattern::Update(Matrix & V, Matrix & P)
{
	// 재사용 대기 시간 계산
	if (isActive == false) {
		resetTime -= Time::Delta();
		if (resetTime <= 0) {
			resetTime = 0;
			isActive = true;
		}
	}

	for (auto action : actions)
	{
		// 실행중인 모든 액션 Update
		//	(Motion, Effect 모두 끝나야 isStart = false임)
		if (action->IsStart() == true)
		{
			if (action->IsPlayerFollow())
			{
				action->Position(position);
			}
			isStart = true;
			action->Update(V, P);

		}
		else {
			isStart = false;
			isAttack = false;
		}
	}

}

void Pattern::Render()
{
	for (auto action : actions)
	{
		// 실행중인 모든 액션 Render
		//	(Motion, Effect 모두 끝나야 isStart = false임)
		if (action->IsStart() == true)
			action->Render();
	}
}

void Pattern::Initialize(MonsterState* state)
{
	currentState = state;
}


// 스킬 시작
//	Action을 처음부터 시작, 쿨타임 적용
// Action은 Motion과 Effect를 가짐 (어느 하나가 없을 수 있음)
//		Motion : 플레이어가 가지는 클립 모션
//		Effect : 스킬의 이펙트 클립 (여러개일수 있으며, Collider를 가질수 있음)
void Pattern::SkillStart(float speed)
{
	moveTo = Vector3(0, 0, 0);
	nextAction = 0;
	isBackAttack = false;
	this->speed = speed;

	// 스킬 시작
	if (isAction == false)
	{
		isAction = true;
		isActive = false;
		resetTime = coolTime;	// 쿨타임 시작
		ActionStart();
	}
}



// Action 시작
void Pattern::ActionStart()
{
	isAttack = false;
	actions[currentAction]->Start(position, scale, rotation, speed);
}



// 다음 Action 진행 가능한지 판단
//	가능하면 nextAction = currentAction + 1
void Pattern::ActionNextCheck()
{

	// 동작에 조건에 따라 실행
	if (actions.size() - 1 > currentAction)
	{
		nextAction = currentAction + 1;
	}
	// 다음 동작이 없는 경우 스킬 종료
	else
	{
		isBackAttack = false;
		SkillEnd();
	}
}



// 다음 Action이 정해져 있다면 다음 액션 진행
//		ㄴ> ActionNextCheck()이 실행 되었었을 경우
// 다음 Action이 없다면 스킬 종료
void Pattern::ActionNext()
{
	if (nextAction > currentAction)
	{
		currentAction = nextAction;
		ActionStart();
	}
	else {
		SkillEnd();
	}
}

void Pattern::AttackAction()
{
}

void Pattern::AttackAction(GameObject* target)
{
}



// 현재 진행중인 Action 종료
//	사음 액션으로 진행
void Pattern::ActionEnd()
{
	ActionNextCheck();
	ActionNext();
}



// 모든 Action의 Motion이 끝날 때
//	(Action의 Effect는 안끝 났을수 있음)
void Pattern::SkillEnd()
{
	isAction = false;
	nextAction = 0;
	currentAction = 0;
}



// 스킬의 공격
//	Action이 가진 모든 Effect를 체크
void Pattern::Attack(GameObject * target, UINT atk, Vector3 & playerPosition, function<void(float)> attack)
{
	for (auto action : actions)
	{
		vector<Effect*> effects;
		if (action->IsStart() == true)
			effects = action->AttackEffects();

		for (auto effect : effects)
			if (effect->IsCollision(target, playerPosition, isAttack, isBackAttack))
			{
				attack(effect->Damage());
				AttackAction();
				AttackAction(target);
			}
	}


}



Vector3 Pattern::MoveTo()
{
	Vector3 tmp = moveTo;
	moveTo = Vector3(0, 0, 0);
	return tmp;
}

Direction Pattern::DirectionTo()
{
	return direction;
}


// 스킬의 Action을 구성
void Pattern::CreateAction(MonsterMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile)
{
	Action* action = new Action((UINT)motion, Condition::None);

	UINT length = imageIndex.size();

	for (UINT i = 0; i < length / 3; i++)
	{
		Clip* clip = NULL;
		if (imageIndex[i * 3] != -1)
		{
			clip = new Clip((PlayMode)imageIndex[i * 3 + 2], 1.0f);
			for (UINT j = imageIndex[i * 3]; j < imageIndex[i * 3 + 1]; j++)
			{
				wstring fileName = spriteFile + to_wstring(j) + L".png";
				clip->AddFrame(new Sprite(fileName, shaderFile, CenterPoint::Center), 0.2f);
			}
		}
		action->AddEffect(clip, delay[i], offset[i], damage[i]);
	}

	actions.push_back(action);
}



// 현재 스킬의 실행중인 Motion
//	Player.cpp에서 호출되어 플레이어의 모션을 결정
MonsterMotion Pattern::CurrentMotion()
{
	return static_cast<MonsterMotion>(actions[currentAction]->Motion());
}



void Pattern::Position(float x, float y)
{
	Position(Vector2(x, y));
}
void Pattern::Position(Vector2 & vec)
{
	position.x = vec.x;
	position.y = vec.y;
}
void Pattern::Position(Vector3 & vec)
{
	position = vec;
}

void Pattern::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}
void Pattern::Scale(Vector2 & vec)
{
	scale.x = vec.x;
	scale.y = vec.y;
}
void Pattern::Scale(Vector3 & vec)
{
	scale = vec;
}

void Pattern::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}
void Pattern::Rotation(Vector3 & vec)
{
	rotation = vec;
}
void Pattern::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}
void Pattern::RotationDegree(Vector3 & vec)
{
	Vector3 r;
	r.x = Math::ToRadian(vec.x);
	r.y = Math::ToRadian(vec.y);
	r.z = Math::ToRadian(vec.z);

	Rotation(r);
}
Vector3 Pattern::RotationDegree()
{
	Vector3 rot;
	rot.x = Math::ToDegree(rotation.x);
	rot.y = Math::ToDegree(rotation.y);
	rot.z = Math::ToDegree(rotation.z);
	return rot;
}

bool Pattern::IsEnable()
{
	bool enable = false;
	for (auto s : enableStates)
	{
		if (s == (*currentState))
		{
			enable = true;
			break;
		}
	}

	return enable;
}
