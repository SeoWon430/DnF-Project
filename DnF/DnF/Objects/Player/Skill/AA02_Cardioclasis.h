#pragma once
#include "Skill_Active.h"

class AA02_Cardioclasis : public Skill_Active
{
public:
	AA02_Cardioclasis(wstring shaderFile, wstring spriteFile);
	virtual ~AA02_Cardioclasis();

	void AttackAction() override;

private:

};