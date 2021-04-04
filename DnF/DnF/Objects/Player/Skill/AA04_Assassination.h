#pragma once
#include "Skill_Active.h"

class AA04_Assassination : public Skill_Active
{
public:
	AA04_Assassination(wstring shaderFile, wstring spriteFile);
	virtual ~AA04_Assassination();

	void Update(Matrix & V, Matrix & P) override;

	void SkillStart(float speed) override;
	void ActionEnd() override;
	void AttackAction(GameObject* target) override;

private:
	bool isStartAttack = false;
	float startTime = 0.0f;

};