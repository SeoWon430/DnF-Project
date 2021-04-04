#pragma once
#include "Skill.h"

class Skill_Passive : public Skill
{
public:
	Skill_Passive(wstring name, wstring iconFile);
	virtual ~Skill_Passive();


private:
	float buff;
};