#pragma once

class Skill
{
public:
	Skill(wstring name, wstring iconFile);
	virtual ~Skill();


	Sprite* Icon() { return icon; }

protected:
	wstring name;	// �̸�
	Sprite* icon = NULL;	// ��ų ������
};