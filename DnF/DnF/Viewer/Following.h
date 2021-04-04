#pragma once
#include "IFollowing.h"

class Following : public Camera
{
public:
	Following(IFollowing* focus = NULL);
	~Following();

	void Update() override;
	void Change(IFollowing* focus);


private:
	IFollowing* focus;	// 카메라가 따라가는 타겟
};