#include "stdafx.h"
#include "Following.h"

// 인터페이스를 통해 따라다닐 타겟을 받음
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

// 따라 다닐 타겟 변경
void Following::Change(IFollowing* focus)
{
	this->focus = focus;
}
