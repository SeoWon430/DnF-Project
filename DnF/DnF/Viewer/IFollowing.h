#pragma once
#include "stdafx.h"

// 플레이어에 붙일 인터페이스
// Follwing카메라와 연결
class IFollowing
{
public:
	virtual void Focus(Vector2* position, Vector2* size) = 0;

	static void text(){};
	Vector2 focusOffset = Vector2(200, 200);
};