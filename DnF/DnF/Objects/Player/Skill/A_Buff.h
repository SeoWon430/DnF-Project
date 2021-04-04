//#pragma once
//#include "Skill_Active.h"
//
//
//class A_Buff : public Skill_Active
//{
//public:
//	A_Buff(wstring name, wstring iconFile);
//	virtual ~A_Buff();
//
//	virtual void Update(Matrix & V, Matrix & P) override;
//	virtual void Render();
//
//	virtual void SkillStart(float speed);
//	virtual void ActionStart();
//	virtual void ActionNext();
//	virtual void ActionEnd();
//
//	PlayerMotion CurrentMotion();
//	//void EffectEnd();
//
//protected:
//	vector<MotionBuff> motions;
//	float durationTime = 0.0f;
//	float currentTime = 0.0f;
//	bool isBuffActive = false;
//
//};