#include "stdafx.h"
#include "Skill_Active.h"

Skill_Active::Skill_Active(wstring name, wstring iconFile)
	:Skill(name, iconFile)
{
}

Skill_Active::~Skill_Active()
{
	//for (auto e : effect)
	//	SafeDelete(e);
}

void Skill_Active::Update(Matrix & V, Matrix & P)
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

void Skill_Active::Render()
{
	for (auto action : actions)
	{
		// �������� ��� �׼� Render
		//	(Motion, Effect ��� ������ isStart = false��)
		if (action->IsStart() == true)
			action->Render();
	}
}



// ��ų ����
//	Action�� ó������ ����, ��Ÿ�� ����
// Action�� Motion�� Effect�� ���� (��� �ϳ��� ���� �� ����)
//		Motion : �÷��̾ ������ Ŭ�� ���
//		Effect : ��ų�� ����Ʈ Ŭ�� (�������ϼ� ������, Collider�� ������ ����)
void Skill_Active::SkillStart(float speed)
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
	// ���� �������̸� ���� ���� ����
	else if (isAction == true && type == Type::Chain)
	{
		ActionNextCheck();
	}
}



// Action ����
void Skill_Active::ActionStart()
{
	isAttack = false;
	actions[currentAction]->Start(position, scale, rotation, speed);
}



// ���� Action ���� �������� �Ǵ�
//	�����ϸ� nextAction = currentAction + 1
void Skill_Active::ActionNextCheck()
{
	
	// ���ۿ� ���ǿ� ���� ����
	if (actions.size() - 1 > currentAction)
	{
		switch (actions[currentAction + 1]->Condition_())
		{
		// ���� ������ �ٷ� ���� ����
		case  Condition::None:
			nextAction = currentAction + 1;
			break;
		// ������ ���� ���� �� ��
		case  Condition::Jump:
			if (currentState == PlayerState::Jump)
				nextAction = currentAction + 1;
			break;

		default:
			break;
		}
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
void Skill_Active::ActionNext()
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

void Skill_Active::AttackAction()
{
}

void Skill_Active::AttackAction(GameObject* target)
{
}



// ���� �������� Action ����
//	���� �׼����� ����
void Skill_Active::ActionEnd()
{
	// ü�� ���� �϶�
	if (type == Type::Chain)
	{
		ActionNext();
	}
	else
	{
		ActionNextCheck();
		ActionNext();
	}
}



// ��� Action�� Motion�� ���� ��
//	(Action�� Effect�� �ȳ� ������ ����)
void Skill_Active::SkillEnd()
{
	isAction = false;
	nextAction = 0;
	currentAction = 0;
}



// ��ų�� ����
//	Action�� ���� ��� Effect�� üũ
void Skill_Active::Attack(GameObject * target, UINT atk, Vector3 & playerPosition, function<void(float)> attack)
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



void Skill_Active::CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile)
{
	CreateAction(motion, damage, delay, imageIndex, offset, shaderFile, spriteFile, Condition::None, ArmorState::Normal);
}

void Skill_Active::CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile, ArmorState armorState)
{
	CreateAction(motion, damage, delay, imageIndex, offset, shaderFile, spriteFile, Condition::None, armorState);
}

void Skill_Active::CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile, Condition condition)
{
	CreateAction(motion, damage, delay, imageIndex, offset, shaderFile, spriteFile, condition, ArmorState::Normal);
}

// ��ų�� Action�� ����
void Skill_Active::CreateAction(PlayerMotion motion, vector<float> damage, vector<float> delay, vector<UINT> imageIndex, vector<Vector3> offset, wstring shaderFile, wstring spriteFile, Condition condition, ArmorState armorState)
{
	Action* action = new Action((UINT)motion, condition, armorState);

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



SkillData Skill_Active::CurrentSkillData()
{
	SkillData data;
	data.Motion = static_cast<PlayerMotion>(actions[currentAction]->Motion());
	data.State = skillState;
	data.Speed = speed;
	data.Move = actions[currentAction]->Move();
	data.ArmorState_ = actions[currentAction]->GetArmorState();
	return data;
}

SkillMoveTo Skill_Active::CurrentSkillMoveTo()
{
	SkillMoveTo data;
	data.Direction_ = direction;
	data.MoveTo = moveTo;
	moveTo = Vector3(0, 0, 0);

	return data;
}



void Skill_Active::Position(float x, float y)
{
	Position(Vector2(x, y));
}
void Skill_Active::Position(Vector2 & vec)
{
	position.x = vec.x;
	position.y = vec.y;
}
void Skill_Active::Position(Vector3 & vec)
{
	position = vec;
}

void Skill_Active::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}
void Skill_Active::Scale(Vector2 & vec)
{
	scale.x = vec.x;
	scale.y = vec.y;
}
void Skill_Active::Scale(Vector3 & vec)
{
	scale = vec;
}

void Skill_Active::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}
void Skill_Active::Rotation(Vector3 & vec)
{
	rotation = vec;
}
void Skill_Active::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}
void Skill_Active::RotationDegree(Vector3 & vec)
{
	Vector3 r;
	r.x = Math::ToRadian(vec.x);
	r.y = Math::ToRadian(vec.y);
	r.z = Math::ToRadian(vec.z);

	Rotation(r);
}
Vector3 Skill_Active::RotationDegree()
{
	Vector3 rot;
	rot.x = Math::ToDegree(rotation.x);
	rot.y = Math::ToDegree(rotation.y);
	rot.z = Math::ToDegree(rotation.z);
	return rot;
}

bool Skill_Active::IsEnable()
{  
	bool enable = false;
	for (auto state : enableStates)
	{
		if (state == currentState)
		{
			enable = true;
			break;
		}
	}

	return enable;
}

