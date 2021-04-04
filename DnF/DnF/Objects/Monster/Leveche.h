#pragma once
#include "Monster.h"



class Leveche : public Monster
{
public:
	Leveche();
	~Leveche();



	void Update(Matrix& V, Matrix &P);
	void Render();

private:
	void CreateAvata();
	void test();
	void CreateSkill();


private:
};