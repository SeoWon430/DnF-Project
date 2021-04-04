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
	IFollowing* focus;	// ī�޶� ���󰡴� Ÿ��
};