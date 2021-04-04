#pragma once
#include "Player.h"


class Player_Thief :public Player
{
public:
	Player_Thief(wstring name);
	~Player_Thief();


	void Update(Matrix& V, Matrix &P);
	void Render();

	
	Sprite* GetSprite();

private:
	void CreateAvataClip();
	void CreateSkill();
	void test();

private:
	RenderTarget* renderTarget;
	Canvas* canvas;

};
