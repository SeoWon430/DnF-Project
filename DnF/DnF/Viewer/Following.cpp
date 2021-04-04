#include "stdafx.h"
#include "Following.h"

// �������̽��� ���� ����ٴ� Ÿ���� ����
Following::Following(IFollowing * focus)
	:focus(focus)
{
}

Following::~Following()
{
}

void Following::Update()
{
	if (focus == NULL) 
		return;


	Vector2 location, size;
	focus->Focus(&location, &size);
	
	//location.x -= (float)WIDTH * 0.5f;
	//location.y -= (float)HEIGHT * 0.5f;
	position = location;

	__super::Update();
}

// ���� �ٴ� Ÿ�� ����
void Following::Change(IFollowing* focus)
{
	this->focus = focus;
}
