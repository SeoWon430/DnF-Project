#pragma once
#include "Scene.h"

class S00_SeriaRoom : public Scene
{
public:
	S00_SeriaRoom(SceneValues* values);
	~S00_SeriaRoom();

	void Update() override;
	void Render() override;


	Vector2 SceneArea() override;

private:

	Sprite* background;
	Vector2 mousePosition;
	class Player* player;
	class Monster* monster;

};