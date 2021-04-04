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
	// ���� ��� �ð� ���
	if (isActive == false) {
		resetTime -= Time::Delta();
		if (resetTime <= 0) {
			resetTime = 0;
			isActive = true;
		}
	}

	for (auto action : actions)
	{
		// �������� ��� �׼� Update
		//	(Motion, Effect ��� ������ isStart = false��)
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
		// �������� ��� �׼� Render
		//	(Motion, Effect ��� ������ isStart = false��)
		if (action->IsStart() == true)
			action->Render();
	}
}

void Pattern::Initialize(MonsterState* state)
{
	currentState = state;
}


// ��ų ����
//	Action�� ó������ ����, ��Ÿ�� ����
// Action�� Motion�� Effect�� ���� (��� �ϳ��� ���� �� ����)
//		Motion : �÷��̾ ������ Ŭ�� ���
//		Effect : ��ų�� ����Ʈ Ŭ�� (�������ϼ� ������, Collider�� ������ ����)
void Pattern::SkillStart(float speed)
{
	moveTo = Vector3(0, 0, 0);
	nextAction = 0;
	isBackAttack = false;
	this->speed = speed;

	// ��ų ����
	if (isAction == false)
	{
		isAction = true;
		isActive = false;
		resetTime = coolTime;	// ��Ÿ�� ����
		ActionStart();
	}
}



// Action ����
void Pattern::ActionStart()
{
	isAttack = false;
	actions[currentAction]->Start(position, scale, rotation, speed);
}



// ���� Action ���� �������� �Ǵ�
//	�����ϸ� nextAction = currentAction + 1
void Pattern::ActionNextCheck()
{

	// ���ۿ� ���ǿ� ���� ����
	if (actions.size() - 1 > currentAction)
	{
		nextAction = currentAction + 1;
	}
	// ���� ������ ���� ��� ��ų ����
	else
	{
		isBackAttack = false;
		SkillEnd();
	}
}



// ���� Action�� ������ �ִٸ� ���� �׼� ����
//		��> ActionNextCheck()�� ���� �Ǿ����� ���
// ���� Action�� ���ٸ� ��ų ����
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



// ���� �������� Action ����
//	���� �׼����� ����
void Pattern::ActionEnd()
{
	ActionNextCheck();
	ActionNext();
}



// ��� Action�� Motion�� ���� ��
//	(Action�� Effect�� �ȳ� ������ ����)
void Pattern::SkillEnd()
{
	isAction = false;
	nextAction = 0;
	currentAction = 0;
}



// ��ų�� ����
//	Action�� ���� ��� Effect�� üũ
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


// ��ų�� Action�� ����
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



// ���� ��ų�� �������� Motion
//	Player.cpp���� ȣ��Ǿ� �÷��̾��� ����� ����
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
