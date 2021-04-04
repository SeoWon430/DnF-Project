#pragma once
#include "Skill_Active.h"

class AA00_Attack : public Skill_Active
{
public:
	AA00_Attack(wstring shaderFile, wstring spriteFile);
	virtual ~AA00_Attack();

	void Update(Matrix & V, Matrix & P) override;
	void Render() override;

	void SkillStart(float speed)override;

private:
	UINT jumpAttack = 0;
	UINT jumpAttackLimit = 2;

};