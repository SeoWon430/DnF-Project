#include "stdafx.h"
#include "Objects/Player/Player.h"
#include "Objects/Monster/Monster.h"
#include "Objects/Monster/Leveche.h"
#include "S00_SeriaRoom.h"

S00_SeriaRoom::S00_SeriaRoom(SceneValues * values)
	:Scene(values)
{
	wstring shaderFile = L"Sprite.fx";

	//background = new Sprite(L"Seria_Room.png", shaderFile);
	background = new Sprite(L"map/TrueCoffin_02.png", shaderFile);
	background->Scale(1.5f, 1.5f);

	Audio->AddSound("BGM", "./_Sounds/terra.mid");
	Audio->AddSound("Gun", "./_Sounds/Gun.wav");
	//Audio->Play("BGM");

	this->player = values->player;

	monster = new Leveche();
}

S00_SeriaRoom::~S00_SeriaRoom()
{
	SafeDelete(background);
}

void S00_SeriaRoom::Update()
{
	Matrix V = values->MainCamera->View();
	Matrix P = values->Projection;


	background->Update(V, P);
	player->Update(V, P);
	monster->Update(V, P);

}

void S00_SeriaRoom::Render()
{
	background->Render();
	player->Render();
	monster->Render();
}

Vector2 S00_SeriaRoom::SceneArea()
{
	return background->ScaledSize();
}
