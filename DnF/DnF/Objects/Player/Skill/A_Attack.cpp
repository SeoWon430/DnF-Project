#include "stdafx.h"
#include "A_Attack.h"
//
//A_Attack::A_Attack(wstring name, wstring iconFile)
//	:Skill_Active(name, iconFile)
//{
//}
//
//A_Attack::~A_Attack()
//{
//	for (auto motion : motions)
//	{
//		SafeDelete(motion.Effect);
//		SafeDelete(motion.EffectCollider);
//	}
//}
//
//void A_Attack::Update(Matrix & V, Matrix & P)
//{
//	__super::Update(V, P);
//	if (isAction == true) {
//		motions[currentAction].EffectCollider->World(World());
//		motions[currentAction].EffectCollider->Update(V, P);
//
//		motions[currentAction].Effect->Position(position);
//		motions[currentAction].Effect->Scale(scale);
//		motions[currentAction].Effect->Rotation(rotation);
//		motions[currentAction].Effect->Update(V, P);
//	}
//
//	if (type == Type::Loop)
//	{
//		if (isAction == true && durationTime > 0.0f)
//		{
//			currentTime += Time::Delta();
//
//			if (durationTime < currentTime)
//			{
//				isBackAttack = false;
//				currentTime = 0.0f;
//				SkillEnd();
//			}
//		}
//	}
//}
//
//
//// ��ų ���� (SkillSet.cpp ���� Ű�Է��� ���� ����)
//// ��ų�� �������� Motion���� ����
////	Motion�� Effect�� Motion�� ����
//void A_Attack::SkillStart(float speed)
//{
//	this->speed = speed;
//
//	// ��ų ����
//	if (isAction == false)
//	{
//		resetTime = coolTime;	// ��Ÿ�� ����
//		ActionStart();
//	}
//	// ���� �������̸� ���� ���� ����
//	else if (isAction == true && type == Type::Chain)
//	{
//		ActionNextCheck();
//	}
//}
//
//// ���� ����
//void A_Attack::ActionStart()
//{
//	isAction = true;
//	isActive = false;
//
//	isMotion = true;
//	
//	isEffect = true;
//	function<void()> effectEnd = bind(&Skill_Active::EffectEnd, this);
//	motions[currentMotionNumber].Effect->Play(effectEnd, 0, this->speed);
//}
//
//// ���� ����
//void A_Attack::ActionNextCheck()
//{
//	// ���ۿ� ���ǿ� ���� ����
//	if (motions.size() - 1 > currentMotionNumber)
//	{
//		switch (motions[currentMotionNumber + 1].MotionCondition)
//		{
//			// ���� ������ �ٷ� ���� ����
//			case  Condition::None:
//				motionNumber = currentMotionNumber + 1;
//				break;
//			// ������ ���� ���� �� ��
//			case  Condition::Jump:
//				if((*currentState) == PlayerState::Jump)
//					motionNumber = currentMotionNumber + 1;
//				break;
//			// ������ ����� �� ��
//			case  Condition::BackAttack:
//				if(isBackAttack == true)
//					motionNumber = currentMotionNumber + 1;
//				break;
//			default:
//				break;
//		}
//
//	}
//	// ���� ������ ���� ��� ��ų ����
//	else
//	{
//		isBackAttack = false;
//		SkillEnd();
//	}
//}
//
//void A_Attack::ActionNext()
//{
//
//	motions[currentMotionNumber].Target.clear();
//	// ���� �׼��� ������ �ִٸ� ���� �׼� ����
//	if (motionNumber > currentMotionNumber)
//	{
//		currentMotionNumber = motionNumber;
//		ActionStart();
//	}
//	// �� �ܴ� �׼� ����
//	else {
//		SkillEnd();
//	}
//}
//
//// ���� ����
//void A_Attack::ActionEnd()
//{
//	isMotion = false;
//	// ü�� ���� �϶�
//	if (type == Type::Chain)
//	{
//		ActionNext();
//	}
//	// �ݺ� ���� �϶�
//	else if (type == Type::Loop)
//	{
//		if (durationTime >= currentTime)
//		{
//			isActive = true;
//			isAction = false;
//			ActionStart();
//		}
//		else
//		{
//			isBackAttack = false;
//			SkillEnd();
//		}
//	}
//	// �ڵ� ���� �϶�
//	else
//	{
//		ActionNextCheck();
//		ActionNext();
//	}
//}
//
//
//// �̹� Ÿ�� �ƴ��� üũ
//bool A_Attack::IsExistTarget(GameObject * target)
//{
//	for (auto t : motions[currentMotionNumber].Target)
//	{
//		if (t == target)
//			return true;
//	}
//	return false;
//}
//
//
//PlayerMotion A_Attack::CurrentMotion()
//{
//	return motions[currentMotionNumber].SkillMotion;
//}
