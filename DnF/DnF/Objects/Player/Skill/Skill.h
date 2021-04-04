#pragma once

class Skill
{
public:
	Skill(wstring name, wstring iconFile);
	virtual ~Skill();


	Sprite* Icon() { return icon; }

protected:
	wstring name;	// 이름
	Sprite* icon = NULL;	// 스킬 아이콘
};