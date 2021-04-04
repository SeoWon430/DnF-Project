//#pragma once
//#include "Skill_Active.h"
//
//enum class Type
//{
//	None,
//	Chain,
//	Loop
//};
//
//
//
//class A_Attack : public Skill_Active
//{
//public:
//	A_Attack(wstring name, wstring iconFile);
//	virtual ~A_Attack();
//
//	virtual void Update(Matrix & V, Matrix & P);
//	virtual void Render();
//
//	virtual void SkillStart(float speed);
//
//
//
//	void AddTarget(GameObject* target) { motions[currentAction].Target.push_back(target); }
//	bool IsExistTarget(GameObject* target);
//	void IsBackAttack(bool value) { isBackAttack = value; }
//
//	Matrix World() { return motions[currentAction].Effect->GetSprite()->World(); }
//	Collider* GetCollider() { return motions[currentAction].EffectCollider; }
//	float Damage() {return motions[currentAction].Damage; }
//
//	PlayerMotion CurrentMotion();
//
//protected:
//	virtual void ActionStart();
//	virtual void ActionNextCheck();
//	virtual void ActionEnd();
//	virtual void ActionNext();
//
//
//protected:
//	Type type = Type::None;
//
//	bool isBackAttack = false;
//};