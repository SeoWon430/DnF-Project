#include "stdafx.h"
#include "A_Buff.h"
//
//A_Buff::A_Buff(wstring name, wstring iconFile)
//	:Skill_Active(name, iconFile)
//{
//}
//
//A_Buff::~A_Buff()
//{
//	for (auto motion : motions)
//	{
//		SafeDelete(motion.Effect);
//	}
//}
//
//
//void A_Buff::Update(Matrix & V, Matrix & P)
//{
//	__super::Update(V, P);
//
//	if (isAction == true) {
//		motions[currentMotionNumber].Effect->Position(position);
//		motions[currentMotionNumber].Effect->Scale(scale);
//		motions[currentMotionNumber].Effect->Rotation(rotation);
//		motions[currentMotionNumber].Effect->Update(V, P);
//	}
//
//	if (isBuffActive == true) {
//		if (durationTime > 0.0f)
//			currentTime += Time::Delta();
//
//		if(currentTime > durationTime) 
//		{
//			isBuffActive = false;
//			currentTime = 0.0f;
//		}
//	}
//	
//	if(isAction == true && isEffect == false)
//		SkillEnd();
//
//}
//
//void A_Buff::Render()
//{
//	if (isAction == false) return;
//	motions[currentMotionNumber].Effect->Render();
//}
//
//
//void A_Buff::SkillStart(float speed)
//{
//	if (isActive == false || isAction == true)
//		return;
//
//	this->speed = speed;
//	isBuffActive = true;
//	resetTime = coolTime;
//	ActionStart();
//}
//
//void A_Buff::ActionStart()
//{
//	isAction = true;
//	isActive = false;
//
//	isMotion = true;
//	isEffect = true;
//	function<void()> effectEnd = bind(&Skill_Active::EffectEnd, this);
//	motions[currentMotionNumber].Effect->Play(effectEnd, 0, this->speed);
//}
//
//void A_Buff::ActionNext()
//{
//	if (motions.size() - 1 > currentMotionNumber)
//		motionNumber = currentMotionNumber + 1;
//	else
//		ActionEnd();
//		
//}
//
//void A_Buff::ActionEnd()
//{
//	isMotion = false;
//}
//
//PlayerMotion A_Buff::CurrentMotion()
//{
//	return motions[currentMotionNumber].SkillMotion;
//}
//
