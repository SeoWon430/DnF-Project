#pragma once
#include "Skill_Active.h"

class AA05_Extractheart : public Skill_Active
{
public:
	AA05_Extractheart(wstring shaderFile, wstring spriteFile);
	virtual ~AA05_Extractheart();

	void Update(Matrix & V, Matrix & P) override;

	void SkillStart(float speed) override;
	void ActionEnd() override;
	void AttackAction(GameObject* target) override;

private:
	bool isStartAttack = false;
	float startTime = 0.0f;

};