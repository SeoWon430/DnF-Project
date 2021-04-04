#include "stdafx.h"
#include "Skill.h"

Skill::Skill(wstring name, wstring iconFile)
	:name(name)
{
	if(iconFile != L"")
		icon = new Sprite(iconFile, L"Skill_Icon.fx");
}

Skill::~Skill()
{
	SafeDelete(icon);
}

