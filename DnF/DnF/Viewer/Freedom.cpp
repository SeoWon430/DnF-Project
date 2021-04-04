#include "stdafx.h"
#include "Freedom.h"

Freedom::Freedom(float speed)
	:speed(speed)
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{

	/*if (Key->Press(VK_LEFT))
		Move(Vector2(-1, 0) * speed);
	if (Key->Press(VK_RIGHT))
		Move(Vector2(1, 0) * speed);
	if (Key->Press(VK_UP))
		Move(Vector2(0, 1) * speed);
	if (Key->Press(VK_DOWN))
		Move(Vector2(0, -1) * speed);
*/
	__super::Update();
}



void Freedom::Move(Vector2 translation)
{
	position += translation * Time::Delta();
}
