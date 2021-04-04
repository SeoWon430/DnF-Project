#pragma once
#include "Scene.h"

class SceneDungeon : public Scene
{
public:
	SceneDungeon(SceneValues* values);
	~SceneDungeon();


protected:
	class Player* player;
	class Monster* monster;

};