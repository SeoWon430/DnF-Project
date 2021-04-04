#pragma once
#include "SceneDungeon.h"

class S01_DungeonSiroco : public SceneDungeon
{
public:
	S01_DungeonSiroco(SceneValues* values);
	~S01_DungeonSiroco();

	void Update() override;
	void Render() override;


	Vector2 SceneArea() override;

private:

};