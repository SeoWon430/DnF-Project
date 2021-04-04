#pragma once
#include "Skill_Active.h"

class AA01_ShadowCut : public Skill_Active
{
public:
	AA01_ShadowCut(wstring shaderFile, wstring spriteFile);
	virtual ~AA01_ShadowCut();

	void ActionEnd() override;
	void AttackAction(GameObject* target) override;

private:
	GameObject* target = NULL;

};