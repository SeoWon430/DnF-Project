#pragma once
#include "stdafx.h"

// �÷��̾ ���� �������̽�
// Follwingī�޶�� ����
class IFollowing
{
public:
	virtual void Focus(Vector2* position, Vector2* size) = 0;

	static void text(){};
	Vector2 focusOffset = Vector2(200, 200);
};