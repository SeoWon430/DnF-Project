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
//// 스킬 시작 (SkillSet.cpp 에서 키입력을 통해 시작)
//// 스킬은 여러개의 Motion으로 구성
////	Motion은 Effect와 Motion을 가짐
//void A_Attack::SkillStart(float speed)
//{
//	this->speed = speed;
//
//	// 스킬 시작
//	if (isAction == false)
//	{
//		resetTime = coolTime;	// 쿨타임 시작
//		ActionStart();
//	}
//	// 현재 실행중이면 다음 동작 실행
//	else if (isAction == true && type == Type::Chain)
//	{
//		ActionNextCheck();
//	}
//}
//
//// 동작 시작
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
//// 다음 동작
//void A_Attack::ActionNextCheck()
//{
//	// 동작에 조건에 따라 실행
//	if (motions.size() - 1 > currentMotionNumber)
//	{
//		switch (motions[currentMotionNumber + 1].MotionCondition)
//		{
//			// 조건 없으면 바로 다음 동작
//			case  Condition::None:
//				motionNumber = currentMotionNumber + 1;
//				break;
//			// 조건이 점프 상태 일 때
//			case  Condition::Jump:
//				if((*currentState) == PlayerState::Jump)
//					motionNumber = currentMotionNumber + 1;
//				break;
//			// 조건이 백어택 일 때
//			case  Condition::BackAttack:
//				if(isBackAttack == true)
//					motionNumber = currentMotionNumber + 1;
//				break;
//			default:
//				break;
//		}
//
//	}
//	// 다음 동작이 없는 경우 스킬 종료
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
//	// 다음 액션이 정해져 있다면 다음 액션 진행
//	if (motionNumber > currentMotionNumber)
//	{
//		currentMotionNumber = motionNumber;
//		ActionStart();
//	}
//	// 그 외는 액션 종료
//	else {
//		SkillEnd();
//	}
//}
//
//// 동작 종료
//void A_Attack::ActionEnd()
//{
//	isMotion = false;
//	// 체인 공격 일때
//	if (type == Type::Chain)
//	{
//		ActionNext();
//	}
//	// 반복 공격 일때
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
//	// 자동 공격 일때
//	else
//	{
//		ActionNextCheck();
//		ActionNext();
//	}
//}
//
//
//// 이미 타격 됐는지 체크
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
