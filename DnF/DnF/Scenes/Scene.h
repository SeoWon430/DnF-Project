#pragma once

#include "stdafx.h"

enum class Map 
{
	SeriaRoom = 0,
	Westcoast,
	Siroco_True_01
};

struct SceneValues 
{
	class Camera* MainCamera;	// View Matrix���� ����
	Matrix Projection;
	Player* player;
};

class Scene
{
public:
	Scene(SceneValues* values) { this->values = values; }
	virtual ~Scene() {};

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual Vector2 SceneArea() = 0;

protected:
	SceneValues* values;
	Sprite* background;
	Sprite* backgroundFar;
	Vector2 mousePosition;

};